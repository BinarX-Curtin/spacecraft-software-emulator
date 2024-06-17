/**
 * @file emulator.h
 * @author Tristan Ward
 * @brief Binar X Emulator source file
 *
 */

#include "emulator/public/emulator.h"

#include <string.h>

#include <array>

#include "emulator_definitions/emulator_definitions.h"

namespace binarx::emulator {

using namespace emulator_definitions;

uint16_t CalculateNumberOfPackets(uint16_t data_size) {
  // Calculate number of packets
  uint16_t num_packets = data_size / kPacketLength;
  // Add an extra packets if the data above has a reminder
  if (data_size % kPacketLength != 0) {
    num_packets++;
  }
  return num_packets;
}

void BinarXEmulator::Run() {
  // Wait for button push
  if (!button_pressed_) {
    return;
  }
  // Turn on payload
  gpo_payload_switch_.SetLow();
  // Yellow LED show's that Payload is Powered
  gpo_yellow_led_.SetLow();
  // Delay to let Payload capacitors discharge
  uint32_t delay = time_controller_.GetTicks() + 3000;
  while(time_controller_.GetTicks() < delay){}
  
  BinarXEmulator::RunStartInfo();

  // Start the timeout timer by storing the now time
  uint32_t emulator_timeout =
      time_controller_.GetTicks() + kWaitForPayloadMaxTime;

  // Turn on payload
  gpo_payload_switch_.SetHigh();
  // Yellow LED show's that Payload is Powered
  gpo_yellow_led_.SetHigh();

  // Calculate when the payload should stop running in ticks
  uint32_t payload_total_runtime = time_controller_.GetTicks() + kPayloadTotalRuntime;
  // Calculate when the payload should send it's data to the emulator
  uint32_t payload_communication_runtime = time_controller_.GetTicks() + kPayloadCommunicationRuntime;

  // turn on green LED to show emulator is in scheduled cycle
  gpo_green_led_.SetHigh();

  // Wait for payload to run it's full scheduled cycle
  while(payload_total_runtime > time_controller_.GetTicks() && 
          payload_status_ != PayloadDataStatus::kTransferCompleted)
  {
    // Check if the payload data collection time has elapsed
    if(payload_communication_runtime < time_controller_.GetTicks())
    {
      // Run the communication function
      PayloadCommunicationHandler();
    }
  }

//  // Turn off payload
//  gpo_payload_switch_.SetLow();

  // Reset the emulator states
  button_pressed_ = false;
  payload_status_ = PayloadDataStatus::kWaitingForPayload;

  BinarXEmulator::RunEndInfo();

  // Turn payload on so it can be flashed by the students
  gpo_payload_switch_.SetHigh();
}

void BinarXEmulator::PayloadCommunicationHandler() {
  // create the packet buffer
  std::array<uint8_t, kPacketLength> packet_buffer = {0};

  // Set chip select High
  gpo_payload_chip_select_.SetLow();
  // Receive the metadata
  binarx_serial_interface::SerialStatus serial_status =
      payload_communication_.Receive(packet_buffer.data(),
                                     kNumberOfBytesInHeader,
                                     kDefaultCommunicationDelay);

  // check if the header was received correctly and extract the number of
  // packets
  if (packet_buffer[0] != kSyncByte) {
    // try again
    attempt_counter_++;

    // Check how many times we attempted to get the metadata
    if (attempt_counter_ < kAllowedMetadataAttempts) {
      // Return out to try again
    	gpo_payload_chip_select_.SetHigh();
      return;
    }

    // Error in receiving the metadata
    payload_status_ = PayloadDataStatus::kErrorWithMetadataPacket;
  }

  // reset counter as the sync byte was received or because we tried too many
  // times and error message will be sent
  attempt_counter_ = 0;

  //if (payload_status_ == PayloadDataStatus::kPayloadReady) 
  if(payload_status_ != PayloadDataStatus::kErrorWithMetadataPacket)
  {
    const uint16_t kDataSize =
        ((uint16_t)packet_buffer[1] << 8) | packet_buffer[2];
    uint16_t num_of_packets = CalculateNumberOfPackets(kDataSize);
    if (kDataSize > kMaxPayloadDataLength || kDataSize == 0) 
    {
      // Error
      payload_status_ = PayloadDataStatus::kErrorDataSize;
    } 
    else 
    {
      std::array<uint8_t, kMaxPayloadDataLength> data_buffer = {0};
      for (uint8_t i = 0; i < num_of_packets; i++) 
      {
        serial_status = payload_communication_.Receive(
            packet_buffer.data(), kPacketLength, kDefaultCommunicationDelay);
        // Break if any of the transactions fail
        if (serial_status != binarx_serial_interface::SerialStatus::Success) 
        {
          payload_status_ = PayloadDataStatus::kFailureToReceiveAllPackets;
          break;
        }
        uint16_t location_start_for_packet = i * kPacketLength;
        // copy data from packet buffer to data buffer
        std::copy(packet_buffer.begin(), packet_buffer.end(),
                  data_buffer.begin() + location_start_for_packet);
      }
      gpo_payload_chip_select_.SetHigh();
      // Turn on the red LED to inform that the payload information was
      // received
      gpo_red_led_.SetHigh();

      // Check the status of the SPI transaction
      if (payload_status_ != PayloadDataStatus::kFailureToReceiveAllPackets) {
        // Send the data over UART if SPI data was received successfully
        computer_communication_.Transmit(data_buffer.data(), kDataSize,
                                          kDefaultCommunicationDelay);
        payload_status_ = PayloadDataStatus::kDataReceivedSuccessfully;
      }
    }
  }

  // If the transaction did not succeed then we need to handle the error
  if (payload_status_ != PayloadDataStatus::kDataReceivedSuccessfully) {
    ErrorHandler();
  }

  // Move to the next state as the communication has been completed
  payload_status_ = PayloadDataStatus::kTransferCompleted;

  // Turn off LED
  gpo_red_led_.SetLow();
  //Comms ended turn chip select back to high
  gpo_payload_chip_select_.SetHigh();
}

void BinarXEmulator::Init() {
  // turn on Yellow LED
  gpo_yellow_led_.SetHigh();
  // Power the payload so it can be flashed
  gpo_payload_switch_.SetHigh();
  // Set chip select to low
  gpo_payload_chip_select_.SetHigh();
}

void BinarXEmulator::PayloadReadyInterruptCallback() {
  if (payload_status_ == PayloadDataStatus::kWaitingForPayload) {
    payload_status_ = PayloadDataStatus::kPayloadReady;
  }
}
void BinarXEmulator::ButtonPressCallback() {
  button_pressed_ = true;
  // Power off the payload as it was on so it could be flashed.
//  gpo_payload_switch_.SetLow();
  //  assert the emulator status
  payload_status_ = PayloadDataStatus::kWaitingForPayload;
}

void BinarXEmulator::RunStartInfo() {
  // Print a message to the Serial Monitor to inform the students
  uint8_t info_msg[] =
      "\r\nINFO: Button pressed turning payload on"
      " and waiting for SPI transmission \r\n";
  computer_communication_.Transmit(info_msg, sizeof(info_msg),
                                   kDefaultCommunicationDelay);
}

void BinarXEmulator::RunEndInfo() {
  // Print a message to the Serial Monitor to inform the students
  uint8_t info_msg[] = "\r\nINFO: Payload scheduled operation completed\r\n";
  computer_communication_.Transmit(info_msg, sizeof(info_msg),
                                   kDefaultCommunicationDelay);

  // turn off LED
  gpo_green_led_.SetLow();
}

void BinarXEmulator::ErrorHandler() {
  constexpr char metadata_error_msg[] =
      "ERROR: The number of packets that must be sent by the payload was "
      "not received correctly \r\n";
  constexpr char too_many_packets_error_msg[] =
      "ERROR: The number of packets is too large and there will not be "
      "enough space to store the information \r\n";
  constexpr char data_received_error_msg[] =
      "ERROR: Sorry the message was not received correctly by the Binar "
      "Emulator \r\n";
  constexpr char default_error_msg[] =
      "ERROR: Sorry an error occurred with the Binar Emulator \r\n";

  switch (payload_status_) {
    case PayloadDataStatus::kErrorWithMetadataPacket:
      computer_communication_.Transmit((uint8_t *)metadata_error_msg,
                                       strlen(metadata_error_msg),
                                       kDefaultCommunicationDelay);
      break;
    case PayloadDataStatus::kErrorDataSize:
      computer_communication_.Transmit((uint8_t *)too_many_packets_error_msg,
                                       strlen(too_many_packets_error_msg),
                                       kDefaultCommunicationDelay);
      break;
    case PayloadDataStatus::kFailureToReceiveAllPackets:
      computer_communication_.Transmit((uint8_t *)data_received_error_msg,
                                       strlen(data_received_error_msg),
                                       kDefaultCommunicationDelay);
      break;
    default:
      computer_communication_.Transmit((uint8_t *)default_error_msg,
                                       strlen(default_error_msg),
                                       kDefaultCommunicationDelay);
  }
}

}  // namespace binarx::emulator

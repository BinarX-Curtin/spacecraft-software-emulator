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
  BinarXEmulator::RunStartInfo();

  // Start the timeout timer by storing the now time
  uint32_t emulator_timeout =
      time_controller_.GetTicks() + kWaitForPayloadMaxTime;

  // Wait for payload data transfer or until timeout
  while (emulator_timeout > time_controller_.GetTicks() &&
         payload_status_ != PayloadDataStatus::kTrasferCompleted) {
    PayloadCommunicationHandler();
  }

  // Reset the emulator states
  button_pressed_ = false;
  payload_status_ = PayloadDataStatus::kWaitingForPayload;

  BinarXEmulator::RunEndInfo();
}

void BinarXEmulator::PayloadCommunicationHandler() {
  // return if the payload is not ready
  if (payload_status_ != PayloadDataStatus::kPayloadReady) {
    return;
  }

  // Turn on the Green LED to inform that the payload ready interrupt was
  // received
  gpio_controller_.SetHigh(binarx_gpio_interface::GpioSelector::RedLed);

  // create the packet buffer
  std::array<uint8_t, kPacketLength> packet_buffer = {0};

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
    if (attempt_counter_ <= kAllolwedMetadataAttempts) {
      // Return out to try again
      return;
    }

    // Error in receiving the metadata
    payload_status_ = PayloadDataStatus::kErrorWithMetadataPacket;
  }

  // reset counter as the Synkbyte was received or because we tried too many
  // times and error mesage will be sent
  attempt_counter_ = 0;

  if (payload_status_ == PayloadDataStatus::kPayloadReady) {
    const uint16_t kDataSize =
        ((uint16_t)packet_buffer[1] << 8) | packet_buffer[2];
    uint16_t num_of_packets = CalculateNumberOfPackets(kDataSize);
    if (kDataSize > kMaxPayloadDataLength || kDataSize == 0) {
      // Error
      payload_status_ = PayloadDataStatus::kErrorDataSize;
    } else {
      std::array<uint8_t, kMaxPayloadDataLength> data_buffer = {0};
      for (uint8_t i = 0; i < num_of_packets; i++) {
        serial_status = payload_communication_.Receive(
            packet_buffer.data(), kPacketLength, kDefaultCommunicationDelay);
        // Break if any of the transactions fail
        if (serial_status != binarx_serial_interface::SerialStatus::Success) {
          payload_status_ = PayloadDataStatus::kFaiulureToReceiveAllPackets;
          break;
        }
        uint16_t location_start_for_packet = i * kPacketLength;
        // copy data from packet buffer to data buffer
        std::copy(packet_buffer.begin(), packet_buffer.end(),
                  data_buffer.begin() + location_start_for_packet);
      }

      // Check the status of the SPI transaction
      if (payload_status_ == PayloadDataStatus::kPayloadReady) {
        // Send the data over UART if SPI data was received succesfully
        computer_communication_.Transmit(data_buffer.data(), kDataSize,
                                         kDefaultCommunicationDelay);
        payload_status_ = PayloadDataStatus::kDataReceivedSuccesfully;
      }
    }
  }

  // If the transaction did not succeed then we need to handle the error
  if (payload_status_ != PayloadDataStatus::kDataReceivedSuccesfully) {
    ErrorHandler();
  }

  // Move to the next state as the communication has been completed
  payload_status_ = PayloadDataStatus::kTrasferCompleted;

  // Turn off Green LED
  gpio_controller_.SetLow(binarx_gpio_interface::GpioSelector::RedLed);
}

void BinarXEmulator::Init() {
  // turn on Yellow LED
  gpio_controller_.SetHigh(binarx_gpio_interface::GpioSelector::YellowLed);
}

void BinarXEmulator::PayloadReadyInterruptCallback() {
  if (payload_status_ == PayloadDataStatus::kWaitingForPayload) {
    payload_status_ = PayloadDataStatus::kPayloadReady;
  }
}
void BinarXEmulator::ButtonPressCallback() { button_pressed_ = true; }

void BinarXEmulator::RunStartInfo() {
  // turn on red LED
  gpio_controller_.SetHigh(binarx_gpio_interface::GpioSelector::GreenLed);
  // Print a message to the Serial Monitor to inform the students
  uint8_t info_msg[] =
      "\r\nINFO: Button pressed and waiting for SPI transmission \r\n";
  computer_communication_.Transmit(info_msg, sizeof(info_msg),
                                   kDefaultCommunicationDelay);
}

void BinarXEmulator::RunEndInfo() {
  // Print a message to the Serial Monitor to inform the students
  uint8_t info_msg[] = "\r\nINFO: Turning payload off\r\n";
  computer_communication_.Transmit(info_msg, sizeof(info_msg),
                                   kDefaultCommunicationDelay);

  // turn on red LED
  gpio_controller_.SetLow(binarx_gpio_interface::GpioSelector::GreenLed);
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
      "ERROR: Sorry an error occured with the Binar Emulator \r\n";

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
    case PayloadDataStatus::kFaiulureToReceiveAllPackets:
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

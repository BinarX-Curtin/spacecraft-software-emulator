/**
 * @file emulator_liason.cc
 * @author Tristan Ward
 * @brief Binar X Emulator source file
 *
 */

#include "emulator_liason/public/emulator_liason.h"

#include "emulator_definitions/emulator_definitions.h"

namespace binarx::emulator_liason {

using namespace emulator_definitions;

void EmulatorLiason::Transmit(const uint8_t *data, const uint16_t data_size) {
  if (payload_status_ == PayloadDataStatus::kCapturingData) {
    // make sure the pin is low
    gpo_payload_ready_.SetLow();

    // Calculate number of packets
    uint16_t num_packets = data_size / kPacketLength;
    // Add an extra packets if the division above has a reminder
    if (data_size % kPacketLength != 0) {
      num_packets++;
    }

    // calculate size of data transfered
    bytes_to_send = num_packets * kPacketLength + kNumberOfBytesInHeader;

    // Populate the header
    buffer_data[0] = kSyncByte;
    buffer_data[1] = static_cast<uint8_t>(data_size >> 8);
    buffer_data[2] = static_cast<uint8_t>(data_size);

    // fill the buffer with the data
    if (data_size <= kMaxPayloadDataLength) {
      for (uint16_t i = 0; i < data_size; i++) {
        buffer_data[i + kNumberOfBytesInHeader] = data[i];
      }
    }
    payload_status_ = PayloadDataStatus::kPayloadReadyToTransmit;

    // TODO(tristan): ChipSelect does not change the behaviour. move this line
    // to ChipSelectInterrupt()
    emulator_communication_.TransmitIt(buffer_data.data(), bytes_to_send);
    // set data ready pin high
    gpo_payload_ready_.SetHigh();
  } else if (payload_status_ == PayloadDataStatus::kPayloadReadyToTransmit) {
    // Trigger interrupts just incase the emulator missed it
    gpo_payload_ready_.Toggle();
  } else if (payload_status_ == PayloadDataStatus::kTrasferCompleted) {
    payload_status_ = PayloadDataStatus::kCapturingData;
  }
}

void EmulatorLiason::ChipSelectInterrupt() {
  payload_status_ = PayloadDataStatus::kChipSelectTriggered;
}

void EmulatorLiason::TransmitCallBackInterrupt() {
  payload_status_ = PayloadDataStatus::kTrasferCompleted;
}
}  // namespace binarx::emulator_liason

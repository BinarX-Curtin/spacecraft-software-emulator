/**
 * @file emulator.h
 * @author Tristan Ward
 * @brief Binar X Emulator source file
 *
 */

#include "emulator_liason/public/emulator_liason.h"

#include <string.h>

#include "emulator_definitions/emulator_definitions.h"

namespace binarx::emulator_liason {

using namespace emulator_definitions;

void EmulatorLiason::Transmit(uint8_t *data, uint16_t data_size) {
  if (payload_status_ == PayloadDataStatus::kCapturingData) {
    // make sure the pin is lowa
    gpo_payload_ready_.SetLow();

    // Calculate number of packets
    uint16_t num_packets = data_size / kPacketLength;
    // Add an extra packets if the data above has a reminder
    if (data_size % kPacketLength != 0) {
      num_packets++;
    }

    // create a buffer with
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

    emulator_communication_.TransmitIt(buffer_data.data(), bytes_to_send);
    // set data ready pin high
    gpo_payload_ready_.SetHigh();
  } else if (payload_status_ == PayloadDataStatus::kPayloadReadyToTransmit) {
    // Trigger another interrupt just incase the emulator missed it
    gpo_payload_ready_.Toggle();
  } else if (payload_status_ == PayloadDataStatus::kTrasferCompleted) {
    payload_status_ = PayloadDataStatus::kCapturingData;
  }
}

void EmulatorLiason::ChipSelectInterrupt() {
  // change to chip select
  payload_status_ = PayloadDataStatus::kHeaderSent;
}

void EmulatorLiason::TransmitCallBackInterrupt() {
  payload_status_ = PayloadDataStatus::kTrasferCompleted;
}
}  // namespace binarx::emulator_liason

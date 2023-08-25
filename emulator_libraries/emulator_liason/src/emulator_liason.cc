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
  // make sure the pin is low
  gpo_payload_ready_.SetLow();

  // Calculate number of packets
  uint16_t num_packets = data_size / kPacketLength;
  // Add an extra packets if the data above has a reminder
  if (data_size % kPacketLength != 0) {
    num_packets++;
  }

  // create a buffer with
  uint16_t buffer_size = num_packets * kPacketLength + kNumberOfBytesInHeader;

  std::array<uint8_t, kMaxPayloadDataLength + kNumberOfBytesInHeader> buffer;
  buffer[0] = kSyncByte;
  buffer[1] = static_cast<uint8_t>(data_size >> 8);
  buffer[2] = static_cast<uint8_t>(data_size);

  for (uint16_t i = 0; i < data_size; i++) {
    buffer[i + kNumberOfBytesInHeader] = data[i];
  };

  // Send the data
  emulator_communication_.TransmitIt(buffer.data(), buffer_size);

  gpo_payload_ready_.SetHigh();

  // Wait to chip select to be high
  while (!gpi_payload_chip_select_.IsHigh()) {  // do nothing
  };

  // Wait for chip select to be low
  while (gpi_payload_chip_select_.IsHigh()) {  // do nothing
  };
}

}  // namespace binarx::emulator_liason

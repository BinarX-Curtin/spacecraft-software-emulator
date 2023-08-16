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
  gpio_controller_.SetLow(binarx_gpio_interface::GpioSelector::PayloadReady);

  // The sync byte value to know that the payload transfered data succesfully
  // and it was not random noice
  constexpr uint8_t kSyncByte = 5;
  constexpr uint8_t kNumBytesInHeader = 2;

  // Calculate number of packets
  uint16_t num_packets = data_size / kPacketLength;
  // Add an extra packets if the data above has a reminder
  if (data_size % kPacketLength != 0) {
    num_packets++;
  }

  // create a buffer with
  uint16_t buffer_size = num_packets * kPacketLength + kNumBytesInHeader;
  uint8_t buffer[buffer_size];
  buffer[0] = kSyncByte;
  buffer[1] = num_packets;

  for (uint16_t i = 0; i < data_size; i++) {
    buffer[i + 2] = data[i];
  };

  // Send the data
  emulator_communication_.Transmit(buffer, buffer_size,
                                   kDefaultCommunicationDelay);

  gpio_controller_.SetHigh(binarx_gpio_interface::GpioSelector::PayloadReady);
}

}  // namespace binarx::emulator_liason

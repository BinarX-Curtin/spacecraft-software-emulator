/**
 * @file emulator.h
 * @author Tristan Ward
 * @brief Binar X Emulator source file
 *
 */

#include "emulator_liason/public/emulator_liason.h"

#include <string.h>

namespace binarx::emulator_liason {

void EmulatorLiason::Transmit(uint8_t *data, uint16_t data_size,
                              uint32_t timeout) {
  // The sync byte value to know that the payload transfered data succesfully
  // and it was not random noice
  uint8_t kSyncByte = 5;

  // Calculate number of packets
  uint8_t num_packets = data_size / kPacketDataLength;
  // Add an extra packets if the data above has a reminder
  if (data_size % kPacketDataLength != 0) {
    num_packets++;
  }

  uint8_t num_packets_buffer[2] = {kSyncByte, num_packets};

  // Send data with number of packets
  emulator_communication_.Transmit(num_packets_buffer,
                                   sizeof(num_packets_buffer),
                                   kDefaultCommunicationDelay);

  // Send the data
  emulator_communication_.Transmit(data, data_size, kDefaultCommunicationDelay);

  gpio_controller_.SetHigh(binarx_gpio_interface::GpioSelector::PayloadReady);
}

}  // namespace binarx::emulator_liason

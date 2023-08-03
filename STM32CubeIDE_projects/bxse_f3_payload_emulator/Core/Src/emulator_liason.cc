																																																																																																																																														/**
 * @file emulator.h
 * @author Tristan Ward
 * @brief Binar X Emulator source file
 *
 */

#include "emulator_liason.h"

#include <string.h>

namespace binarx::emulator_liason {

void EmulatorLiason::Transmit(uint8_t *data, uint16_t data_size,
                              uint32_t timeout) {
  // The sync byte value to know that the payload transfered data succesfully
  // and it was not random noise
  constexpr uint8_t kSyncByte = 5;
  constexpr uint8_t kNumBytesInHeader = 2;

  // Calculate number of packets
  uint8_t num_packets = data_size / kPacketDataLength;
  // Add an extra packets if the data above has a reminder
  if (data_size % kPacketDataLength != 0) {
    num_packets++;
  }

  // create a buffer with
  uint16_t buffer_size = num_packets * kPacketDataLength + kNumBytesInHeader;
  uint8_t buffer[buffer_size] = {0};
  buffer[0] = kSyncByte;
  buffer[1] = num_packets;

  for (uint16_t i = 0; i < data_size; i++) {
    buffer[i + 2] = data[i];
  };

  // Send the data
  emulator_communication_.Transmit(buffer, buffer_size,
                                   kDefaultCommunicationDelay);

  // uint8_t num_packets_buffer[kNumBytesInHeader] = {kSyncByte, num_packets};

  // // Send data with number of packets
  // emulator_communication_.Transmit(num_packets_buffer,
  //                                  sizeof(num_packets_buffer),
  //                                  kDefaultCommunicationDelay);

  // // Send the data
  // emulator_communication_.Transmit(data, data_size,
  // kDefaultCommunicationDelay);

  gpio_controller_.SetHigh(binarx_gpio_interface::GpioSelector::PayloadReady);
}

}  // namespace binarx::emulator_liason

/**
 * @file peripheral.c
 * @author Will
 * @brief Binar X Peripheral Source (Adapted from Emulator Libraries)
 *
 */

#include "peripheral.h"

void Transmit(SPI_HandleTypeDef *hspi1, uint8_t *data) {
    // Calculate number of packets
    uint16_t data_size = sizeof(data);
    uint16_t num_packets = data_size / kPacketLength;
    // Add an extra packets if the division above has a reminder
    if (data_size % kPacketLength != 0) {
      num_packets++;
    }

    // calculate size of data transfered
    bytes_to_send = num_packets * kPacketLength + kNumberOfBytesInHeader;

    // Populate the header
    buffer_data[0] = kSyncByte;
    buffer_data[1] = data_size >> 8;
    buffer_data[2] = data_size;

    // fill the buffer with the data
    if (data_size <= kMaxPayloadDataLength) {
      for (uint16_t i = 0; i < data_size; i++) {
        buffer_data[i + kNumberOfBytesInHeader] = data[i];
      }
    }

    // TODO(tristan): ChipSelect does not change the behaviour. move this line
    // to ChipSelectInterrupt()
    HAL_SPI_Transmit_IT(hspi1, buffer_data, bytes_to_send);
}

void ChipSelectInterrupt() {
  //EMPTY FUNCTION ATM
}

void TransmitCallBackInterrupt() {
  return true;
}

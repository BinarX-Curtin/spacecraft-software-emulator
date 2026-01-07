/**
 * @file peripheral.c
 * @author Will
 * @brief Binar X Peripheral Source (Adapted from Emulator Libraries)
 *
 */

#include "peripheral.h"

/**> Max payload data size allowed */
const uint16_t kMaxPayloadDataLength = 10000;
/**> Packet Length*/
const uint16_t kPacketLength = 250;
/**> Sync byte value used to determine if The SPI transmission failed*/
const uint8_t kSyncByte = 5;
/**< the number of bytes in the payload header*/
const uint8_t kNumberOfBytesInHeader = 5;
/**< Size of the data plus any bytes extra to fill all required packets that
   * must be sent to the emulator */
uint32_t bytes_to_send;

/** Array used to construct message for SPI communication to Controller */
uint8_t buffer_data[NUM_BYTE_HEADER+MAX_PAYLOAD_DATA_LENGTH];

bool kTransferComplete;

void Transmit(uint8_t *data, uint16_t data_size) {
    // Calculate number of packets
    //uint16_t data_size = sizeof(data) / sizeof(data[0]);
    uint32_t num_packets = data_size / kPacketLength;
    // Add an extra packets if the division above has a reminder
    if (data_size % kPacketLength != 0) {
      num_packets++;
    }

    // calculate size of data transfered
    bytes_to_send = num_packets * kPacketLength + kNumberOfBytesInHeader;

    // Populate the header for 32-bit length (LITTLE-ENDIAN)
        buffer_data[0] = kSyncByte;
        buffer_data[1] = data_size;       // Least Significant Byte (LSB)
        buffer_data[2] = data_size >> 8;
        buffer_data[3] = data_size >> 16;
        buffer_data[4] = data_size >> 24; // Most Significant Byte (MSB)

    // fill the buffer with the data
    if (data_size <= kMaxPayloadDataLength) {
      for (uint16_t i = 0; i < data_size; i++) {
        buffer_data[i + kNumberOfBytesInHeader] = data[i];
      }
    }
}

void ChipSelectInterrupt(SPI_HandleTypeDef *hspi, bool kPayloadReadyToTransmit) {
	if (kPayloadReadyToTransmit) {
		HAL_SPI_Transmit(hspi, buffer_data, sizeof(buffer_data),1000);
	}
}


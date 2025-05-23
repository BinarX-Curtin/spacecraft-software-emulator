
#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdint.h>
#include "stm32L4xx_hal.h"
#include <stdbool.h>

#define NUM_BYTE_HEADER 3
#define MAX_PAYLOAD_DATA_LENGTH 10000
/**> Max payload data size allowed */
const uint16_t kMaxPayloadDataLength = 10000;
/**> Packet Length*/
const uint16_t kPacketLength = 250;
/**> Sync byte value used to determine if The SPI transmission failed*/
const uint8_t kSyncByte = 5;
/**< the number of bytes in the payload header*/
const uint8_t kNumberOfBytesInHeader = 3;
/**< Size of the data plus any bytes extra to fill all required packets that
   * must be sent to the emulator */
uint16_t bytes_to_send;

/** Array used to construct message for SPI communication to Controller */
uint8_t buffer_data[NUM_BYTE_HEADER+MAX_PAYLOAD_DATA_LENGTH];

bool kTransferComplete;

/**
* @brief Function that processes the data to be transmitted from the payload 
* to the emulator
*
*/
void Transmit(uint8_t *data, uint16_t data_size);

/**
* @brief Function that sends the data from the payload to the emulator
* 
*
*/
void ChipSelectInterrupt(SPI_HandleTypeDef *hspi1, bool kPayloadReadyToTransmit);

#endif


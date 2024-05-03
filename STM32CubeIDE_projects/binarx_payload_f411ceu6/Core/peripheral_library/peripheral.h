#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "main.h"
#include <stdbool.h>


/**> Max payload data size allowed */
uint16_t kMaxPayloadDataLength = 1000;
/**> Packet Length*/
uint16_t kPacketLength = 250;
/**> Synck byte value used to determine if The SPI transmision failed*/
uint8_t kSyncByte = 5;
/**< the number of bytes in the payload header*/
uint8_t kNumberOfBytesInHeader = 3;
/**< Size of the data plus any bytes extra to fill all required packets that
   * must be sent to the emulator */
uint16_t bytes_to_send;

/** Array used to construct message for SPI communication to Controller */
uint8_t buffer_data[1028];

#endif


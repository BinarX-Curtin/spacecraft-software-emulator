
#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdint.h>
#include "stm32L4xx_hal.h"
#include <stdbool.h>

#define NUM_BYTE_HEADER 3
#define MAX_PAYLOAD_DATA_LENGTH 10000

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


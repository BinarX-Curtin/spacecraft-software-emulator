/**
 * @file fgd02f.c
 * @author Will Blackie
 * @brief This file contains the function implementations for interacting with
 * the fgd02f Dosimeter device.
 *
 */

 #include "fgd02f.h"

SPI_HandleTypeDef* hspi;

 
void FGD02FInit(SPI_HandleTypeDef *hspi_pntr){
	hspi = hspi_pntr;

  // Check if the device ID matches the expected value
  uint8_t pID = GetRadPid();
  if (pID != CHIPID_VAL) {
    // Handle error: incorrect device ID
    return;
  }  
  // Set up the device
  HAL_Delay(5);

  uint8_t write_buf[2] = {WRITE | REFWINDOW, 0b01000000 }; // Set 1 second long measurement window in HS
  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_RESET); // CS LOW
  HAL_SPI_Transmit(hspi, write_buf, sizeof(write_buf), 1000);
  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_SET); // CS HIGH

  HAL_Delay(5);

  write_buf[0] = (WRITE | CHARGECFG_1); // Set high sensitivity and force charge off
  write_buf[1] = 0b01111001;
  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_RESET); // CS LOW
  HAL_SPI_Transmit(hspi, write_buf, sizeof(write_buf), 1000);
  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_SET); // CS HIGH

  HAL_Delay(5);

  write_buf[0] = (WRITE | CHARGECFG_2);  // turn off charging
  write_buf[1] = 0b00000000;
  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_RESET); // CS LOW
  HAL_SPI_Transmit(hspi, write_buf, sizeof(write_buf), 1000);
  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_SET); // CS HIGH

  HAL_Delay(5);

  write_buf[0] = (WRITE | TARGET);  // Set target frequency to current reference
  write_buf[1] = 0b00011101;
  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_RESET); // CS LOW
  HAL_SPI_Transmit(hspi, write_buf, sizeof(write_buf), 1000);
  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_SET); // CS HIGH

  HAL_Delay(5);

  write_buf[0] = (WRITE | THRES);  // Set the threshold frequency 30kHz in HS
  write_buf[1] = 0b00011101;
  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_RESET); // CS LOW
  HAL_SPI_Transmit(hspi, write_buf, sizeof(write_buf), 1000);
  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_SET); // CS HIGH

  HAL_Delay(5);

  write_buf[0] = (WRITE | CFG);  // enable interrupt on new data
  write_buf[1] = 0b01110000;
  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_RESET); // CS LOW
  HAL_SPI_Transmit(hspi, write_buf, sizeof(write_buf), 1000);
  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_SET); // CS HIGH

  HAL_Delay(5);

  write_buf[0] = (WRITE | CHARGECFG_2);  // Set automatic recharge mode and SET ECH high
  write_buf[1] = 0b01000001;
  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_RESET); // CS LOW
  HAL_SPI_Transmit(hspi, write_buf, sizeof(write_buf), 1000);
  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_SET); // CS HIGH
}

uint8_t GetRadPid() {
  uint8_t write_buf[2] = {READ | CHIPID,0x00};
  uint8_t read_buf[2] = {0};

  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_RESET); // CS LOW
  HAL_SPI_TransmitReceive(hspi, write_buf, read_buf, sizeof(read_buf), 1000);
  HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_SET); // CS HIGH

  return read_buf[1];
}
 
 uint32_t GetRadReference() {
   uint8_t write_buf[4] = {READ | F1R_1,0x00,0x00,0x00};

   uint8_t read_buf[4] = {0};

   HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_RESET); // CS LOW
   HAL_SPI_TransmitReceive(hspi, write_buf, read_buf, sizeof(read_buf), 1000);
   HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_SET); // CS HIGH

   uint32_t reference = ((read_buf[3] & 0x03) << 16) | (read_buf[2] << 8) | read_buf[1];

   return reference;
 }

 uint32_t GetRadSensor() {
   uint8_t write_buf[4] = {READ | F1S_1,0x00,0x00,0x00};

   uint8_t read_buf[4] = {0};

   HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_RESET); // CS LOW
   HAL_SPI_TransmitReceive(hspi, write_buf, read_buf, sizeof(read_buf), 1000);
   HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_SET); // CS HIGH

   uint32_t sensor = ((read_buf[3] & 0x03) << 16) | (read_buf[2] << 8) | read_buf[1];

   return sensor;
 }

 uint8_t GetRadTemp() {
    uint8_t write_buf[2] = {READ | TEMP,0x00};

    uint8_t read_buf[2] = {0};

    HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_RESET); // CS LOW
    HAL_SPI_TransmitReceive(hspi, write_buf, read_buf, sizeof(read_buf), 1000);
    HAL_GPIO_WritePin(GPIOB, SENSOR_SPI_CS_Pin, GPIO_PIN_SET); // CS HIGH

    uint32_t temp = read_buf[1];

    return temp;
  }
 

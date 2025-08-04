/**
 * @file icm42670.h
 * @author Will Blackie
 * @brief This file contains the function prototypes for interacting with the
 * icm42670p IMU device.
 *
 *
 */

#ifndef ICM42670P_DRIVER_H
#define ICM42670P_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32L4xx_hal.h"

// Base Addresses - User Bank 0
#define REG_MCLK_RDY             0x00
#define REG_DEVICE_CONFIG        0x01
#define REG_SIGNAL_PATH_RESET    0x02
#define REG_DRIVE_CONFIG1        0x03
#define REG_DRIVE_CONFIG2        0x04
#define REG_DRIVE_CONFIG3        0x05
#define REG_INT_CONFIG           0x06

#define REG_TEMP_DATA1           0x09
#define REG_TEMP_DATA0           0x0A

#define REG_ACCEL_DATA_X1        0x0B
#define REG_ACCEL_DATA_X0        0x0C
#define REG_ACCEL_DATA_Y1        0x0D
#define REG_ACCEL_DATA_Y0        0x0E
#define REG_ACCEL_DATA_Z1        0x0F
#define REG_ACCEL_DATA_Z0        0x10

#define REG_GYRO_DATA_X1         0x11
#define REG_GYRO_DATA_X0         0x12
#define REG_GYRO_DATA_Y1         0x13
#define REG_GYRO_DATA_Y0         0x14
#define REG_GYRO_DATA_Z1         0x15
#define REG_GYRO_DATA_Z0         0x16

#define REG_PWR_MGMT0            0x1F
#define REG_GYRO_CONFIG0         0x20
#define REG_ACCEL_CONFIG0        0x21
#define REG_TEMP_CONFIG0         0x22
#define REG_GYRO_CONFIG1         0x23
#define REG_ACCEL_CONFIG1        0x24

#define REG_FIFO_CONFIG1         0x16
#define REG_FIFO_CONFIG2         0x17
#define REG_FIFO_CONFIG3         0x18

#define REG_WHO_AM_I             0x75

// Indirect Access Registers
#define REG_BLK_SEL_W            0x79
#define REG_MADDR_W              0x7A
#define REG_M_W                  0x7B
#define REG_BLK_SEL_R            0x7C
#define REG_MADDR_R              0x7D
#define REG_M_R                  0x7E


#define WHO_AM_I_VAL            0x67  // WHO_AM_I value for ICM42670P
#define ICM42670_ADDR            0x68 << 1

void ICM42670_Init(I2C_HandleTypeDef *hi2c);

/**
 * @brief 16-bit data structure for the ICM42670P IMU device.
 *
 */
typedef struct {
  int16_t temp;
  int16_t accel_x;
  int16_t accel_y;
  int16_t accel_z;
  int16_t gyro_x;
  int16_t gyro_y;
  int16_t gyro_z;
} icm42670_data_t;

icm42670_data_t ICM42670_ReadData(I2C_HandleTypeDef *hi2c);

#ifdef __cplusplus
}
#endif

#endif  // MMC5983MA_DRIVER_H

/**
 * @file mmc5983ma.c
 * @author Will Blackie
 * @brief This file contains the function implementations for interacting with
 * the ICM42670P IMU device.
 *
 */

#include "icm42670p.h"

void ICM42670_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t data;

    // Check WHO_AM_I
    HAL_I2C_Mem_Read(hi2c, ICM42670_ADDR, REG_WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
    if (data != WHO_AM_I_VAL) {
        return;
    }

    HAL_Delay(10);



    // Configure Gyroscope (ODR: 100Hz, FSR: ±2000dps)
    // Bits: [3:0]=ODR=1001 (100Hz), [6:5]=FS_SEL=00 (±2000dps)
    data = 0b01101001;
    HAL_I2C_Mem_Write(hi2c, ICM42670_ADDR, REG_GYRO_CONFIG0, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    HAL_Delay(10);

    // Configure Gyroscope (bandpass 73Hz)
    // Bits: [2:0]=011 (73Hz)
    data = 0b00000011;
    HAL_I2C_Mem_Write(hi2c, ICM42670_ADDR, REG_GYRO_CONFIG1, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    HAL_Delay(10);

    // Configure Accelerometer (ODR: 100Hz, FSR: ±16g)
    // Bits: [3:0]=ODR=1001 (100Hz), [6:5]=FS_SEL=00 (FSR: ±16g)
    data = 0b01101001;
    HAL_I2C_Mem_Write(hi2c, ICM42670_ADDR, REG_ACCEL_CONFIG0, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    HAL_Delay(10);
    // Configure Accelerometer (bandpass 73Hz) & 8x average
    // Bits: [2:0]=011 (73Hz), [6:4]=010 (8x)
    data = 0b00100011;
    HAL_I2C_Mem_Write(hi2c, ICM42670_ADDR, REG_ACCEL_CONFIG1, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

     HAL_Delay(10);

     // Set Power Management - Enable gyro + accel in Low-Noise Mode
     // Bits: [7]=ACCEL_LP_CLK_SEL => 0, [4]=IDLE => 0, [3:2] = GYRO_MODE => 11, [1:0] = ACCEL_MODE => 11,
     data = 0b00001111;
     HAL_I2C_Mem_Write(hi2c, ICM42670_ADDR, REG_PWR_MGMT0, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
     HAL_Delay(50);
    // Route Data Ready interrupt to INT1, push-pull active high
//    data = 0x00;
//    status = HAL_I2C_Mem_Write(hi2c, ICM42670_ADDR, REG_INT_CONFIG, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
//    if (status != HAL_OK) return status;

}

icm42670_data_t ICM42670_ReadData(I2C_HandleTypeDef *hi2c) {
    icm42670_data_t data;
    uint8_t buffer[14];

    // Read 14 bytes of data starting from the first data register
    HAL_I2C_Mem_Read(hi2c, ICM42670_ADDR, REG_TEMP_DATA1, I2C_MEMADD_SIZE_8BIT, buffer, 14, HAL_MAX_DELAY);

    // Combine bytes into 16-bit values
    data.temp = (buffer[0] << 8) | buffer[1];
    data.accel_x = (buffer[2] << 8) | buffer[3];
    data.accel_y = (buffer[4] << 8) | buffer[5];
    data.accel_z = (buffer[6] << 8) | buffer[7];
    data.gyro_x = (buffer[8] << 8) | buffer[9];
    data.gyro_y = (buffer[10] << 8) | buffer[11];
    data.gyro_z = (buffer[12] << 8) | buffer[13];

    return data;
}

/**
 * @file fgd02f.h
 * @author Will Blackie
 * @brief This file contains the function prototypes for interacting with
 * the FGD-02F Dosimeter device.
 *
 */

 #ifndef FGD02F_DRIVER_H
 #define FGD02F_DRIVER_H
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
#include <stdint.h>
#include <stddef.h>  // <-- Add this
#include "stm32L4xx_hal.h"
#include "main.h"
 
 // Register map.
 #define TEMP 0x00   //  Temperature monitor
 #define RECHARGE 0x01   //   Recharge event flag (7) + Recharge request (6) + Recharge counter (3:0)
 #define NI_1 0x02   // Not Implemented
 #define F1R_1 0x03   //  Reference Counter
 #define F1R_2 0x04   //  Reference Counter 2
 #define F1R_3 0x05   //  0101 +  Reference data new value (3) + Reference counter overflow (2) + Reference Counter 3 (1:0 = 17:16)
 #define F1S_1 0x06   // Sensor Counter
 #define F1S_2 0x07   // Sensor Counter 2
 #define F1S_3 0x08   // 0101 +  Sensor data new value (3) + Sensor counter overflow (2) + Sensor Counter 3 (1:0 = 17:16)
 #define TARGET 0x09   //  Upper level target frequency
 #define THRES 0x0A   //  Lower level threshold frequency
 #define REFWINDOW 0x0B   // Reference frequency configuration(6:4) Window length selection (3:2)
 #define CHARGECFG_1 0x0C   //  Force charge in Manual Recharging mode (7) + Sensitivity Configuration (2:0)
 #define CHARGECFG_2 0x0D   //  Enable Recharging (6) + Charge mode Selection (1:0)
 #define CFG 0x0E   //  Measurement / Recharge indicator (6) + Measurements during SPI enable bit (2) + Interrupt output (1) + Enable window gating (0)
 #define NI_2 0x0F   // Not Implemented
 #define SN_1 0x10   // Serial Number 1
 #define SN_2 0x11   // Serial Number 2
 #define SN_3 0x12   // Serial Number 3
 #define CHIPID 0x13   //  Chip Identification Number (0x01)
 #define RESV 0x14   // Reserved

// REG VALUES
#define CHIPID_VAL 0x01          // Product ID.

#define READ 0x80
#define WRITE 0x40

  /**
    * @brief Initialise the FGD-02F device.
    *
    */
 void FGD02FInit(SPI_HandleTypeDef *hspi);

 /**
  * @brief Get the product ID.
  *
  * @param dosimeter Dosimeter read/write interface.
  * @return uint8_t Product ID.
  */
 uint8_t GetRadPid();
 
 /**
  * @brief Get sensor counter measurement.
  *
  * @param dosimeter Dosimeter read/write interface.
  * @return uint32_t Number of bytes written to the device.
  */
 uint32_t GetRadSensor();

  /**
  * @brief Get reference counter measurement.
  *
  * @param dosimeter Dosimeter read/write interface.
  * @return uint32_t Number of bytes written to the device.
  */
 uint32_t GetRadReference();

 
 /**
  * @brief Perform a temperature measurement
  *
  * @param dosimeter Dosimeter read/write interface.
  * @return uint32_t Number of bytes written to the device.
  */
 uint8_t GetRadTemp();
 
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif  // MMC5983MA_DRIVER_H
 

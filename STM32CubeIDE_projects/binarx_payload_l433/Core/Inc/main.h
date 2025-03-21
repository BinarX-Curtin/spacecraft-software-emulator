/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h> // Include the String data type
#include <stdio.h> // sprintf()
#include <stdbool.h> // Include the String data type
#include <stdint.h> // Include the uint data types (uint8_t)
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FC_GPIO_1_Pin GPIO_PIN_13
#define FC_GPIO_1_GPIO_Port GPIOC
#define FC_GPIO_4_Pin GPIO_PIN_0
#define FC_GPIO_4_GPIO_Port GPIOA
#define SPI1_CS_Pin GPIO_PIN_2
#define SPI1_CS_GPIO_Port GPIOA
#define SPI1_CS_EXTI_IRQn EXTI2_IRQn
#define FC_GPIO_3_Pin GPIO_PIN_3
#define FC_GPIO_3_GPIO_Port GPIOA
#define SENSOR_ADC_1_Pin GPIO_PIN_4
#define SENSOR_ADC_1_GPIO_Port GPIOA
#define SENSOR_ADC_2_Pin GPIO_PIN_5
#define SENSOR_ADC_2_GPIO_Port GPIOA
#define SENSOR_ADC_3_Pin GPIO_PIN_0
#define SENSOR_ADC_3_GPIO_Port GPIOB
#define SENSOR_ADC_4_Pin GPIO_PIN_1
#define SENSOR_ADC_4_GPIO_Port GPIOB
#define FC_GPIO_2_Pin GPIO_PIN_2
#define FC_GPIO_2_GPIO_Port GPIOB
#define DEBUG_USART_TX_Pin GPIO_PIN_10
#define DEBUG_USART_TX_GPIO_Port GPIOB
#define DEBUG_USART_RX_Pin GPIO_PIN_11
#define DEBUG_USART_RX_GPIO_Port GPIOB
#define SENSOR_SPI_CS_Pin GPIO_PIN_12
#define SENSOR_SPI_CS_GPIO_Port GPIOB
#define SENSOR_SPI_CS_EXTI_IRQn EXTI15_10_IRQn
#define SENSOR_SPI_SCK_Pin GPIO_PIN_13
#define SENSOR_SPI_SCK_GPIO_Port GPIOB
#define SENSOR_SPI_POCI_Pin GPIO_PIN_14
#define SENSOR_SPI_POCI_GPIO_Port GPIOB
#define SENSOR_SPI_PICO_Pin GPIO_PIN_15
#define SENSOR_SPI_PICO_GPIO_Port GPIOB
#define SENSOR_USART_TX_Pin GPIO_PIN_9
#define SENSOR_USART_TX_GPIO_Port GPIOA
#define SENSOR_USART_RX_Pin GPIO_PIN_10
#define SENSOR_USART_RX_GPIO_Port GPIOA
#define SENSOR_GPIO_3_Pin GPIO_PIN_12
#define SENSOR_GPIO_3_GPIO_Port GPIOA
#define SENSOR_GPIO_2_Pin GPIO_PIN_5
#define SENSOR_GPIO_2_GPIO_Port GPIOB
#define SENSOR_I2C_SCL_Pin GPIO_PIN_6
#define SENSOR_I2C_SCL_GPIO_Port GPIOB
#define SENSOR_I2C_SDA_Pin GPIO_PIN_7
#define SENSOR_I2C_SDA_GPIO_Port GPIOB
#define SENSOR_GPIO_1_Pin GPIO_PIN_8
#define SENSOR_GPIO_1_GPIO_Port GPIOB
#define SENSOR_GPIO_INT_Pin GPIO_PIN_9
#define SENSOR_GPIO_INT_GPIO_Port GPIOB
#define SENSOR_GPIO_INT_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

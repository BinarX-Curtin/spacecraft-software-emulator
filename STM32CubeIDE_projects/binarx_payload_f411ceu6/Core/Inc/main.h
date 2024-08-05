/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define Payload_Chip_Select_Pin GPIO_PIN_4
#define Payload_Chip_Select_GPIO_Port GPIOA
#define Payload_Chip_Select_EXTI_IRQn EXTI4_IRQn
#define SENSOR_GPIO_1_Pin GPIO_PIN_10
#define SENSOR_GPIO_1_GPIO_Port GPIOB
#define DEBUG_USART_TX_Pin GPIO_PIN_11
#define DEBUG_USART_TX_GPIO_Port GPIOA
#define DEBUG_USART_RX_Pin GPIO_PIN_12
#define DEBUG_USART_RX_GPIO_Port GPIOA
#define SENSOR_GPIO_2_Pin GPIO_PIN_5
#define SENSOR_GPIO_2_GPIO_Port GPIOB
#define SENSOR_GPIO_3_Pin GPIO_PIN_7
#define SENSOR_GPIO_3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define IRQ_SNS_Pin GPIO_PIN_0
#define IRQ_SNS_GPIO_Port GPIOD
#define VIN_Pin GPIO_PIN_0
#define VIN_GPIO_Port GPIOA
#define TXD2_Pin GPIO_PIN_2
#define TXD2_GPIO_Port GPIOA
#define RXD2_Pin GPIO_PIN_3
#define RXD2_GPIO_Port GPIOA
#define IRQ_TRX_Pin GPIO_PIN_4
#define IRQ_TRX_GPIO_Port GPIOA
#define ENN_Pin GPIO_PIN_6
#define ENN_GPIO_Port GPIOA
#define BEEP_Pin GPIO_PIN_7
#define BEEP_GPIO_Port GPIOA
#define LG_Pin GPIO_PIN_0
#define LG_GPIO_Port GPIOB
#define DLED_Pin GPIO_PIN_1
#define DLED_GPIO_Port GPIOB
#define BEEN_Pin GPIO_PIN_8
#define BEEN_GPIO_Port GPIOA
#define TXD1_Pin GPIO_PIN_9
#define TXD1_GPIO_Port GPIOA
#define LR_Pin GPIO_PIN_10
#define LR_GPIO_Port GPIOA
#define SND_Pin GPIO_PIN_11
#define SND_GPIO_Port GPIOA
#define SRAM_Pin GPIO_PIN_12
#define SRAM_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

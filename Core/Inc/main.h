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
#include "stm32f4xx_hal.h"

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
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define HGM_CC1190_Pin GPIO_PIN_14
#define HGM_CC1190_GPIO_Port GPIOB
#define LNA_EN_CC1190_Pin GPIO_PIN_15
#define LNA_EN_CC1190_GPIO_Port GPIOB
#define PA_EN_CC11190_Pin GPIO_PIN_6
#define PA_EN_CC11190_GPIO_Port GPIOC
#define GPIO3_CC1120_Pin GPIO_PIN_7
#define GPIO3_CC1120_GPIO_Port GPIOC
#define GPIO2_CC1120_EXTI_Pin GPIO_PIN_8
#define GPIO2_CC1120_EXTI_GPIO_Port GPIOC
#define GPIO2_CC1120_EXTI_EXTI_IRQn EXTI9_5_IRQn
#define GPIO0_CC1120_EXTI_Pin GPIO_PIN_9
#define GPIO0_CC1120_EXTI_GPIO_Port GPIOC
#define GPIO0_CC1120_EXTI_EXTI_IRQn EXTI9_5_IRQn
#define USERLED2_Pin GPIO_PIN_3
#define USERLED2_GPIO_Port GPIOB
#define USERLED_Pin GPIO_PIN_4
#define USERLED_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_5
#define BUZZER_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

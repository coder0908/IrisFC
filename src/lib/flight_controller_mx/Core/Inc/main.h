/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#define DEBUG_LED_Pin GPIO_PIN_13
#define DEBUG_LED_GPIO_Port GPIOC
#define RECEIVER_TX_Pin GPIO_PIN_2
#define RECEIVER_TX_GPIO_Port GPIOA
#define RECEIVER_RX_Pin GPIO_PIN_3
#define RECEIVER_RX_GPIO_Port GPIOA
#define EXBAT1_Pin GPIO_PIN_4
#define EXBAT1_GPIO_Port GPIOA
#define EXBAT2_Pin GPIO_PIN_7
#define EXBAT2_GPIO_Port GPIOA
#define SERVO_FLAP_R_Pin GPIO_PIN_0
#define SERVO_FLAP_R_GPIO_Port GPIOB
#define SERVO_AIL_L_Pin GPIO_PIN_1
#define SERVO_AIL_L_GPIO_Port GPIOB
#define ICM20602_0_READY_Pin GPIO_PIN_12
#define ICM20602_0_READY_GPIO_Port GPIOB
#define ICM20602_0_CS_Pin GPIO_PIN_13
#define ICM20602_0_CS_GPIO_Port GPIOB
#define BLDC_MOT_L_Pin GPIO_PIN_8
#define BLDC_MOT_L_GPIO_Port GPIOA
#define BLDC_MOT_R_Pin GPIO_PIN_9
#define BLDC_MOT_R_GPIO_Port GPIOA
#define TLM_RX_Pin GPIO_PIN_10
#define TLM_RX_GPIO_Port GPIOA
#define NEOM8N_TX_Pin GPIO_PIN_11
#define NEOM8N_TX_GPIO_Port GPIOA
#define NEOM8N_RX_Pin GPIO_PIN_12
#define NEOM8N_RX_GPIO_Port GPIOA
#define TLM_TX_Pin GPIO_PIN_15
#define TLM_TX_GPIO_Port GPIOA
#define SERVO_FLAP_L_Pin GPIO_PIN_3
#define SERVO_FLAP_L_GPIO_Port GPIOB
#define SERVO_LAN_Pin GPIO_PIN_4
#define SERVO_LAN_GPIO_Port GPIOB
#define SERVO_ELE_Pin GPIO_PIN_5
#define SERVO_ELE_GPIO_Port GPIOB
#define SERVO_RUD_Pin GPIO_PIN_6
#define SERVO_RUD_GPIO_Port GPIOB
#define SERVO_AIL_R_Pin GPIO_PIN_7
#define SERVO_AIL_R_GPIO_Port GPIOB
#define HMC5883L_SCL_Pin GPIO_PIN_8
#define HMC5883L_SCL_GPIO_Port GPIOB
#define HMC5883L_SDA_Pin GPIO_PIN_9
#define HMC5883L_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/*
 * platform_hal.h
 *
 *  Created on: Oct 8, 2025
 *      Author: coder0908
 */

#pragma once

#include <stdbool.h>
#include "stm32f4xx_hal.h"

#define DEBUG_LED_Pin GPIO_PIN_13
#define DEBUG_LED_GPIO_Port GPIOC
#define BNO08X_WAKE_Pin GPIO_PIN_1
#define BNO08X_WAKE_GPIO_Port GPIOA
#define RECEIVER_TX_Pin GPIO_PIN_2
#define RECEIVER_TX_GPIO_Port GPIOA
#define RECEIVER_RX_Pin GPIO_PIN_3
#define RECEIVER_RX_GPIO_Port GPIOA
#define EXBAT2_Pin GPIO_PIN_4
#define EXBAT2_GPIO_Port GPIOA
#define BNO08X_CS_Pin GPIO_PIN_5
#define BNO08X_CS_GPIO_Port GPIOA
#define BNO08X_INT_Pin GPIO_PIN_6
#define BNO08X_INT_GPIO_Port GPIOA
#define BNO08X_INT_EXTI_IRQn EXTI9_5_IRQn
#define EXBAT1_Pin GPIO_PIN_7
#define EXBAT1_GPIO_Port GPIOA
#define SERVO_FLAP_R_Pin GPIO_PIN_0
#define SERVO_FLAP_R_GPIO_Port GPIOB
#define SERVO_AIL_L_Pin GPIO_PIN_1
#define SERVO_AIL_L_GPIO_Port GPIOB
#define BNO08X_NRST_Pin GPIO_PIN_2
#define BNO08X_NRST_GPIO_Port GPIOB
#define ICM20602_INT_Pin GPIO_PIN_12
#define ICM20602_INT_GPIO_Port GPIOB
#define ICM20602_INT_EXTI_IRQn EXTI15_10_IRQn
#define ICM20602_CS_Pin GPIO_PIN_13
#define ICM20602_CS_GPIO_Port GPIOB
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
#define I2C_SCL_Pin GPIO_PIN_8
#define I2C_SCL_GPIO_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_9
#define I2C_SDA_GPIO_Port GPIOB

void platform_hal_init(void);
void Error_Handler(void);



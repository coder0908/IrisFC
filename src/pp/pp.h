/*
 * sys.h
 *
 *  Created on: Oct 8, 2025
 *      Author: unRR
 */

#ifndef __PP_H__
#define __PP_H__

#include "stm32f4xx_hal.h"	//always top
#include "gpio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "adc.h"
#include "i2c.h"
#include <stdbool.h>

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

void pp_init(void);
void Error_Handler(void);


#endif /* __PP_H__ */

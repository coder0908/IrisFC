/*
 * servo.h
 *
 *  Created on: Jun 28, 2025
 *      Author: mia00
 */

#ifndef __SERVO_DRIVER_H__
#define __SERVO_DRIVER_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

struct Servo {
	TIM_HandleTypeDef *htim;
	uint32_t channel;

	uint32_t minCcr;
	uint32_t maxCcr;
};

bool servo_init(struct Servo *servo, TIM_HandleTypeDef *htim, uint32_t channel, uint32_t minCcr, uint32_t maxCcr);
bool servo_write(struct Servo *servo, uint32_t between0To1800);

#endif /* __SERVO_DRIVER_H__ */

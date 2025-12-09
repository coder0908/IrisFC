/*
 * servo.h
 *
 *  Created on: Jun 28, 2025
 *      Author: coder0908
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

struct servo {
	TIM_HandleTypeDef *htim;
	uint32_t channel;

	uint32_t min_input;
	uint32_t max_input;

	uint32_t min_ccr;
	uint32_t max_ccr;
};

bool servo_init(struct servo *servo, TIM_HandleTypeDef *htim, uint32_t channel, uint32_t min_input, uint32_t max_input, uint32_t min_ccr, uint32_t max_ccr);
bool servo_write(struct servo *servo, uint32_t input);


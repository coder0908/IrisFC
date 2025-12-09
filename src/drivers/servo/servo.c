/*
 * servo.c
 *
 *  Created on: Jun 28, 2025
 *      Author: coder0908
 */

#include <assert.h>
#include "servo.h"

uint32_t map(uint32_t x, uint32_t min_in, uint32_t max_in, uint32_t min_out, uint32_t max_out)
{
  return (x - min_in) * (max_out - min_out) / (max_in - min_in) + min_out;
}

bool servo_init(struct servo *servo, TIM_HandleTypeDef *htim, uint32_t channel, uint32_t min_input, uint32_t max_input, uint32_t min_ccr, uint32_t max_ccr)
{
	assert(servo);
	assert(htim);

	servo->htim = htim;
	servo->channel = channel;

	servo->min_input = min_input;
	servo->max_input = max_input;

	servo->min_ccr = min_ccr;
	servo->max_ccr = max_ccr;

	HAL_StatusTypeDef status = HAL_TIM_PWM_Start(servo->htim, servo->channel);
	if (status != HAL_OK) {
		return false;
	}

	return true;
}

bool servo_write(struct servo *servo, uint32_t input)
{
	assert(servo);

	uint32_t ccr = map(input, servo->min_input, servo->max_input, servo->min_ccr, servo->max_ccr);

	__HAL_TIM_SET_COMPARE(servo->htim, servo->channel, ccr);

	return true;
}

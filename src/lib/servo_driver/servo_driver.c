/*
 * servo.c
 *
 *  Created on: Jun 28, 2025
 *      Author: mia00
 */


#include "servo_driver.h"
#include "vmd.h"

static uint32_t map(uint16_t x, uint32_t min_in, uint32_t max_in, uint32_t min_out, uint32_t max_out)
{
  return (x - min_in) * (max_out - min_out) / (max_in - min_in) + min_out;
}

bool servo_init(struct Servo *servo, TIM_HandleTypeDef *htim, uint32_t channel, uint32_t min_ccr, uint32_t max_ccr)
{
	VMD_ASSERT_PARAM(servo);
	VMD_ASSERT_PARAM(htim);

	servo->htim = htim;
	servo->channel = channel;

	servo->min_ccr = min_ccr;
	servo->max_ccr = max_ccr;

	HAL_StatusTypeDef spiStatus = HAL_TIM_PWM_Start(servo->htim, servo->channel);
	if (spiStatus != HAL_OK) {
		return false;
	}

	return true;
}

//bool servo_change_CCR(struct Servo *servo, uint32_t CCR_0Degree, uint32_t CCR_180Degree)
//{
//	VMD_ASSERT_PARAM(servo);
//	servo->CCR_0Degree = CCR_0Degree;
//	servo->CCR_180Degree = CCR_180Degree;
//
//	return true;
//}

bool servo_write(struct Servo *servo, uint32_t between0_to_1800)
{
	VMD_ASSERT_PARAM(servo);
	VMD_ASSERT_PARAM(between0_to_1800 <= 1800);
	VMD_ASSERT_PARAM(between0_to_1800 >= 0);

	uint32_t ccr = map(between0_to_1800, 0, 1800, servo->min_ccr, servo->max_ccr);

	__HAL_TIM_SET_COMPARE(servo->htim, servo->channel, ccr);

	return true;
}

/*
 * servo.c
 *
 *  Created on: Jun 28, 2025
 *      Author: mia00
 */


#include "servo_driver.h"
#include "vmd.h"

static uint32_t map(uint16_t x, uint32_t inMin, uint32_t inMax, uint32_t outMin, uint32_t outMax)
{
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

bool servo_init(struct Servo *servo, TIM_HandleTypeDef *htim, uint32_t channel, uint32_t minCcr, uint32_t maxCcr)
{
	VMD_ASSERT_PARAM(servo);
	VMD_ASSERT_PARAM(htim);

	servo->htim = htim;
	servo->channel = channel;

	servo->minCcr = minCcr;
	servo->maxCcr = maxCcr;

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

bool servo_write(struct Servo *servo, uint32_t between0To1800)
{
	VMD_ASSERT_PARAM(servo);
	VMD_ASSERT_PARAM(between0To1800 <= 1800);
	VMD_ASSERT_PARAM(between0To1800 >= 0);

	uint32_t ccr = map(between0To1800, 0, 1800, servo->minCcr, servo->maxCcr);

	__HAL_TIM_SET_COMPARE(servo->htim, servo->channel, ccr);

	return true;
}

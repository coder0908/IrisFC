/*
 * pwm_app.c
 *
 *  Created on: May 3, 2025
 *      Author: mia00
 */

#include "consur.h"
#include "pp.h"

struct Servo  bldc_left_mot, bldc_right_mot, servo_left_ail, servo_right_ail, servo_ele, servo_rud, servo_lan, servo_left_flap, servo_right_flap;

bool consur_init()
{
	bool ret = false;

	servo_init(&bldc_left_mot, &htim1, TIM_CHANNEL_1, 500, 1000);
	servo_init(&bldc_right_mot, &htim1, TIM_CHANNEL_2, 500, 1000);
	servo_init(&servo_left_ail, &htim4, TIM_CHANNEL_1, 250, 1250);
	servo_init(&servo_right_ail, &htim4, TIM_CHANNEL_2, 250, 1250);
	servo_init(&servo_ele, &htim3, TIM_CHANNEL_2, 250, 1250);
	servo_init(&servo_rud, &htim3, TIM_CHANNEL_1, 250, 1250);
	servo_init(&servo_lan, &htim2, TIM_CHANNEL_2, 250, 1250);
	servo_init(&servo_left_flap, &htim3, TIM_CHANNEL_4, 250, 1250);
	servo_init(&servo_right_flap, &htim3, TIM_CHANNEL_3, 250, 1250);

	ret = servo_write(&servo_left_ail, 900);
	if (!ret) {
		return false;
	}
	ret = servo_write(&servo_right_ail, 900);
	if (!ret) {
		return false;
	}
	ret = servo_write(&servo_ele, 900);
	if (!ret) {
		return false;
	}
	ret = servo_write(&servo_rud, 900);
	if (!ret) {
		return false;
	}
	ret = servo_write(&bldc_left_mot, 0);
	if (!ret) {
		return false;
	}
	ret = servo_write(&bldc_right_mot, 0);
	if (!ret) {
		return false;
	}
	ret = servo_write(&servo_lan, 900);
	if (!ret) {
		return false;
	}
	ret = servo_write(&servo_left_flap, 900);
	if (!ret) {
		return false;
	}
	ret = servo_write(&servo_right_flap, 900);
	if (!ret) {
		return false;
	}

	return ret;
}



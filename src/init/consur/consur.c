/*
 * pwm_app.c
 *
 *  Created on: May 3, 2025
 *      Author: mia00
 */

#include "consur.h"
#include "pp.h"

struct Servo  bldcLeftMot, bldcRightMot, servoLeftAil, servoRightAil, servoEle, servoRud, servoLan, servoLeftFlap, servoRightFlap;

bool consur_init()
{
	bool ret = false;

	servo_init(&bldcLeftMot, &htim1, TIM_CHANNEL_1, 500, 1000);
	servo_init(&bldcRightMot, &htim1, TIM_CHANNEL_2, 500, 1000);
	servo_init(&servoLeftAil, &htim4, TIM_CHANNEL_1, 250, 1250);
	servo_init(&servoRightAil, &htim4, TIM_CHANNEL_2, 250, 1250);
	servo_init(&servoEle, &htim3, TIM_CHANNEL_2, 250, 1250);
	servo_init(&servoRud, &htim3, TIM_CHANNEL_1, 250, 1250);
	servo_init(&servoLan, &htim2, TIM_CHANNEL_2, 250, 1250);
	servo_init(&servoLeftFlap, &htim3, TIM_CHANNEL_4, 250, 1250);
	servo_init(&servoRightFlap, &htim3, TIM_CHANNEL_3, 250, 1250);

	ret = servo_write(&servoLeftAil, 900);
	if (!ret) {
		return false;
	}
	ret = servo_write(&servoRightAil, 900);
	if (!ret) {
		return false;
	}
	ret = servo_write(&servoEle, 900);
	if (!ret) {
		return false;
	}
	ret = servo_write(&servoRud, 900);
	if (!ret) {
		return false;
	}
	ret = servo_write(&bldcLeftMot, 0);
	if (!ret) {
		return false;
	}
	ret = servo_write(&bldcRightMot, 0);
	if (!ret) {
		return false;
	}
	ret = servo_write(&servoLan, 900);
	if (!ret) {
		return false;
	}
	ret = servo_write(&servoLeftFlap, 900);
	if (!ret) {
		return false;
	}
	ret = servo_write(&servoRightFlap, 900);
	if (!ret) {
		return false;
	}

	return ret;
}



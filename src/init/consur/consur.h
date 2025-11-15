/*
 * consur.h
 *
 *  Created on: May 5, 2025
 *      Author: mia00
 */

#ifndef __CONSUR_H__
#define __CONSUR_H__

#include <stdint.h>
#include "servo_driver.h"

extern struct Servo  bldc_left_mot, bldc_right_mot, servo_left_ail, servo_right_ail, servo_ele, servo_rud, servo_lan, servo_left_flap, servo_right_flap;

bool consur_init();

#endif /* __CONSUR_H__ */

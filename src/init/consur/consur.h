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

extern struct Servo  bldcLeftMot, bldcRightMot, servoLeftAil, servoRightAil, servoEle, servoRud, servoLan, servoLeftFlap, servoRightFlap;

bool consur_init();

#endif /* __CONSUR_H__ */

/*
 * attitude.c
 *
 *  Created on: Nov 19, 2025
 *      Author: coder0908
 */



#include "platform/hal/platform_hal.h"
#include "lib/pid/pid.h"
#include "attitude.h"

static struct cascade_pid s_roll_cpid;
static struct cascade_pid s_pitch_cpid;

bool attitude_init()
{
	if (!cascade_pid_init(&s_roll_cpid, 6, 0.00005, 0.0005, 0, -30, 30, 0.7, 0.00005, 0.0001, 0, -30, 30)) {
		return false;
	}
	if (!cascade_pid_init(&s_pitch_cpid, 6, 0.00005, 0.0005, 0, -30, 30, 0.7, 0.00005, 0.0001, 0, -30, 30)) {
		return false;
	}

	return true;

}

void attitude_loop()
{

}

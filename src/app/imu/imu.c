/*
 * imu.c
 *
 *  Created on: 2025. 12. 11.
 *      Author: coder0908
 */

#include "icm20602/icm20602.h"
#include "imu.h"


bool imu_init()
{
	bool tmp = false;
	tmp = app_icm20602_init();
	if (!tmp) {
		return false;
	}

	return true;
}

void imu_loop()
{
	app_icm20602_loop();
}

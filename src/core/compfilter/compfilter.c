/*
 * compfilter.c
 *
 *  Created on: 2025. 12. 6.
 *      Author: coder0908
 */

#include <assert.h>
#include <math.h>
#include "stm32f4xx_hal.h"
#include "compfilter.h"
#include "utils/utils.h"

bool compfilter_calc_angle(struct compfilter *cmpf, float x_accel_g, float y_accel_g, float z_accel_g, float x_gyro_dps, float y_gyro_dps, float z_gyro_dps)
{
	assert(cmpf);

	cmpf->x_gyro_dps = x_gyro_dps;
	cmpf->y_gyro_dps = y_gyro_dps;
	cmpf->z_gyro_dps = z_gyro_dps;


	uint32_t current_time_ms = HAL_GetTick();
	if (current_time_ms == cmpf->prev_time_ms) {
		return false;
	}
	float dt_s = ((float)current_time_ms - (float)cmpf->prev_time_ms) / 1000.0f;

	float add_x_angle_by_gyro_deg = x_gyro_dps * dt_s;	//현재 루프에서 추가된 각도
	float add_y_angle_by_gyro_deg = y_gyro_dps * dt_s;	//현재 루프에서 추가된 각도
	float add_z_angle_by_gyro_deg = z_gyro_dps * dt_s;	//현재 루프에서 추가된 각도


	cmpf->x_angle_deg -= add_y_angle_by_gyro_deg * sinf(DEG_TO_RAD(add_z_angle_by_gyro_deg));
	cmpf->y_angle_deg += add_x_angle_by_gyro_deg * sinf(DEG_TO_RAD(add_z_angle_by_gyro_deg));
	cmpf->z_angle_deg += add_z_angle_by_gyro_deg;

//	cmpf->total_accel_vevtor = sqrtf((x_accel_g*x_accel_g) + (y_accel_g*y_accel_g) + (z_accel_g* z_accel_g));

	float x_angle_by_accel_deg = atan2f(y_accel_g, sqrtf(x_accel_g*x_accel_g + z_accel_g*z_accel_g)) * RADIAN;
	float y_angle_by_accel_deg = atan2f(-x_accel_g, sqrtf(y_accel_g*y_accel_g + z_accel_g*z_accel_g)) * RADIAN;

	float alpha = cmpf->alpha;

	cmpf->x_angle_deg = (1 - alpha) * x_angle_by_accel_deg + alpha * (cmpf->x_angle_deg + add_x_angle_by_gyro_deg);
	cmpf->y_angle_deg = (1 - alpha) * y_angle_by_accel_deg + alpha * (cmpf->y_angle_deg + add_y_angle_by_gyro_deg);

	if (cmpf->x_angle_deg >= 180) {
		cmpf->x_angle_deg -= 360;
	} else if (cmpf->x_angle_deg < -180) {
		cmpf->x_angle_deg += 360;
	}

	if (cmpf->y_angle_deg >= 180) {
		cmpf->y_angle_deg -= 360;
	} else if (cmpf->y_angle_deg < -180) {
		cmpf->y_angle_deg += 360;
	}

	if (cmpf->z_angle_deg >= 180) {
		cmpf->z_angle_deg -= 360;
	} else if (cmpf->z_angle_deg < -180) {
		cmpf->z_angle_deg += 360;
	}

	cmpf->prev_time_ms = HAL_GetTick();

	return true;
}

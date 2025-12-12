/*
 * compfilter.h
 *
 *  Created on: 2025. 12. 6.
 *      Author: coder0908
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>


struct compfilter {
	float alpha;

	uint32_t prev_time_ms;

	float x_angle_deg;
	float y_angle_deg;
	float z_angle_deg;

	float x_gyro_dps;
	float y_gyro_dps;
	float z_gyro_dps;
};

bool compfilter_calc_angle(struct compfilter *cmpf, float x_accel_g, float y_accel_g, float z_accel_g, float x_gyro_dps, float y_gyro_dps, float z_gyro_dps);

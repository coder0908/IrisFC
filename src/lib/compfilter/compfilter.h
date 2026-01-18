#ifndef COMPFILTER_H_
#define COMPFILTER_H_

#include <stdbool.h>
#include <stdint.h>

struct compfilter {
	float beta;
	uint32_t prev_time_ms;

	float pitch_deg; // X축 방향 기울기 (사용자 정의)
	float roll_deg;  // Y축 방향 기울기 (사용자 정의)
	float yaw_deg;

	float q0, q1, q2, q3;
};

void compfilter_init(struct compfilter *cmpf, float beta);
bool compfilter_calc_angle(struct compfilter *cmpf, float x_accel_g, float y_accel_g, float z_accel_g, float x_gyro_dps, float y_gyro_dps, float z_gyro_dps);

#endif

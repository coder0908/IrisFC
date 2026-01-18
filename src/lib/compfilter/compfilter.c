#include <assert.h>
#include <math.h>
#include "compfilter.h"
#include "platform/hal/platform_hal.h"

#define DEG_TO_RAD (3.1415926535f / 180.0f)
#define RAD_TO_DEG (180.0f / 3.1415926535f)

void compfilter_init(struct compfilter *cmpf, float beta) {
	cmpf->beta = beta;
	cmpf->q0 = 1.0f; cmpf->q1 = 0.0f; cmpf->q2 = 0.0f; cmpf->q3 = 0.0f;
	cmpf->prev_time_ms = 0;
}

bool compfilter_calc_angle(struct compfilter *cmpf, float ax, float ay, float az, float gx, float gy, float gz) {
	uint32_t now = HAL_GetTick();
	if (cmpf->prev_time_ms == 0) {
		cmpf->prev_time_ms = now;
		return false;
	}
	float dt = (float)(now - cmpf->prev_time_ms) / 1000.0f;
	if (dt <= 0.0f) return false;
	cmpf->prev_time_ms = now;

	// 1. 단위 변환 및 정규화
	gx *= DEG_TO_RAD;
	gy *= DEG_TO_RAD;
	gz *= DEG_TO_RAD;

	float norm = sqrtf(ax*ax + ay*ay + az*az);
	if (norm < 0.0001f) return false; // Zero-g 방지
	ax /= norm; ay /= norm; az /= norm;

	// [참고] 필요하다면 여기서 축을 바꿉니다.
	// 예: float tx = ax; ax = ay; ay = tx;

	// 2. Madgwick 알고리즘 (경사 하강법 단계)
	float q0 = cmpf->q0, q1 = cmpf->q1, q2 = cmpf->q2, q3 = cmpf->q3;
	float _2q0 = 2.0f * q0;
	float _2q1 = 2.0f * q1;
	float _2q2 = 2.0f * q2;
	float _2q3 = 2.0f * q3;
	//    float _4q0 = 4.0f * q0;
	float _4q1 = 4.0f * q1;
	float _4q2 = 4.0f * q2;
	//    float _8q1 = 8.0f * q1;
	//    float _8q2 = 8.0f * q2;
	//    float q0q0 = q0 * q0;
	//    float q1q1 = q1 * q1;
	//    float q2q2 = q2 * q2;
	//    float q3q3 = q3 * q3;

	// Gradient objective function: f(q, a) = q*g*q' - a
	float f1 = _2q1 * q3 - _2q0 * q2 - ax;
	float f2 = _2q0 * q1 + _2q2 * q3 - ay;
	float f3 = 1.0f - _2q1 * q1 - _2q2 * q2 - az;

	// Jacobian: J^T * f
	float s0 = -_2q2 * f1 + _2q1 * f2;
	float s1 =  _2q3 * f1 + _2q0 * f2 - _4q1 * f3;
	float s2 = -_2q0 * f1 + _2q3 * f2 - _4q2 * f3;
	float s3 =  _2q1 * f1 + _2q2 * f2;

	// Gradient 정규화
	norm = sqrtf(s0*s0 + s1*s1 + s2*s2 + s3*s3);
	if (norm > 0.0001f) {
		s0 /= norm; s1 /= norm; s2 /= norm; s3 /= norm;
	}

	// 자이로스코프에 의한 변화량 반영 + Beta(가속도계) 보정
	float qDot0 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz) - cmpf->beta * s0;
	float qDot1 = 0.5f * ( q0 * gx + q2 * gz - q3 * gy) - cmpf->beta * s1;
	float qDot2 = 0.5f * ( q0 * gy - q1 * gz + q3 * gx) - cmpf->beta * s2;
	float qDot3 = 0.5f * ( q0 * gz + q1 * gy - q2 * gx) - cmpf->beta * s3;

	// 적분 및 정규화
	cmpf->q0 += qDot0 * dt;
	cmpf->q1 += qDot1 * dt;
	cmpf->q2 += qDot2 * dt;
	cmpf->q3 += qDot3 * dt;

	norm = sqrtf(cmpf->q0*cmpf->q0 + cmpf->q1*cmpf->q1 + cmpf->q2*cmpf->q2 + cmpf->q3*cmpf->q3);
	cmpf->q0 /= norm; cmpf->q1 /= norm; cmpf->q2 /= norm; cmpf->q3 /= norm;

	// 3. 오일러 각도 추출 (표준 NED 좌표계 기준)
	q0 = cmpf->q0; q1 = cmpf->q1; q2 = cmpf->q2; q3 = cmpf->q3;

	// Roll (x-axis rotation)
	cmpf->roll_deg = atan2f(2.0f * (q0 * q1 + q2 * q3), 1.0f - 2.0f * (q1 * q1 + q2 * q2)) * RAD_TO_DEG;

	// Pitch (y-axis rotation)
	float sinp = 2.0f * (q0 * q2 - q3 * q1);
	if (fabsf(sinp) >= 1.0f)
		cmpf->pitch_deg = copysignf(90.0f, sinp);
	else
		cmpf->pitch_deg = asinf(sinp) * RAD_TO_DEG;

	// Yaw (z-axis rotation)
	cmpf->yaw_deg = atan2f(2.0f * (q0 * q3 + q1 * q2), 1.0f - 2.0f * (q2 * q2 + q3 * q3)) * RAD_TO_DEG;

	return true;
}

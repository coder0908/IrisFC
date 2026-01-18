/*
 * pid.c
 *
 *  Created on: Nov 19, 2025
 *      Author: coder0908
 */

#include <assert.h>
#include <stdio.h>
#include "platform/hal/platform_hal.h"
#include "pid.h"



bool single_pid_init(struct single_pid *spid, float kp, float ki, float kd, float d_filter_gain, float error_sum_min, float error_sum_max)
{
	assert(spid);

	spid->kp = kp;
	spid->ki = ki;
	spid->kd = kd;
	spid->d_filter_gain = d_filter_gain;
	spid->error_sum_max = error_sum_max;
	spid->error_sum_min = error_sum_min;
	spid->error_sum = 0;
	spid->prev_time = HAL_GetTick();
	spid->prev_state = 0;

	spid->pid_result = 0;

	return true;
}

bool cascade_pid_init(struct cascade_pid *cpid, float outter_kp, float outter_ki, float outter_kd, float outter_d_filter_gain, float outter_error_sum_min, float outter_error_sum_max,
		float inner_kp, float inner_ki, float inner_kd, float inner_d_filter_gain, float inner_error_sum_min, float inner_error_sum_max)
{
	assert(cpid);

	bool tmp;
	tmp = single_pid_init(&(cpid->outter), outter_kp, outter_ki, outter_kd, outter_d_filter_gain, outter_error_sum_min, outter_error_sum_max);
	if (!tmp) {
		return false;
	}
	tmp = single_pid_init(&(cpid->inner), inner_kp, inner_ki, inner_kd, inner_d_filter_gain, inner_error_sum_min, inner_error_sum_max);
	if (!tmp) {
		return false;
	}

	return true;
}

void single_pid_reset_error_sum(struct single_pid *spid)
{
	assert(spid);

	spid->error_sum = 0;
}

bool single_pid_calc(struct single_pid *spid, float set_state, float current_state)
{
	assert(spid);

	float current_error = set_state - current_state;

	spid->p_result = current_error * spid->kp;

	uint32_t current_time = HAL_GetTick();
	if (current_time == spid->prev_time) {
		return true;
	}

	float dt_s = (float)(current_time - spid->prev_time) / 1000.0;
	spid->prev_time = current_time;
	spid->error_sum += current_error * dt_s;
	spid->error_sum = spid->error_sum < spid->error_sum_min ? spid->error_sum_min : spid->error_sum;
	spid->error_sum = spid->error_sum > spid->error_sum_max ? spid->error_sum_max : spid->error_sum;
	spid->i_result = spid->error_sum * spid->ki;

	float d_delta = -(current_state - spid->prev_state) / dt_s;
	spid->prev_state = current_state;
	spid->d_result = spid->d_result * spid->d_filter_gain + d_delta * spid->kd * (1-spid->d_filter_gain);

	spid->pid_result = spid->p_result + spid->i_result + spid->d_result;


	return true;
}

bool cascade_pid_calc(struct cascade_pid *cpid, float set_state/*ouuter*/, float outter_current_state, float inner_current_state)
{
	assert(cpid);

	bool tmp;

	tmp = single_pid_calc(&(cpid->outter), set_state, outter_current_state);
	if (!tmp) {
		return false;
	}

	tmp = single_pid_calc(&(cpid->inner), cpid->outter.pid_result, inner_current_state);
	if (!tmp) {
		return false;
	}

	return true;

}



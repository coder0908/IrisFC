/*
 * utils.h
 *
 *  Created on: 2025. 12. 4.
 *      Author: coder0908
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define RADIAN (180.0f/M_PI)

static inline float DEG_TO_RAD(float degree)
{
	return degree / RADIAN;
}

static inline float RAD_TO_DEG(float radian)
{
	return radian * RADIAN;
}


char* strcpy_n(char *dest, const char *src, uint32_t len);
bool get_bit_value(const uint8_t value[], uint32_t value_len, int trgt_bit_idx);
void set_bit_value(uint8_t value[], uint32_t value_len, int trgt_bit_idx, bool en);

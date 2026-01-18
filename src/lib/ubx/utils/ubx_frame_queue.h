/*
 * ubx_frame_queue.h
 *
 *  Created on: 2025. 12. 10.
 *      Author: coder0908
 */

#pragma once

#include "../ubx.h"

#define UBX_FRAMESLEN_MAX	2

struct ubx_frame_queue {
	struct ubx_frame frames[UBX_FRAMESLEN_MAX];
	uint8_t head;
	uint8_t len;
};



bool ubx_init_frame_queue(struct ubx_frame_queue *queue);
bool ubx_push_frame_queue(struct ubx_frame_queue *queue, const struct ubx_frame *frame);
bool ubx_pop_frame_queue(struct ubx_frame_queue *queue, struct ubx_frame *frame);
void ubx_parse_frames(struct ubx_frame_queue *queue, uint8_t *buf, uint16_t buf_len, uint16_t *read_len);

/*
 * crsf_frame_queue.h
 *
 *  Created on: 2025. 12. 6.
 *      Author: coder0908
 */

#pragma once

#include "../crsf.h"

//crsf driver settings
#define CRSF_FRAMESLEN_MAX 50




struct crsf_frame_queue {
	struct crsf_frame frames[CRSF_FRAMESLEN_MAX];
	uint64_t head;
	uint64_t cnt;
};


bool crsf_init_frame_queue(struct crsf_frame_queue *queue);
bool crsf_push_frame_queue(struct crsf_frame_queue *queue, const struct crsf_frame *frame);
bool crsf_pop_frame_queue(struct crsf_frame_queue *queue, struct crsf_frame *frame);

bool crsf_clear_frmae_queue(struct crsf_frame_queue *queue);
void crsf_parse_frames(struct crsf_frame_queue *queue, const uint8_t buf[],
                       uint64_t buf_len, uint64_t *read_len);

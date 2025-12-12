/*
 * crsf_frame_queue.c
 *
 *  Created on: 2025. 12. 6.
 *      Author: coder0908
 */

#include <assert.h>
#include "crsf_frame_queue.h"




bool crsf_init_frame_queue(struct crsf_frame_queue *queue)
{
	assert(queue);

	queue->len = 0;
	queue->head = 0;

	return true;
}

bool crsf_push_frame_queue(struct crsf_frame_queue *queue, const struct crsf_frame *frame)
{
	assert(queue);
	assert(queue->len <= CRSF_FRAMESLEN_MAX);

	if (queue->len == CRSF_FRAMESLEN_MAX) {
		return false;
	}

	struct crsf_frame *const dest = queue->frames + (queue->head + queue->len) % CRSF_FRAMESLEN_MAX;
	*dest = *frame;
	queue->len += 1;

	return true;
}

bool crsf_pop_frame_queue(struct crsf_frame_queue *queue, struct crsf_frame *frame)
{
	assert(queue);
	assert(frame);

	if (queue->len == 0) {
		return false;
	}

	struct crsf_frame *src = &(queue->frames[queue->head]);
	*frame = *src;
	queue->len -= 1;
	queue->head = (queue->head + 1) % CRSF_FRAMESLEN_MAX;

	return true;
}


bool crsf_clear_frmae_queue(struct crsf_frame_queue *queue)
{
	assert(queue);

	queue->len = 0;

	return true;
}

void crsf_parse_frames(struct crsf_frame_queue *queue, const uint8_t *buf, uint8_t buf_len, uint8_t *read_len)
{
	assert(queue);
	assert(buf);
	assert(read_len);

	struct crsf_frame frame;
	uint8_t tmp_read_len;
	uint8_t total_read_len;
	bool is_parse_success;

	for (total_read_len=0; total_read_len<buf_len; /*intentionally do nothing*/) {
		is_parse_success = crsf_parse_frame(&frame, buf + total_read_len, buf_len - total_read_len, &tmp_read_len);

		total_read_len += tmp_read_len;
		if (is_parse_success) {
			if (!crsf_push_frame_queue(queue, &frame)) {
				break;
			}
		}
	}

	*read_len = total_read_len;
}




















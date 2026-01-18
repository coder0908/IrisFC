/*
 * ubx_frame_queue.c
 *
 *  Created on: 2025. 12. 10.
 *      Author: coder0908
 */

#include "ubx_frame_queue.h"


bool ubx_init_frame_queue(struct ubx_frame_queue *queue)
{
	assert(queue);

	queue->head = 0;
	queue->len = 0;

	return true;
}

bool ubx_push_frame_queue(struct ubx_frame_queue *queue, const struct ubx_frame *frame)
{
	assert(queue);
	assert(queue->len <= UBX_FRAMESLEN_MAX);

	if (queue->len == UBX_FRAMESLEN_MAX) {
		return false;
	}

	struct ubx_frame *const dest = queue->frames + (queue->head + queue->len) % UBX_FRAMESLEN_MAX;
	*dest = *frame;
	queue->len += 1;

	return true;
}

bool ubx_pop_frame_queue(struct ubx_frame_queue *queue, struct ubx_frame *frame)
{
	assert(queue);
	assert(frame);

	if (queue->len == 0) {
		return false;
	}

	struct ubx_frame *src = &(queue->frames[queue->head]);
	*frame = *src;
	queue->len -= 1;
	queue->head = (queue->head + 1) % UBX_FRAMESLEN_MAX;

	return true;
}


bool ubx_clear_frame_queue(struct ubx_frame_queue *queue)
{
	assert(queue);

	queue->len = 0;

	return true;
}

void ubx_parse_frames(struct ubx_frame_queue *queue, uint8_t *buf, uint16_t buf_len, uint16_t *read_len)
{
	assert(queue);
	assert(buf);
	assert(read_len);

	struct ubx_frame frame;
	uint16_t tmp_read_len;
	uint16_t total_read_len;
	bool is_parse_success;

	for (total_read_len=0; total_read_len<buf_len; /*intentionally do nothing*/) {
		is_parse_success = ubx_parse_frame(&frame, buf + total_read_len, buf_len - total_read_len, &tmp_read_len);

		total_read_len += tmp_read_len;
		if (is_parse_success) {
			if (!ubx_push_frame_queue(queue, &frame)) {
				break;
			}
		}
	}
	*read_len = total_read_len;
}

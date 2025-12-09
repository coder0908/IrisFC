/*
 * msgbox.c
 *
 *  Created on: 2025. 12. 4.
 *      Author: coder0908
 */


#include <string.h>
#include <assert.h>
#include "msgbox.h"
#include "utils/utils.h"

#define MSGBOX_CNT_MAX 50
#define MSGBOX_SUBCNT_MAX 20
#define MSGBOX_NAMELEN_MAX 20
#define MSGBOX_LEN_IDS ((MSGBOX_CNT_MAX/8) + (MSGBOX_CNT_MAX%8)?1:0)
#define MSGBOX_ID_INVALID -1

struct msgbox {
//	uint32_t id;
	bool using;
	char name[MSGBOX_NAMELEN_MAX];
	uint8_t name_len;

	uint8_t subs_cnt;	// sub가 빠져나간다면, 앞에서부터 꽉차게 재정렬 필요
	msgbox_callback_fp callbacks[MSGBOX_SUBCNT_MAX];

};


static struct msgbox s_msgboxes[MSGBOX_CNT_MAX];
static int s_msgboxes_len = 0;

static uint32_t msgbox_hash(uint32_t hash, char val)
{
	return (hash*31 + val) % MSGBOX_CNT_MAX;
}

static uint32_t msgbox_hash_name(const char msgbox_name[], uint8_t msgbox_name_len)
{
	assert(msgbox_name);
	assert(msgbox_name_len <= MSGBOX_NAMELEN_MAX);

	uint32_t msgbox_id = 0xD2;
	for (uint8_t i=0; i<msgbox_name_len; i++) {
		msgbox_id = msgbox_hash(msgbox_id, msgbox_name[i]);
	}

	return msgbox_id;
}

static bool msgbox_find_id(const char msgbox_name[], uint8_t msgbox_name_len, uint32_t *msgbox_id)
{
	assert(msgbox_name);
	assert(msgbox_name_len <= MSGBOX_NAMELEN_MAX);
	assert(msgbox_id);

	if (s_msgboxes_len == 0) {
		return false;
	}

	int tmp_msgbox_id = msgbox_hash_name(msgbox_name, msgbox_name_len);

	struct msgbox *mb = NULL;
	for (uint32_t i=0; i<MSGBOX_CNT_MAX; i++) {
		mb = &s_msgboxes[tmp_msgbox_id];
		if (mb->using &&
			mb->name_len == msgbox_name_len &&
			strncmp(msgbox_name, mb->name, msgbox_name_len) == 0) {
			*msgbox_id = tmp_msgbox_id;
			return true;
		}
		tmp_msgbox_id = (tmp_msgbox_id+1) % MSGBOX_CNT_MAX;
	}
	return false;
}

static bool msgbox_find_new_id(const char msgbox_name[], uint8_t msgbox_name_len, uint32_t *msgbox_id)
{

	assert(msgbox_id);

	if (s_msgboxes_len == MSGBOX_CNT_MAX) {
		return false;
	}

	int tmp_msgbox_id = msgbox_hash_name(msgbox_name, msgbox_name_len);

	for (uint32_t i=0; i<MSGBOX_CNT_MAX; i++) {
		if (!s_msgboxes[tmp_msgbox_id].using) {
			*msgbox_id = tmp_msgbox_id;
			return true;
		}
		tmp_msgbox_id = (tmp_msgbox_id+1) % MSGBOX_CNT_MAX;
	}
	return false;

}

static bool msgbox_create(const char msgbox_name[], uint8_t msgbox_name_len, uint32_t *msgbox_id)
{
	assert(msgbox_name);
	assert(msgbox_name_len<=MSGBOX_NAMELEN_MAX);
	assert(msgbox_id);

	if (!msgbox_find_new_id(msgbox_name, msgbox_name_len, msgbox_id)) {
		return false;
	}
	struct msgbox *mb = &s_msgboxes[*msgbox_id];

	strcpy_n(mb->name, msgbox_name, msgbox_name_len);
	mb->name_len = msgbox_name_len;
	mb->subs_cnt = 0;
	mb->using = true;
	for (uint8_t i=0; i<MSGBOX_SUBCNT_MAX; i++) {
		mb->callbacks[i] = NULL;
	}
	s_msgboxes_len += 1;

	return true;
}

static void msgbox_destroy(int msgbox_id)
{
	assert(msgbox_id < MSGBOX_CNT_MAX);

	struct msgbox *mb = &s_msgboxes[msgbox_id];
	assert(mb->using);


	for (uint8_t i=0; i<MSGBOX_SUBCNT_MAX; i++) {
		mb->callbacks[i] = NULL;
	}

	mb->subs_cnt = 0;
	mb->name_len = 0;
	mb->using = false;

	s_msgboxes_len -= 1;
}

bool msgbox_get(const char msgbox_name[], uint8_t msgbox_name_len, uint32_t *msgbox_id)
{
	if (msgbox_find_id(msgbox_name, msgbox_name_len, msgbox_id)) {
		return true;
	}
	return msgbox_create(msgbox_name, msgbox_name_len, msgbox_id);
}

void msgbox_release(uint32_t msgbox_id)
{
	msgbox_destroy(msgbox_id);
}

bool msgbox_publish(uint32_t msgbox_id, uint8_t msg[], uint8_t msg_len)
{
	assert(msgbox_id < MSGBOX_CNT_MAX);
	assert(msg);

	if (!msg_len) {
		return true;
	}

	struct msgbox *mb = &s_msgboxes[msgbox_id];
	if (!mb->using) {
		return false;
	}
	if (mb->subs_cnt == 0) {
		return true;
	}

	msgbox_callback_fp *callbacks = mb->callbacks;
	for (uint8_t i=0; i<mb->subs_cnt; i++) {
		callbacks[i](msg, msg_len);
	}

	return true;
}

bool msgbox_subscribe(uint32_t msgbox_id, msgbox_callback_fp callback)
{
	assert(msgbox_id < MSGBOX_CNT_MAX);
	assert(callback);

	struct msgbox *mb = &s_msgboxes[msgbox_id];

	if (!mb->using) {
		return false;
	}

	if (mb->subs_cnt == MSGBOX_SUBCNT_MAX) {
		return false;
	}

	mb->callbacks[mb->subs_cnt] = callback;
	mb->subs_cnt++;

	return true;
}

bool msgbox_unsubscribe(uint32_t msgbox_id, msgbox_callback_fp callback)
{
	assert(msgbox_id < MSGBOX_CNT_MAX);
	assert(callback);

	struct msgbox *mb = &s_msgboxes[msgbox_id];
	if (!mb->using) {
		return false;
	}

	if (mb->subs_cnt == 0) {
		return false;
	}

	for (uint8_t i=0; i<mb->subs_cnt; i++) {
		if (mb->callbacks[i] != callback) {
			continue;
		}
		mb->callbacks[i] = mb->callbacks[mb->subs_cnt-1];
		mb->callbacks[mb->subs_cnt-1] = NULL;
		mb->subs_cnt -= 1;

		return true;
	}

	return false;

}







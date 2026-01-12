/*
 * consur.c
 *
 *  Created on: May 3, 2025
 *      Author: coder0908
 */

#include <stdio.h>
#include "platform/hal/platform_hal.h"
#include "tim.h"
#include "drivers/servo/servo.h"
#include "lib/protocols/crsf/utils/crsf_frame_queue.h"
#include "middleware/msgbox/msgbox.h"
#include "app/hardware/tranceiver/tranceiver.h"
#include "consur.h"

static struct servo  s_bldc_left_mot, s_bldc_right_mot, s_servo_left_ail, s_servo_right_ail, s_servo_ele, s_servo_rud, s_servo_lan, s_servo_left_flap, s_servo_right_flap;
static uint32_t s_rx_crsf_msgbox_id;
static struct crsf_frame_queue s_crsf_frame_queue;
static bool s_is_frame_updated = false;
static void crsf_rx_msgbox_callback(uint8_t msg[], uint8_t msg_len);

bool consur_init()
{
	bool ret = false;

	servo_init(&s_bldc_left_mot, &htim1, TIM_CHANNEL_1, 172, 1811, 500, 1000);
	servo_init(&s_bldc_right_mot, &htim1, TIM_CHANNEL_2, 172, 1811, 500, 1000);
	servo_init(&s_servo_left_ail, &htim3, TIM_CHANNEL_4, 172, 1811, 500, 1000);
	servo_init(&s_servo_right_ail, &htim4, TIM_CHANNEL_2, 172, 1811, 500, 1000);
	servo_init(&s_servo_ele, &htim3, TIM_CHANNEL_2, 172, 1811, 250, 1250);
	servo_init(&s_servo_rud, &htim4, TIM_CHANNEL_1, 172, 1811, 250, 1250);
	servo_init(&s_servo_lan, &htim3, TIM_CHANNEL_1, 172, 1811, 500, 1000);
	servo_init(&s_servo_left_flap, &htim2, TIM_CHANNEL_2, 172, 1811, 250, 1250);
	servo_init(&s_servo_right_flap, &htim3, TIM_CHANNEL_3, 172, 1811, 250, 1250);

	ret = servo_write(&s_servo_left_ail, 922);
	if (!ret) {
		return false;
	}
	ret = servo_write(&s_servo_right_ail, 922);
	if (!ret) {
		return false;
	}
	ret = servo_write(&s_servo_ele, 922);
	if (!ret) {
		return false;
	}
	ret = servo_write(&s_servo_rud, 992);
	if (!ret) {
		return false;
	}
	ret = servo_write(&s_bldc_left_mot, 174);
	if (!ret) {
		return false;
	}
	ret = servo_write(&s_bldc_right_mot, 174);
	if (!ret) {
		return false;
	}
	ret = servo_write(&s_servo_lan, 922);
	if (!ret) {
		return false;
	}
	ret = servo_write(&s_servo_left_flap, 922);
	if (!ret) {
		return false;
	}
	ret = servo_write(&s_servo_right_flap, 922);
	if (!ret) {
		return false;
	}

	ret = crsf_init_frame_queue(&s_crsf_frame_queue);
	if (!ret) {
		return false;
	}

	ret = msgbox_get(TRCIVR_MSGBOX_NAME_CRSF_RX, TRCIVR_MSGBOX_NAMELEN_CRSF_RX, &s_rx_crsf_msgbox_id);
	if (!ret) {
		return false;
	}

	ret = msgbox_subscribe(s_rx_crsf_msgbox_id, crsf_rx_msgbox_callback);
	if (!ret) {
		return false;
	}

	return ret;

}

// msgbox_callback_fp
static void crsf_rx_msgbox_callback(uint8_t msg[], uint8_t msg_len)
{
	uint64_t read_len = 0;
	crsf_parse_frames(&s_crsf_frame_queue, msg, msg_len, &read_len);
	s_is_frame_updated = true;

}

void consur_loop()
{
	if (!s_is_frame_updated) {
		return;
	}

	s_is_frame_updated = false;

	struct crsf_frame frame = {0,};
	struct crsf_rc_channels rc_channels = {0,};

	while (crsf_pop_frame_queue(&s_crsf_frame_queue, &frame)) {

		if (!crsf_parse_rc_channels(&frame, &rc_channels)) {
			continue;
		}

		servo_write(&s_bldc_left_mot, rc_channels.chan1);
		servo_write(&s_bldc_right_mot, rc_channels.chan1);

		servo_write(&s_servo_ele, 1811 - rc_channels.chan3 + 172);
		servo_write(&s_servo_left_ail, 1811 - rc_channels.chan2 +172 + 65);
		servo_write(&s_servo_right_ail, 1811-rc_channels.chan2+172+150);

		servo_write(&s_servo_rud, rc_channels.chan4);

		if (rc_channels.chan5 == 191) {
			servo_write(&s_servo_lan, rc_channels.chan4);
		} else if (rc_channels.chan5 == 1792) {
			servo_write(&s_servo_lan, 922);
		}

	}

}





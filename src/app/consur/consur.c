/*
 * consur.c
 *
 *  Created on: May 3, 2025
 *      Author: coder0908
 */

#include "platform/hal/platform_hal.h"
#include "tim.h"
#include "drivers/servo/servo.h"
#include "protocols/crsf/utils/crsf_frame_queue.h"
#include "services/msgbox/msgbox.h"
#include "app/tranceiver/tranceiver.h"
#include "consur.h"

static struct servo  s_bldc_left_mot, s_bldc_right_mot, s_servo_left_ail, s_servo_right_ail, s_servo_ele, s_servo_rud, s_servo_lan, s_servo_left_flap, s_servo_right_flap;
static uint32_t s_trcivr_rx_crsf_msgbox_id;
static struct crsf_frame_queue s_consur_crsf_frame_queue;
static bool s_is_frame_updated = false;
static void consur_msgbox_rx_crsf_callback(uint8_t msg[], uint8_t msg_len);

bool consur_init()
{
	bool tmp = false;

	servo_init(&s_bldc_left_mot, &htim1, TIM_CHANNEL_1, 174, 1811, 500, 1000);
	servo_init(&s_bldc_right_mot, &htim1, TIM_CHANNEL_2, 174, 1811, 500, 1000);
	servo_init(&s_servo_left_ail, &htim3, TIM_CHANNEL_4, 174, 1811, 500, 1000);
	servo_init(&s_servo_right_ail, &htim4, TIM_CHANNEL_2, 174, 1811, 500, 1000);
	servo_init(&s_servo_ele, &htim3, TIM_CHANNEL_2, 174, 1811, 250, 1250);
	servo_init(&s_servo_rud, &htim4, TIM_CHANNEL_1, 174, 1811, 250, 1250);
	servo_init(&s_servo_lan, &htim3, TIM_CHANNEL_1, 174, 1811, 250, 1250);
	servo_init(&s_servo_left_flap, &htim2, TIM_CHANNEL_2, 174, 1811, 250, 1250);
	servo_init(&s_servo_right_flap, &htim3, TIM_CHANNEL_3, 174, 1811, 250, 1250);

	tmp = servo_write(&s_servo_left_ail, 922);
	if (!tmp) {
		return false;
	}
	tmp = servo_write(&s_servo_right_ail, 922);
	if (!tmp) {
		return false;
	}
	tmp = servo_write(&s_servo_ele, 922);
	if (!tmp) {
		return false;
	}
	tmp = servo_write(&s_servo_rud, 992);
	if (!tmp) {
		return false;
	}
	tmp = servo_write(&s_bldc_left_mot, 174);
	if (!tmp) {
		return false;
	}
	tmp = servo_write(&s_bldc_right_mot, 174);
	if (!tmp) {
		return false;
	}
	tmp = servo_write(&s_servo_lan, 922);
	if (!tmp) {
		return false;
	}
	tmp = servo_write(&s_servo_left_flap, 922);
	if (!tmp) {
		return false;
	}
	tmp = servo_write(&s_servo_right_flap, 922);
	if (!tmp) {
		return false;
	}

	tmp = crsf_init_frame_queue(&s_consur_crsf_frame_queue);
	if (!tmp) {
		return false;
	}

	tmp = msgbox_get(TRCEIVER_MSGBOX_NAME_RX_CRSF, TRCEIVER_MSGBOX_NAMELEN_RX_CRSF, &s_trcivr_rx_crsf_msgbox_id);
	if (!tmp) {
		return false;
	}

	tmp = msgbox_subscribe(s_trcivr_rx_crsf_msgbox_id, consur_msgbox_rx_crsf_callback);
	if (!tmp) {
		return false;
	}

	return tmp;

}

// msgbox_callback_fp
static void consur_msgbox_rx_crsf_callback(uint8_t msg[], uint8_t msg_len)
{
	uint8_t read_len = 0;
	crsf_parse_frames(&s_consur_crsf_frame_queue, msg, msg_len, &read_len);
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

	while (true) {
		if (!crsf_pop_frame_queue(&s_consur_crsf_frame_queue, &frame)) {
			break;
		}

		if (!crsf_parse_rc_channels(&frame, &rc_channels)) {
			continue;
		}


		if (rc_channels.chan8 == 191) {
			servo_write(&s_bldc_left_mot, rc_channels.chan1);

		} else {
			servo_write(&s_bldc_left_mot, 174);
		}

		servo_write(&s_servo_rud, rc_channels.chan4);
		servo_write(&s_servo_lan, rc_channels.chan4);
		servo_write(&s_servo_ele, 1811 - rc_channels.chan3 + 174);
		servo_write(&s_servo_left_ail, rc_channels.chan2);
		servo_write(&s_servo_right_ail, rc_channels.chan2);
	}

}





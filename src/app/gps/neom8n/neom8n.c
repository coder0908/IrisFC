/*
 * neom8n.c
 *
 *  Created on: 2025. 12. 3.
 *      Author: coder0908
 */

#include "platform/hal/platform_hal.h"
#include "usart.h"
#include "protocols/ubx/utils/ubx_frame_queue.h"
#include "protocols/crsf/utils/crsf_frame_queue.h"
#include "services/msgbox/msgbox.h"
#include "app/tranceiver/tranceiver.h"
#include "neom8n.h"

#define NEOM8N_LEN_RXBUF 150

static UART_HandleTypeDef * const s_neom8n_huart = &huart6;
static uint8_t s_uart_rx_buf[NEOM8N_LEN_RXBUF];

static uint32_t s_crsf_tx_msgbox_id;
static bool s_is_uart_rx_buf_full = false;

static struct ubx_frame_queue s_neom8n_ubx_frame_queue;
static struct crsf_frame_queue s_neom8n_crsf_frame_queue;

bool neom8n_init()
{
	HAL_StatusTypeDef status = HAL_ERROR;
	bool tmp = false;

	tmp = ubx_init_frame_queue(&s_neom8n_ubx_frame_queue);
	if (!tmp) {
		return false;
	}

	tmp = crsf_init_frame_queue(&s_neom8n_crsf_frame_queue);
	if (!tmp) {
		return false;
	}

	tmp = msgbox_get(TRCIVR_MSGBOX_NAME_CRSF_TX, TRCIVR_MSGBOX_NAMELEN_CRSF_TX, &s_crsf_tx_msgbox_id);
	if (!tmp) {
		return false;
	}

	status = HAL_UART_Receive_DMA(s_neom8n_huart, s_uart_rx_buf, NEOM8N_LEN_RXBUF);
	if (status != HAL_OK) {
		return false;
	}
	return true;

}


void neom8n_flush_rx()
{
	if (!s_is_uart_rx_buf_full) {
		return;
	}
	s_is_uart_rx_buf_full = false;

	struct ubx_frame ubx_frame = {0,};
	struct ubx_nav_pvt nav_pvt_frame = {0,};
	struct crsf_frame crsf_frame = {0,};
	uint16_t read_len = 0;
	ubx_parse_frames(&s_neom8n_ubx_frame_queue, s_uart_rx_buf, NEOM8N_LEN_RXBUF, &read_len);

	bool tmp = false;
	while (true) {
		tmp = ubx_pop_frame_queue(&s_neom8n_ubx_frame_queue, &ubx_frame);
		if (!tmp) {
			break;
		}

		bool tmp = ubx_parse_nav_pvt(&ubx_frame, &nav_pvt_frame);
		if (!tmp) {
			continue;
		}

		crsf_framing_gps(&crsf_frame, nav_pvt_frame.latitude_100ndeg, nav_pvt_frame.longitude_100ndeg, (nav_pvt_frame.groundspeed_mmps)/10000, (nav_pvt_frame.heading_of_motion_10udeg)/1000, (nav_pvt_frame.alti_ellipsoid_mm)/1000, nav_pvt_frame.satellite);

		tmp = msgbox_publish(s_crsf_tx_msgbox_id, crsf_frame.frame, crsf_get_frame_length(&crsf_frame));
		assert(tmp);
	}

	if (HAL_UART_Receive_DMA(s_neom8n_huart, s_uart_rx_buf, NEOM8N_LEN_RXBUF) != HAL_OK) {
		assert(false);
	}
}



void neom8n_loop()
{
	neom8n_flush_rx();
}




void neom8n_uart_rx_cplt_callback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == s_neom8n_huart->Instance) {
		s_is_uart_rx_buf_full = true;
	}

}

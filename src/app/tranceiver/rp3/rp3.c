/*
 * rp3.c
 *
 *  Created on: 2025. 12. 3.
 *      Author: coder0908
 */

#include "usart.h"
#include "services/msgbox/msgbox.h"
#include "protocols/crsf/utils/crsf_frame_queue.h"
#include "../tranceiver.h"
#include "rp3.h"

#define RP3_LEN_RXBUF (CRSF_PLDLEN_MAX + CRSF_LEN_SYNC + CRSF_LEN_LEN)

static UART_HandleTypeDef *s_rp3_huart = &huart2;
static uint8_t s_rp3_uart_buf[RP3_LEN_RXBUF];
static bool s_rp3_is_rx_buf_full = false;

static uint32_t s_trcivr_rx_crsf_msgbox_id;
static uint32_t s_trcivr_tx_crsf_msgbox_id;

static struct crsf_frame_queue s_rp3_rx_crsf_queue;
static struct crsf_frame_queue s_rp3_tx_crsf_queue;

static void rp3_msgbox_tx_crsf_callback(uint8_t msg[], uint8_t msg_len);


bool rp3_init()
{
	bool tmp = false;
	HAL_StatusTypeDef status = HAL_ERROR;

	tmp = crsf_init_frame_queue(&s_rp3_rx_crsf_queue);
	if (!tmp) {
		return false;
	}

	tmp = crsf_init_frame_queue(&s_rp3_tx_crsf_queue);
	if (!tmp) {
		return false;
	}


	tmp = msgbox_get(TRCEIVER_MSGBOX_NAME_RX_CRSF, TRCEIVER_MSGBOX_NAMELEN_RX_CRSF, &s_trcivr_rx_crsf_msgbox_id);
	if (!tmp) {
		return false;
	}

	tmp = msgbox_get(TRCEIVER_MSGBOX_NAME_TX_CRSF, TRCEIVER_MSGBOX_NAMELEN_TX_CRSF, &s_trcivr_tx_crsf_msgbox_id);
	if (!tmp) {
		return false;
	}

	tmp = msgbox_subscribe(s_trcivr_tx_crsf_msgbox_id, rp3_msgbox_tx_crsf_callback);
	if (!tmp) {
		return false;
	}

	status = HAL_UART_Receive_DMA(s_rp3_huart, s_rp3_uart_buf, RP3_LEN_RXBUF);
	if (status != HAL_OK) {
		return false;
	}
	return true;
}


// msgbox_callback_fp
static void rp3_msgbox_tx_crsf_callback(uint8_t msg[], uint8_t msg_len)
{
	uint8_t read_len = 0;
	crsf_parse_frames(&s_rp3_tx_crsf_queue, msg, msg_len, &read_len);
}

void rp3_flush_tx()
{
	HAL_StatusTypeDef status = HAL_ERROR;

	bool tmp = false;
	struct crsf_frame frame = {0,};

	while (true) {
		tmp = crsf_pop_frame_queue(&s_rp3_tx_crsf_queue, &frame);
		if (!tmp) {
			break;
		}
		status = HAL_UART_Transmit(s_rp3_huart, frame.frame, (uint16_t)crsf_get_frame_length(&frame), 200);
		assert(status == HAL_OK);
	}
}

void rp3_flush_rx()
{
	if (!s_rp3_is_rx_buf_full) {
		return;
	}

	s_rp3_is_rx_buf_full = false;

	uint8_t read_len = 0;
	crsf_parse_frames(&s_rp3_rx_crsf_queue, s_rp3_uart_buf, RP3_LEN_RXBUF, &read_len);

	struct crsf_frame frame = {0,};
	bool tmp = false;
	while (true) {
		tmp = crsf_pop_frame_queue(&s_rp3_rx_crsf_queue, &frame);
		if (!tmp) {
			break;
		}
		msgbox_publish(s_trcivr_rx_crsf_msgbox_id, frame.frame, crsf_get_frame_length(&frame));
	}

	if (HAL_UART_Receive_DMA(s_rp3_huart, s_rp3_uart_buf, RP3_LEN_RXBUF) != HAL_OK) {
		assert(false);
		return;
	}

}

void rp3_uart_receive_rx_cplt_callback(UART_HandleTypeDef *huart)
{
	if (huart->Instance != s_rp3_huart->Instance) {
		return;
	}
	s_rp3_is_rx_buf_full = true;
}


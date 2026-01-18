/*
 * rp3.c
 *
 *  Created on: 2025. 12. 3.
 *      Author: coder0908
 */

#include "usart.h"
#include "middleware/msgbox/msgbox.h"
#include "lib/crsf/utils/crsf_frame_queue.h"
#include "app/hardware/tranceiver/tranceiver.h"
#include "rp3.h"

#define UART_LEN_RXBUF (CRSF_PLDLEN_MAX + CRSF_LEN_SYNC + CRSF_LEN_LEN)

static UART_HandleTypeDef *s_huart = &huart2;
static uint8_t s_uart_rx_buf[UART_LEN_RXBUF];
static bool s_is_uart_rx_buf_full = false;

static uint32_t s_crsf_rx_msgbox_id;
static uint32_t s_crsf_tx_msgbox_id;

static struct crsf_frame_queue s_crsf_rx_queue;
static struct crsf_frame_queue s_crsf_tx_queue;

static void crsf_tx_msgbox_callback(uint8_t msg[], uint8_t msg_len);

bool rp3_init()
{
	bool ret = false;
	HAL_StatusTypeDef status = HAL_ERROR;

	ret = crsf_init_frame_queue(&s_crsf_rx_queue);
	if (!ret) {
		return false;
	}

	ret = crsf_init_frame_queue(&s_crsf_tx_queue);
	if (!ret) {
		return false;
	}

	ret = msgbox_get(TRCIVR_MSGBOX_NAME_CRSF_RX, TRCIVR_MSGBOX_NAMELEN_CRSF_RX, &s_crsf_rx_msgbox_id);
	if (!ret) {
		return false;
	}

	ret = msgbox_get(TRCIVR_MSGBOX_NAME_CRSF_TX, TRCIVR_MSGBOX_NAMELEN_CRSF_TX, &s_crsf_tx_msgbox_id);
	if (!ret) {
		return false;
	}

	ret = msgbox_subscribe(s_crsf_tx_msgbox_id, crsf_tx_msgbox_callback);
	if (!ret) {
		return false;
	}

	status = HAL_UART_Receive_DMA(s_huart, s_uart_rx_buf, UART_LEN_RXBUF);
	if (status != HAL_OK) {
		return false;
	}
	return true;
}


// msgbox_callback_fp
static void crsf_tx_msgbox_callback(uint8_t msg[], uint8_t msg_len)
{
	uint64_t read_len = 0;
	crsf_parse_frames(&s_crsf_tx_queue, msg, msg_len, &read_len);
}

void rp3_flush_tx()
{

	struct crsf_frame frame = {0,};

	while (crsf_pop_frame_queue(&s_crsf_tx_queue, &frame)) {

		//		HAL_UART_Transmit(s_huart, frame.frame, (uint16_t)crsf_get_frame_length(&frame), 200);
		HAL_UART_Transmit(&huart1, frame.frame, (uint16_t)crsf_get_frame_length(&frame), 200);
	}
}

void rp3_flush_rx()
{
	if (!s_is_uart_rx_buf_full) {
		return;
	}

	s_is_uart_rx_buf_full = false;

	uint64_t read_len = 0;
	crsf_parse_frames(&s_crsf_rx_queue, s_uart_rx_buf, UART_LEN_RXBUF, &read_len);

	struct crsf_frame frame = {0,};
	bool ret = false;

	while (true) {
		ret = crsf_pop_frame_queue(&s_crsf_rx_queue, &frame);
		if (!ret) {
			break;
		}
		msgbox_publish(s_crsf_rx_msgbox_id, frame.frame, crsf_get_frame_length(&frame));
	}

	if (HAL_UART_Receive_DMA(s_huart, s_uart_rx_buf, UART_LEN_RXBUF) != HAL_OK) {
		assert(false);
		return;
	}

}

void rp3_uart_rx_cplt_callback(UART_HandleTypeDef *huart)
{
	if (huart->Instance != s_huart->Instance) {
		return;
	}
	s_is_uart_rx_buf_full = true;
}


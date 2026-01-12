/*
 * neom8n.c
 *
 *  Created on: 2025. 12. 3.
 *      Author: coder0908
 */

#include <math.h>
#include <stdio.h>
#include "platform/hal/platform_hal.h"
#include "usart.h"
#include "i2c.h"
#include "lib/protocols/ubx/utils/ubx_frame_queue.h"
#include "lib/protocols/crsf/crsf.h"
#include "middleware/msgbox/msgbox.h"
#include "app/hardware/tranceiver/tranceiver.h"
#include "drivers/hmc5883l_driver/hmc5883l_driver.h"
#include "neom8n.h"

#define NEOM8N_LEN_RXBUF 150

static UART_HandleTypeDef * const s_neom8n_huart = &huart6;
static uint8_t s_uart_rx_buf[NEOM8N_LEN_RXBUF];

static uint32_t s_crsf_tx_msgbox_id;
static bool s_is_uart_rx_buf_full = false;

static struct ubx_frame_queue s_neom8n_ubx_frame_queue;
static struct hmc5883l s_compass;


bool neom8n_init()
{
	if (!ubx_init_frame_queue(&s_neom8n_ubx_frame_queue)) {
		return false;
	}
//
//	if (!hmc5883l_init(&s_compass, &hi2c1)) {
//		return false;
//	}

	if (!msgbox_get(TRCIVR_MSGBOX_NAME_CRSF_TX, TRCIVR_MSGBOX_NAMELEN_CRSF_TX, &s_crsf_tx_msgbox_id)) {
		return false;
	}

	if (HAL_UART_Receive_DMA(s_neom8n_huart, s_uart_rx_buf, NEOM8N_LEN_RXBUF) != HAL_OK) {
		return false;
	}

	return true;
}

static float hmc5883l_compute_heading(float x_mgauss, float y_mgauss, float declination_degree)
{
	// 1. atan2f를 사용하여 라디안 계산 (y, x)
	// 일반적으로 반시계 방향으로 각도가 증가하므로 시계 방향 방위각을 위해 그대로 사용하거나
	// 모듈 뒤집힘 여부에 따라 -y_mgauss를 사용하기도 합니다.
	float heading = atan2f(y_mgauss, x_mgauss);

	// 2. 편각(Declination) 보정 (라디안 단위로 변환 후 더하기)
	float declination_rad = declination_degree * (M_PI / 180.0f);
	heading += declination_rad;

	// 3. 결과값 범위를 0 ~ 2*PI (0~360도) 사이로 정규화
	if (heading < 0) {
		heading += 2.0f * M_PI;
	}
	if (heading > 2.0f * M_PI) {
		heading -= 2.0f * M_PI;
	}

	// 4. 라디안을 도(Degree) 단위로 최종 변환
	float heading_degrees = heading * (180.0f / M_PI);

	return heading_degrees;
}


static float heading_deg = 0;

void neom8n_loop()
{
	if (!s_is_uart_rx_buf_full) {
		return;
	}
	s_is_uart_rx_buf_full = false;

	struct ubx_frame ubx_frame = {0,};
	struct ubx_nav_pvt nav_pvt_frame = {0,};

	int16_t x_lsb, y_lsb, z_lsb;
	float x_mgauss = 0, y_mgauss = 0, z_mgauss = 0;

	struct crsf_frame crsf_frame = {0,};

	uint16_t read_len = 0;

//	if (hmc5883l_is_data_ready(&s_compass)) {
//		if(hmc5883l_get_mag_lsb(&s_compass, &x_lsb, &y_lsb, &z_lsb)) {
//			hmc5883l_parse_mag(&s_compass, x_lsb, y_lsb, z_lsb, &x_mgauss, &y_mgauss, &z_mgauss);
//			heading_deg = hmc5883l_compute_heading(x_mgauss, y_mgauss, -8.5f);
//			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//		}
//	}


	ubx_parse_frames(&s_neom8n_ubx_frame_queue, s_uart_rx_buf, NEOM8N_LEN_RXBUF, &read_len);
	while (ubx_pop_frame_queue(&s_neom8n_ubx_frame_queue, &ubx_frame)) {

		if (!ubx_parse_nav_pvt(&ubx_frame, &nav_pvt_frame)) {
			continue;
		}
		crsf_framing_gps(&crsf_frame, nav_pvt_frame.latitude_100ndeg, nav_pvt_frame.longitude_100ndeg, (nav_pvt_frame.groundspeed_mmps)/(25.0f/9.0f), heading_deg*100.0f, (nav_pvt_frame.alti_ellipsoid_mm)/1000.0, nav_pvt_frame.satellite);
		msgbox_publish(s_crsf_tx_msgbox_id, crsf_frame.frame, crsf_get_frame_length(&crsf_frame));

		crsf_framing_gps_ex(&crsf_frame, nav_pvt_frame.gnss_fix_type, nav_pvt_frame.northward_velocity_mmps/10.0f, nav_pvt_frame.eastward_velocity_mmps/10.0f, nav_pvt_frame.down_velocity_mmps/-10.0f, 0, 0, nav_pvt_frame.alti_ellipsoid_mm/1000.0f, nav_pvt_frame.horizontal_accuracy_mm/10.0f, nav_pvt_frame.vertical_accuracy_mm/10.0f, 0, 0);
		msgbox_publish(s_crsf_tx_msgbox_id, crsf_frame.frame, crsf_get_frame_length(&crsf_frame));

		crsf_framing_gps_time(&crsf_frame, nav_pvt_frame.year, nav_pvt_frame.month, nav_pvt_frame.day, nav_pvt_frame.hour, nav_pvt_frame.minuate, nav_pvt_frame.second, 0);
		msgbox_publish(s_crsf_tx_msgbox_id, crsf_frame.frame, crsf_get_frame_length(&crsf_frame));

	}

	HAL_UART_Receive_DMA(s_neom8n_huart, s_uart_rx_buf, NEOM8N_LEN_RXBUF);

}




void neom8n_uart_rx_cplt_callback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == s_neom8n_huart->Instance) {
		s_is_uart_rx_buf_full = true;
	}

}

/*
 * g_gps.c
 *
 *  Created on: Jun 21, 2025
 *      Author: mia00
 */


#include "gps.h"
#include "pp.h"
#include "vmd.h"
#include <stdio.h>

#include "crsf/crsf.h"

#define UBX_PACKET_BUF_SIZE 36
#define UART6_RX_BUF_SIZE 36

static uint8_t g_ubx_packet_buf[UBX_PACKET_BUF_SIZE];
static uint8_t g_uart6_rx_buf[UART6_RX_BUF_SIZE];
struct ubx_gps g_gps;

struct ubx_nav_posllh g_gps_nav_posllh;




bool g_gps_init()
{
	init_ubx_gps(&g_gps, &huart6, g_ubx_packet_buf, UBX_PACKET_BUF_SIZE, g_uart6_rx_buf, UART6_RX_BUF_SIZE);

	transmit_ubx_data(&g_gps, UBX_CFG_PRT, 28);
	transmit_ubx_data(&g_gps, UBX_CFG_MSG, 16);
	transmit_ubx_data(&g_gps, UBX_CFG_RATE, 14);
	transmit_ubx_data(&g_gps, UBX_CFG_CFG, 21);

	begin_ubx_gps(&g_gps);

	return true;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	ubx_uart_rx_cplt_callback(huart, &g_gps);

}

bool ubx_callback_callback()
{
	bool checksum = calc_ubx_checksum(&g_gps, NULL, NULL);
	if (!checksum) {
		return false;
	}
	parse_ubx_nav_posllh(&g_gps, &g_gps_nav_posllh);

	//printf("checksum : %d, tow_ms : %lu, raw_longitude : %ld, raw_latitude : %ld, height_ellipsoid_mm : %ld, height_msl_mm : %ld, horizontal_accuracy_mm : %lu, vertical_accuracy_mm : %lu \n", checksum, g_gps_nav_posllh.tow_ms, g_gps_nav_posllh.raw_longitude, g_gps_nav_posllh.raw_latitude, g_gps_nav_posllh.height_ellipsoid_mm, g_gps_nav_posllh.height_msl_mm, g_gps_nav_posllh.horizontal_accuracy_mm, g_gps_nav_posllh.vertical_accuracy_mm);


	/////////////////////
	uint8_t crsf_gps_buf[CRSF_PLD_SIZE_GPS+4] ={0,};
	uint8_t crsf_gps_buf_size = crsf_make_gps(crsf_gps_buf, CRSF_PLD_SIZE_GPS+4, g_gps_nav_posllh.latitude_100ndeg, g_gps_nav_posllh.longitude_100ndeg, 0, 0, g_gps_nav_posllh.height_ellipsoid_mm/10000.0, 0);
	if (crsf_gps_buf_size != 19) {
		return false;
	}
	HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, crsf_gps_buf, 19, 2000);
	if (status != HAL_OK) {
		return false;
	}


	return true;
}

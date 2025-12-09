/*
 * app.c
 *
 *  Created on: May 5, 2025
 *      Author: coder0908
 */


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "stm32f4xx_hal.h"
#include "platform/hal/platform_hal.h"
#include "usart.h"

#include "app/battery/battery.h"
#include "app/consur/consur.h"
#include "app/tranceiver/tranceiver.h"
#include "app/imu/icm20602/icm20602.h"
#include "app/attitude/attitude.h"

#include "app.h"

#define TTTT (10000.0)

int _write(int file, char *p, int len)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)p, len, HAL_MAX_DELAY);
	return len;
}

//static int32_t map(int32_t x, int32_t min_in, int32_t max_in, int32_t min_out, int32_t max_out)
//{
//  return (x - min_in) * (max_out - min_out) / (max_in - min_in) + min_out;
//}

bool setup()
{

	bool ret = false;

	ret = battery_init();
	if (!ret) {
		return false;
	}
//
//	ret = attitude_init();
//	if (!ret) {
//		return false;
//	}

	ret = trcivr_init();
	if (!ret) {
		return false;
	}

	ret = consur_init();
	if (!ret) {
		return false;
	}

//	ret = imu_init();
//	if (!ret) {
//		return false;
//	}

	return ret;
}

//static int16_t g_x_gyro_lsb, g_y_gyro_lsb, g_z_gyro_lsb;
//static int16_t g_x_accel_lsb, g_y_accel_lsb, g_z_accel_lsb;


//static bool is_imu_data_ready = false;

void loop(void)
{
//	is_imu_data_ready = icm20602_is_data_ready(&g_imu);
//	if (is_imu_data_ready) {
//
//		icm20602_get_accel_gyro_lsb(&g_imu, &g_x_accel_lsb, &g_y_accel_lsb, &g_z_accel_lsb, &g_x_gyro_lsb, &g_y_gyro_lsb, &g_z_gyro_lsb);
//
//		icm20602_calc_angle_compfilter(&g_imu, g_x_accel_lsb, g_y_accel_lsb, g_z_accel_lsb, g_x_gyro_lsb, g_y_gyro_lsb, g_z_gyro_lsb);
//
//
//	}

	trcivr_loop();
	consur_loop();
	battery_loop();


}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	trcivr_uart_rx_cplt_callback(huart);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
//	if (huart->Instance == g_trcivr_elrs.huart->Instance) {
//		HAL_UART_AbortReceive(huart);
//		__HAL_UART_CLEAR_PEFLAG(huart);
//		__HAL_UART_CLEAR_FEFLAG(huart);
//		__HAL_UART_CLEAR_NEFLAG(huart);
//		__HAL_UART_CLEAR_OREFLAG(huart);
//
////		HAL_UART_Receive_DMA(huart, g_trcivr_elrs.buf, g_trcivr_elrs.buf_size);
//	}
//
//	if (huart->Instance == g_gps.huart->Instance) {
//		HAL_UART_AbortReceive(huart);
//		__HAL_UART_CLEAR_PEFLAG(huart);
//		__HAL_UART_CLEAR_FEFLAG(huart);
//		__HAL_UART_CLEAR_NEFLAG(huart);
//		__HAL_UART_CLEAR_OREFLAG(huart);
//
////		HAL_UART_Receive_DMA(huart, g_gps.buf, g_gps.buf_size);
//	}

}






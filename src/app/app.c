/*
 * app.c
 *
 *  Created on: May 5, 2025
 *      Author: coder0908
 */


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "platform/hal/platform_hal.h"
#include "usart.h"

#include "battery/battery.h"
#include "consur/consur.h"
#include "tranceiver/tranceiver.h"
#include "imu/imu.h"
#include "attitude/attitude.h"
#include "gps/gps.h"

#include "app.h"

int _write(int file, char *p, int len)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)p, len, HAL_MAX_DELAY);
	return len;
}

bool setup()
{
	bool ret = false;

	ret = battery_init();
	if (!ret) {
		return false;
	}

	ret = gps_init();
	if (!ret) {
		return false;
	}

	ret = trcivr_init();
	if (!ret) {
		return false;
	}

	ret = consur_init();
	if (!ret) {
		return false;
	}

	ret = imu_init();
	if (!ret) {
		return false;
	}

	return ret;
}


void loop(void)
{


	trcivr_loop();
	consur_loop();
	battery_loop();
	gps_loop();
	imu_loop();



}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	trcivr_uart_rx_cplt_callback(huart);
	gps_uart_rx_cplt_callback(huart);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

}






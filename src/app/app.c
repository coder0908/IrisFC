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

#include "hardware/consur/consur.h"
#include "hardware/battery/battery.h"
#include "hardware/tranceiver/tranceiver.h"
#include "hardware/imu/imu.h"
#include "hardware/gnss/gnss.h"
#include "modules/attitude/attitude.h"

#include "app.h"

int _write(int file, char *p, int len)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)p, len, HAL_MAX_DELAY);
	return len;
}

bool setup()
{

	if (!trcivr_init()) {
		return false;
	}

	if (!battery_init()) {
		return false;
	}

	if (!consur_init()) {
		return false;
	}

	if (!imu_init()) {
		return false;
	}

	if (!gnss_init()) {
		return false;
	}

	return true;
}

void loop(void)
{
	trcivr_loop();
	consur_loop();
	battery_loop();
	imu_loop();
	gnss_loop();
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	trcivr_uart_rx_cplt_callback(huart);
	gnss_uart_rx_cplt_callback(huart);

}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
}






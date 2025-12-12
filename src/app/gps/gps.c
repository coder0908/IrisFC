/*
 * gps.c
 *
 *  Created on: 2025. 12. 10.
 *      Author: coder0908
 */


#include "platform/hal/platform_hal.h"
#include "neom8n/neom8n.h"
#include "gps.h"

bool gps_init()
{
	bool ret = false;
	ret = neom8n_init();
	if (!ret) {
		return false;
	}

	return true;
}

void gps_loop()
{
	neom8n_loop();
}

void gps_uart_rx_cplt_callback(UART_HandleTypeDef *huart)
{
	neom8n_uart_rx_cplt_callback(huart);
}

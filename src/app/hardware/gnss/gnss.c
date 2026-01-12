/*
 * gps.c
 *
 *  Created on: 2025. 12. 10.
 *      Author: coder0908
 */


#include "platform/hal/platform_hal.h"
#include "neom8n/neom8n.h"
#include "gnss.h"

bool gnss_init()
{

	if (!neom8n_init()) {
		return false;
	}

	return true;
}

void gnss_loop()
{
	neom8n_loop();
}

void gnss_uart_rx_cplt_callback(UART_HandleTypeDef *huart)
{
	neom8n_uart_rx_cplt_callback(huart);
}

/*
 * tranceiver.c
 *
 *  Created on: 2025. 12. 3.
 *      Author: coder0908
 */

#include "platform/hal/platform_hal.h"
#include "services/msgbox/msgbox.h"
#include "rp3/rp3.h"
#include "rtt24/rtt24.h"
#include "tranceiver.h"


bool trcivr_init()
{
	bool ret = false;

	ret = rp3_init();
	if (!ret ) {
		return false;
	}

	return true;
}

void trcivr_loop()
{
	rp3_flush_rx();
	rp3_flush_tx();
}

void trcivr_uart_rx_cplt_callback(UART_HandleTypeDef *huart)
{
	rp3_uart_rx_cplt_callback(huart);

}

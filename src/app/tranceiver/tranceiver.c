/*
 * tranceiver.c
 *
 *  Created on: 2025. 12. 3.
 *      Author: coder0908
 */

#include "platform/hal/platform_hal.h"
#include "services/msgbox/msgbox.h"
#include "app/tranceiver/rp3/rp3.h"
#include "app/tranceiver/rtt24/rtt24.h"
#include "tranceiver.h"


bool trcivr_init()
{
	rp3_init();

	return true;
}

void trcivr_loop()
{
	rp3_flush_rx();
	rp3_flush_tx();
}

void trcivr_uart_rx_cplt_callback(UART_HandleTypeDef *huart)
{
	rp3_uart_receive_rx_cplt_callback(huart);

}

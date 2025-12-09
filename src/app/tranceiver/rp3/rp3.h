/*
 * rp3.h
 *
 *  Created on: 2025. 12. 3.
 *      Author: coder0908
 */

#pragma once

#include <stdbool.h>
#include "platform/hal/platform_hal.h"

bool rp3_init();
void rp3_flush_rx();
void rp3_flush_tx();
void rp3_uart_receive_rx_cplt_callback(UART_HandleTypeDef *huart);

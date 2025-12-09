/*
 * consur.h
 *
 *  Created on: May 5, 2025
 *      Author: coder0908
 */


#include <stdint.h>
#include <stdbool.h>


bool consur_init();
void consur_loop();
void trcivr_uart_rx_cplt_callback(UART_HandleTypeDef *huart);

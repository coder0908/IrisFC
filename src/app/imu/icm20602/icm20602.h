/*
 * icm20602.h
 *
 *  Created on: Jun 22, 2025
 *      Author: coder0908
 */

#pragma once

#include <stdbool.h>




bool app_icm20602_init();
void app_icm20602_loop();
void app_icm20602_irq_handler();

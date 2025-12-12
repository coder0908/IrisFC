/*
 * tranceiver.h
 *
 *  Created on: 2025. 12. 3.
 *      Author: coder0908
 */

#pragma once

#include <stdbool.h>


#define TRCIVR_MSGBOX_NAME_CRSF_RX	"rx_crsf"
#define TRCIVR_MSGBOX_NAMELEN_CRSF_RX	7
#define TRCIVR_MSGBOX_NAME_CRSF_TX	"tx_crsf"
#define TRCIVR_MSGBOX_NAMELEN_CRSF_TX	7

bool trcivr_init();
void trcivr_loop();

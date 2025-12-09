/*
 * tranceiver.h
 *
 *  Created on: 2025. 12. 3.
 *      Author: coder0908
 */

#pragma once

#include <stdbool.h>


#define TRCEIVER_MSGBOX_NAME_RX_CRSF	"rx_crsf"
#define TRCEIVER_MSGBOX_NAMELEN_RX_CRSF	7
#define TRCEIVER_MSGBOX_NAME_TX_CRSF	"tx_crsf"
#define TRCEIVER_MSGBOX_NAMELEN_TX_CRSF	7

bool trcivr_init();
void trcivr_loop();

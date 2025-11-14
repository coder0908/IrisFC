/*
 * gps.h
 *
 *  Created on: Jun 21, 2025
 *      Author: mia00
 */

#ifndef __GPS_H__
#define __GPS_H__

#include <stdbool.h>
#include <stdint.h>
#include "ubx.h"



extern struct ubx_gps gps;
extern struct ubx_nav_posllh gps_nav_posllh;

bool gps_init();


#endif /* __GPS_H__ */

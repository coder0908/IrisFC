/*
 * app.h
 *
 *  Created on: May 5, 2025
 *      Author: mia00
 */

#ifndef __APP_H__
#define __APP_H__

#include "init/consur/consur.h"
#include "init/gps/gps.h"
#include "init/imu/imu.h"
#include "init/trcivr/trcivr.h"
#include "pp.h"



bool setup();
void loop(void);

#endif /* __APP_H__ */

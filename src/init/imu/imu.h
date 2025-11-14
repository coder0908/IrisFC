/*
 * imu.h
 *
 *  Created on: Jun 22, 2025
 *      Author: mia00
 */

#ifndef __IMU_H__
#define __IMU_H__

#include <stdbool.h>
#include "icm20602_driver.h"


extern struct icm20602 g_imu;


bool imu_init();



#endif /* __IMU_H__ */

/*
 * app.c
 *
 *  Created on: May 5, 2025
 *      Author: mia00
 */


#include "init.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "crsf/crsf.h"

#define TTTT (10000.0)

int _write(int file, char *p, int len)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)p, len, HAL_MAX_DELAY);
	return len;
}

bool setup()
{
	bool ret = false;
//
//	ret = trcivr_init();
//	if (!ret) {
//		return false;
//	}

//	ret = consur_init();
//	if (!ret) {
//		return false;
//	}

//	ret = gps_init();
//	if (!ret) {
//		return false;
//	}

	ret = imu_init();
	if (!ret) {
		return false;
	}
	return ret;
}

static int16_t g_x_gyro_lsb, g_y_gyro_lsb, g_z_gyro_lsb;
static int16_t g_x_accel_lsb, g_y_accel_lsb, g_z_accel_lsb;
static float g_x_gyro_dps, g_y_gyro_dps, g_z_gyro_dps;

void loop(void)
{
	if (icm20602_is_data_ready(&g_imu)) {
		bool tmp;
		tmp = icm20602_get_accel_gyro_lsb(&g_imu, &g_x_accel_lsb, &g_y_accel_lsb, &g_z_accel_lsb, &g_x_gyro_lsb, &g_y_gyro_lsb, &g_z_gyro_lsb);
		//tmp = icm20602_parse_gyro(&g_imu, g_x_gyro_lsb, g_y_gyro_lsb, g_z_gyro_lsb, &g_x_gyro_dps, &g_y_gyro_dps, &g_z_gyro_dps);
		tmp = icm20602_calc_angle_compfilter(&g_imu, g_x_accel_lsb, g_y_accel_lsb, g_z_accel_lsb, g_x_gyro_lsb, g_y_gyro_lsb, g_z_gyro_lsb);




		uint8_t crsf_attitude_buf[CRSF_PLD_SIZE_ATTITUDE+4] = {0,};
		uint8_t crsf_attitude_buf_size = crsf_make_attitude(crsf_attitude_buf, CRSF_PLD_SIZE_ATTITUDE+4, (int16_t)(DEG_TO_RAD(g_imu.x_angle_deg)*10000), (int16_t)(DEG_TO_RAD(g_imu.y_angle_deg)*10000), (int16_t)(DEG_TO_RAD(g_imu.z_angle_deg)*10000));
		if(crsf_attitude_buf_size != 10) {
			return;
		}
		HAL_UART_Transmit(&huart2, crsf_attitude_buf, 10, 2000);



		printf("x : %ld, y : %ld, z : %ld\n\r", (int32_t)g_imu.x_angle_deg, (int32_t)g_imu.y_angle_deg, (int32_t)g_imu.z_angle_deg);


		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	}
//	for (uint32_t i=0; i<1800; i++) {
//
//		servo_write(&bldcLeftMot, i);
//		servo_write(&bldcRightMot, i);
//		servo_write(&servoLeftAil, i);
//		servo_write(&servoRightAil,i);
//		servo_write(&servoEle, i);
//		servo_write(&servoRud, i);
//		servo_write(&servoLan, i);
//		servo_write(&servoLeftFlap, i);
//		servo_write(&servoRightFlap, i);
//		HAL_Delay(10);
//	}
//	uint8_t receiver_buf[1];
//	HAL_StatusTypeDef status = HAL_UART_Receive(&huart2, receiver_buf, 1, 1000);
//	if (status == HAL_OK) {
//		HAL_UART_Transmit(&huart1, receiver_buf, 1, 1000);
//	}
}


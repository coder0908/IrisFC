/*
 * icm20602.c
 *
 *  Created on: Jun 22, 2025
 *      Author: coder0908
 */

#include <stdio.h>
#include "platform/hal/platform_hal.h"
#include "spi.h"
#include "drivers/icm20602/icm20602.h"
#include "protocols/crsf/crsf.h"
#include "services/msgbox/msgbox.h"
#include "app/tranceiver/tranceiver.h"
#include "core/compfilter/compfilter.h"
#include "icm20602.h"
#include "utils/utils.h"


static struct icm20602 s_icm20602;
static struct compfilter s_comfilter;

static uint32_t s_tx_crsf_msgbox_id;

bool app_icm20602_init()
{
	bool ret = false;

	ret = icm20602_init(&s_icm20602, &hspi2, ICM20602_CS_GPIO_Port, ICM20602_CS_Pin, ICM20602_INT_GPIO_Port, ICM20602_INT_Pin, 4000.0/65536.0, 32.0/65536.0, 0.996);
	HAL_Delay(50);
	if (!ret) {
		return false;
	}

	// PWR_MGMT_2 0x6C
//	ret = icm20602_write_spi_byte(&s_icm20602, ICM20602_REG_PWR_MGMT_2, 0x38); // Disable Acc(bit5:3-111), Enable Gyro(bit2:0-000)
	ret = icm20602_write_spi_byte(&s_icm20602, ICM20602_REG_PWR_MGMT_2, 0x00);// Enable Acc(bit5:3-000), Enable Gyro(bit2:0-000)
	HAL_Delay(50);
	if (!ret) {
		return false;
	}

	// set sample rate to 1000Hz and apply a software filter
	ret = icm20602_write_spi_byte(&s_icm20602, ICM20602_REG_SMPLRT_DIV, 0x00);
	HAL_Delay(50);
	if (!ret) {
		return false;
	}

	// Gyro DLPF Config
	//icm20602_write_spi_byte(CONFIG, 0x00); // Gyro LPF fc 250Hz(bit2:0-000)
	ret = icm20602_write_spi_byte(&s_icm20602, ICM20602_REG_CONFIG, 0x05); // Gyro LPF fc 20Hz(bit2:0-100) at 1kHz sample rate
	HAL_Delay(50);
	if (!ret) {
		return false;
	}

	// GYRO_CONFIG 0x1B
	ret = icm20602_write_spi_byte(&s_icm20602, ICM20602_REG_GYRO_CONFIG, 0x18); // Gyro sensitivity +-2000 dps(bit4:3-11), FCHOICE (bit1:0-00)
	HAL_Delay(50);
	if (!ret) {
		return false;
	}

	// ACCEL_CONFIG 0x1C
	ret = icm20602_write_spi_byte(&s_icm20602, ICM20602_REG_ACCEL_CONFIG, 0x18); // Acc sensitivity 16g
	HAL_Delay(50);
	if (!ret) {
		return false;
	}

	// ACCEL_CONFIG2 0x1D
	ret = icm20602_write_spi_byte(&s_icm20602, ICM20602_REG_ACCEL_CONFIG2, 0x03); // Acc FCHOICE 1kHz(bit3-0), DLPF fc 44.8Hz(bit2:0-011)
	HAL_Delay(50);
	if (!ret) {
		return false;
	}

	ret = icm20602_calibrate_accel(&s_icm20602);
	if (!ret) {
		return false;
	}

	ret = icm20602_calibrate_gyro(&s_icm20602);
	if (!ret) {
		return false;
	}

	ret = msgbox_get(TRCIVR_MSGBOX_NAME_CRSF_TX, TRCIVR_MSGBOX_NAMELEN_CRSF_TX, &s_tx_crsf_msgbox_id);
	if (!ret) {
		return false;
	}

	return ret;
}

static int16_t s_x_accel_lsb, s_y_accel_lsb, s_z_accel_lsb;
static int16_t s_x_gyro_lsb, s_y_gyro_lsb, s_z_gyro_lsb;

static float s_x_accel_g, s_y_accel_g, s_z_accel_g;
static float s_x_gyro_dps, s_y_gyro_dps, s_z_gyro_dps;

void app_icm20602_loop()
{
	if (icm20602_is_data_ready(&s_icm20602)) {
		struct crsf_frame frame = {0,};

		icm20602_get_accel_gyro_lsb(&s_icm20602, &s_x_accel_lsb, &s_y_accel_lsb, &s_z_accel_lsb, &s_x_gyro_lsb, &s_y_gyro_lsb, &s_z_gyro_lsb);
		icm20602_parse_accel(&s_icm20602, s_x_accel_lsb, s_y_accel_lsb, s_z_accel_lsb, &s_x_accel_g, &s_y_accel_g, &s_z_accel_g);
		icm20602_parse_gyro(&s_icm20602, s_x_gyro_lsb, s_y_gyro_lsb, s_z_gyro_lsb, &s_x_gyro_dps, &s_y_gyro_dps, &s_z_gyro_dps);

		compfilter_calc_angle(&s_comfilter, s_x_accel_g, s_y_accel_g, s_z_accel_g, s_x_gyro_dps, s_y_gyro_dps, s_z_gyro_dps);

		crsf_framing_attitude(&frame, (int16_t)(DEG_TO_RAD(s_comfilter.x_angle_deg)*10000.0f), (int16_t)(DEG_TO_RAD(s_comfilter.y_angle_deg)*10000.0f), (int16_t)(DEG_TO_RAD(s_comfilter.z_angle_deg)*10000.0f));
//		printf("x : %ld, y : %ld, z : %ld\n\r", (int32_t)s_comfilter.x_angle_deg, (int32_t)s_comfilter.y_angle_deg, (int32_t)s_comfilter.z_angle_deg);
//		printf("ax: %ld, ay: %ld, az: %ld -- gx: %ld, gy: %ld, gz: %ld\n\r", (int32_t)s_x_accel_g, (int32_t)s_y_accel_g, (int32_t)s_z_accel_g, (int32_t)s_x_gyro_dps, (int32_t)s_y_gyro_dps, (int32_t)s_z_gyro_dps);
		msgbox_publish(s_tx_crsf_msgbox_id, frame.frame, crsf_get_frame_length(&frame));
	}
}

void app_icm20602_irq_handler()
{
	icm20602_irq_handler(&s_icm20602);
}


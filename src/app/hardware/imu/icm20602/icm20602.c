/*
 * icm20602.c
 *
 *  Created on: Jun 22, 2025
 *      Author: coder0908
 */

#include <stdio.h>
#include "platform/hal/platform_hal.h"
#include "spi.h"
#include "drivers/icm20602/icm20602_driver.h"
#include "lib/protocols/crsf/crsf.h"
#include "middleware/msgbox/msgbox.h"
#include "app/hardware/tranceiver/tranceiver.h"
#include "lib/compfilter/compfilter.h"
#include "icm20602.h"
#include "lib/math.h"


static struct icm20602 s_icm20602;
static struct compfilter s_comfilter;

static uint32_t s_tx_crsf_msgbox_id;

float map(float x, float min_in, float max_in, float min_out, float max_out)
{
	return (x - min_in) * (max_out - min_out) / (max_in - min_in) + min_out;
}

bool app_icm20602_init()
{
	if (!icm20602_init(&s_icm20602, &hspi2, ICM20602_CS_GPIO_Port, ICM20602_CS_Pin, ICM20602_INT_GPIO_Port, ICM20602_INT_Pin, 4000.0/65536.0, 32.0/65536.0, 0.996)) {
		return false;
	}
	HAL_Delay(50);

	// PWR_MGMT_2 0x6C
	//	ret = icm20602_write_spi_byte(&s_icm20602, ICM20602_REG_PWR_MGMT_2, 0x38); // Disable Acc(bit5:3-111), Enable Gyro(bit2:0-000)
	if (!icm20602_write_spi_byte(&s_icm20602, ICM20602_REG_PWR_MGMT_2, 0x00)) {
		return false;
	}

	// set sample rate to 1000Hz and apply a software filter
	if (!icm20602_write_spi_byte(&s_icm20602, ICM20602_REG_SMPLRT_DIV, 0x00)) {
		return false;
	}
	HAL_Delay(50);

	// Gyro DLPF Config
	//icm20602_write_spi_byte(CONFIG, 0x00); // Gyro LPF fc 250Hz(bit2:0-000)
	// Gyro LPF fc 20Hz(bit2:0-100) at 1kHz sample rate
	if (!icm20602_write_spi_byte(&s_icm20602, ICM20602_REG_CONFIG, 0x05)) {
		return false;
	}
	HAL_Delay(50);

	// GYRO_CONFIG 0x1B
	// +- 2000dps
	if (!icm20602_write_spi_byte(&s_icm20602, ICM20602_REG_GYRO_CONFIG, 0x18)) {
		return false;
	}
	HAL_Delay(50);

	// ACCEL_CONFIG 0x1C
	// Acc sensitivity +-16g
	if (!icm20602_write_spi_byte(&s_icm20602, ICM20602_REG_ACCEL_CONFIG, 0x18)) {
		return false;
	}
	HAL_Delay(50);

	// ACCEL_CONFIG2 0x1D
	// Acc FCHOICE 1kHz(bit3-0), DLPF fc 44.8Hz(bit2:0-011)
	if (!icm20602_write_spi_byte(&s_icm20602, ICM20602_REG_ACCEL_CONFIG2, 0x03)) {
		return false;
	}
	HAL_Delay(50);

	if (!icm20602_calibrate_gyro(&s_icm20602)) {
		return false;
	}
	HAL_Delay(50);


	if (!msgbox_get(TRCIVR_MSGBOX_NAME_CRSF_TX, TRCIVR_MSGBOX_NAMELEN_CRSF_TX, &s_tx_crsf_msgbox_id)) {
		return false;
	}

	compfilter_init(&s_comfilter, 0.06);

	return true;
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

		s_x_accel_g = map(s_x_accel_g, -16, 16, 16, -16);
		s_y_accel_g = map(s_y_accel_g, -16, 16, 16, -16);
		s_z_accel_g = map(s_z_accel_g, -16, 16, 16, -16);

		s_x_gyro_dps = map(s_x_gyro_dps, -2000, 2000, 2000, -2000);
		s_y_gyro_dps = map(s_y_gyro_dps, -2000, 2000, 2000, -2000);
		s_z_gyro_dps = map(s_z_gyro_dps, -2000, 2000, 2000, -2000);

		s_comfilter.pitch_deg = map(s_comfilter.pitch_deg, -180, 180, 180, -180);
		s_comfilter.roll_deg= map(s_comfilter.roll_deg, -180, 180, 180, -180);
		s_comfilter.yaw_deg = map(s_comfilter.yaw_deg, -180, 180, 180, -180);

		crsf_framing_attitude(&frame, (int16_t)(DEG_TO_RAD(s_comfilter.pitch_deg)*10000.0f), (int16_t)(DEG_TO_RAD(s_comfilter.roll_deg)*10000.0f), (int16_t)(DEG_TO_RAD(s_comfilter.yaw_deg)*10000.0f));
		msgbox_publish(s_tx_crsf_msgbox_id, frame.frame, crsf_get_frame_length(&frame));
		crsf_framing_rate(&frame, (int16_t)(s_x_gyro_dps*10.0f), (int16_t)(s_y_gyro_dps*10.0f), (int16_t)(s_z_gyro_dps*10.0f));
		msgbox_publish(s_tx_crsf_msgbox_id, frame.frame, crsf_get_frame_length(&frame));
		crsf_framing_accel(&frame, (int16_t)(s_x_accel_g*1000.0f), (int16_t)(s_y_accel_g*1000.0f), (int16_t)(s_z_accel_g*1000.0f));
		msgbox_publish(s_tx_crsf_msgbox_id, frame.frame, crsf_get_frame_length(&frame));
	}
}

void app_icm20602_irq_handler()
{
	icm20602_irq_handler(&s_icm20602);
}


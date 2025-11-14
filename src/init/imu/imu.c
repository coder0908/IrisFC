/*
 * imu.c
 *
 *  Created on: Jun 22, 2025
 *      Author: mia00
 */


#include "imu.h"
#include "pp.h"

#define FIXED_ACCL_CALIBRATION false

struct icm20602 g_imu;


bool imu_init()
{
	bool ret;



	ret = icm20602_init(&g_imu, &hspi2, ICM20602_0_CS_GPIO_Port, ICM20602_0_CS_Pin, ICM20602_0_READY_GPIO_Port, ICM20602_0_READY_Pin, 4000.0/65536.0, 32.0/65536.0, 0.004);
	HAL_Delay(50);
	if (!ret) {
		return false;
	}


	// PWR_MGMT_2 0x6C
//	ret = icm20602_write_spi_byte(&g_imu, ICM20602_REG_PWR_MGMT_2, 0x38); // Disable Acc(bit5:3-111), Enable Gyro(bit2:0-000)
	ret = icm20602_write_spi_byte(&g_imu, ICM20602_REG_PWR_MGMT_2, 0x00);// Enable Acc(bit5:3-000), Enable Gyro(bit2:0-000)
	HAL_Delay(50);
	if (!ret) {
		return false;
	}

	// set sample rate to 1000Hz and apply a software filter
	ret = icm20602_write_spi_byte(&g_imu, ICM20602_REG_SMPLRT_DIV, 0x00);
	HAL_Delay(50);
	if (!ret) {
		return false;
	}

	// Gyro DLPF Config
	//icm20602_write_spi_byte(CONFIG, 0x00); // Gyro LPF fc 250Hz(bit2:0-000)
	ret = icm20602_write_spi_byte(&g_imu, ICM20602_REG_CONFIG, 0x05); // Gyro LPF fc 20Hz(bit2:0-100) at 1kHz sample rate
	HAL_Delay(50);
	if (!ret) {
		return false;
	}

	// GYRO_CONFIG 0x1B
	ret = icm20602_write_spi_byte(&g_imu, ICM20602_REG_GYRO_CONFIG, 0x18); // Gyro sensitivity +-2000 dps(bit4:3-11), FCHOICE (bit1:0-00)
	HAL_Delay(50);
	if (!ret) {
		return false;
	}

	// ACCEL_CONFIG 0x1C
	ret = icm20602_write_spi_byte(&g_imu, ICM20602_REG_ACCEL_CONFIG, 0x18); // Acc sensitivity 16g
	HAL_Delay(50);
	if (!ret) {
		return false;
	}

	// ACCEL_CONFIG2 0x1D
	ret = icm20602_write_spi_byte(&g_imu, ICM20602_REG_ACCEL_CONFIG2, 0x03); // Acc FCHOICE 1kHz(bit3-0), DLPF fc 44.8Hz(bit2:0-011)
	HAL_Delay(50);
	if (!ret) {
		return false;
	}

	ret = icm20602_calibrate_accel(&g_imu);
	if (!ret) {
		return false;
	}

	ret = icm20602_calibrate_gyro(&g_imu);
	if (!ret) {
		return false;
	}

	return true;
}


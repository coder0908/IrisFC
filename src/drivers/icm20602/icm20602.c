/*
 * icm20602.c
 *
 *  Created on: Jun 28, 2025
 *      Author: coder0908
 */


#include <assert.h>
#include "icm20602.h"

static void en_cs(struct icm20602 *imu)
{
	assert(imu);

	HAL_GPIO_WritePin(imu->cs_port, imu->cs_pin, GPIO_PIN_RESET);
}

static void dis_cs(struct icm20602 *imu)
{
	assert(imu);

	HAL_GPIO_WritePin(imu->cs_port, imu->cs_pin, GPIO_PIN_SET);
}

static bool icm20602_read_spi(struct icm20602 *imu, uint8_t reg, uint8_t *buf, uint8_t size)
{
	assert(imu);
	assert(buf);
	assert(size != 0);

	HAL_StatusTypeDef spi_status;
	reg |= 0x80;

	en_cs(imu);
	spi_status = HAL_SPI_Transmit(imu->hspi, &reg, 1, 500);
	if (spi_status != HAL_OK) {
		dis_cs(imu);
		return 0;
	}

	spi_status = HAL_SPI_Receive(imu->hspi, buf, size, 2000);
	if (spi_status != HAL_OK) {
		dis_cs(imu);
		return 0;
	}

	dis_cs(imu);

	return true;
}

static bool icm20602_write_spi(struct icm20602 *imu, uint8_t reg, const uint8_t *buf, uint8_t size)
{
	assert(imu);
	assert(buf);
	assert(size != 0);

	HAL_StatusTypeDef spi_status;
	reg &= 0x7f;

	en_cs(imu);
	spi_status = HAL_SPI_Transmit(imu->hspi, &reg, 1, 500);
	if (spi_status != HAL_OK) {
		dis_cs(imu);
		return 0;
	}

	spi_status = HAL_SPI_Transmit(imu->hspi, buf, size, 2000);
	if (spi_status != HAL_OK) {
		dis_cs(imu);
		return 0;
	}

	dis_cs(imu);

	return true;
}

/*static*/ bool icm20602_read_spi_byte(struct icm20602 *imu, uint8_t reg, uint8_t *buf)
{
	return icm20602_read_spi(imu, reg, buf, 1);
}

/*static*/ bool icm20602_write_spi_byte(struct icm20602 *imu, uint8_t reg, uint8_t buf)
{
	return icm20602_write_spi(imu, reg, &buf, 1);
}

bool icm20602_init(struct icm20602 *imu, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin, GPIO_TypeDef *int_port, uint16_t int_pin, float degree_per_lsb, float g_per_lsb, float compfilter_alpha)
{
	assert(imu);
	assert(hspi);
	assert(cs_port);
	assert(int_port);

	dis_cs(imu);

	imu->hspi = hspi;
	imu->cs_port = cs_port;
	imu->cs_pin = cs_pin;
	imu->int_port = int_port;
	imu->int_pin = int_pin;

	imu->degree_per_lsb = degree_per_lsb;
	imu->g_per_lsb = g_per_lsb;

	bool tmp = false;
	uint8_t who_am_i = 0;
	for (uint8_t i = 0; i < 5; i++) {
		tmp = icm20602_read_spi_byte(imu, ICM20602_REG_WHO_AM_I, &who_am_i);
		if (who_am_i == 0x12 && tmp) {
			break;
		} else if (i == 4) {
			return false;
		}
	}

	// PWR_MGMT_1 0x6B
	tmp = icm20602_write_spi_byte(imu, ICM20602_REG_PWR_MGMT_1, 0x80); //Reset ICM20602
	HAL_Delay(50);
	if (!tmp) {
		return false;
	}

	// PWR_MGMT_1 0x6B
	tmp = icm20602_write_spi_byte(imu, ICM20602_REG_PWR_MGMT_1, 0x01); // Enable Temperature sensor(bit4-0), Use PLL(bit2:0-01)
	HAL_Delay(50);
	if (!tmp) {
		return false;
	}

	tmp = icm20602_write_spi_byte(imu, ICM20602_REG_INT_ENABLE, 0x01); // Enable DRDY Interrupt
	HAL_Delay(50);
	if (!tmp) {
		return false;
	}

	return true;
}

bool icm20602_get_accel_gyro_lsb(struct icm20602 *imu, int16_t *x_accel_lsb, int16_t *y_accel_lsb, int16_t *z_accel_lsb, int16_t *x_gyro_lsb, int16_t *y_gyro_lsb, int16_t *z_gyro_lsb)
{
	assert(x_accel_lsb);
	assert(y_accel_lsb);
	assert(z_accel_lsb);
	assert(x_gyro_lsb);
	assert(y_gyro_lsb);
	assert(z_gyro_lsb);

	bool ret;
	uint8_t data[14];

	ret = icm20602_read_spi(imu, ICM20602_REG_ACCEL_XOUT_H, data, 14);
	if (!ret) {
		return ret;
	}

	*x_accel_lsb = ((data[0] << 8) | data[1]);
	*y_accel_lsb = ((data[2] << 8) | data[3]);
	*z_accel_lsb = ((data[4] << 8) | data[5]);

	*x_gyro_lsb = ((data[8] << 8) | data[9]);
	*y_gyro_lsb = ((data[10] << 8) | data[11]);
	*z_gyro_lsb = ((data[12] << 8) | data[13]);

	return ret;
}

bool icm20602_get_gyro_lsb(struct icm20602 *imu, int16_t *x_gyro_lsb, int16_t *y_gyro_lsb, int16_t *z_gyro_lsb)
{
	assert(x_gyro_lsb);
	assert(y_gyro_lsb);
	assert(z_gyro_lsb);
	bool ret;
	uint8_t data[6] = {0,};

	ret = icm20602_read_spi(imu, ICM20602_REG_GYRO_XOUT_H, data, 6);
	if (!ret) {
		return ret;
	}

	*x_gyro_lsb = ((data[0] << 8) | data[1]);
	*y_gyro_lsb = ((data[2] << 8) | data[3]);
	*z_gyro_lsb = ((data[4] << 8) | data[5]);

	return ret;
}

bool icm20602_get_accel_lsb(struct icm20602 *imu, int16_t *x_accel_lsb, int16_t *y_accel_lsb, int16_t *z_accel_lsb)
{
	assert(x_accel_lsb);
	assert(y_accel_lsb);
	assert(z_accel_lsb);

	bool ret;
	uint8_t data[6] ={0,};

	ret = icm20602_read_spi(imu, ICM20602_REG_ACCEL_XOUT_H, data, 6);
	if (!ret) {
		return ret;
	}

	*x_accel_lsb = ((data[0] << 8) | data[1]);
	*y_accel_lsb = ((data[2] << 8) | data[3]);
	*z_accel_lsb = ((data[4] << 8) | data[5]);

	return ret;
}

// gpio input
//bool icm20602_is_data_ready(struct icm20602 *imu)
//{
//	assert(imu);
//
//	GPIO_PinState gpio_state = HAL_GPIO_ReadPin(imu->int_port, imu->int_pin);
//	if (gpio_state == GPIO_PIN_RESET) {
//		return false;
//	}
//
//	return true;
//}

bool icm20602_is_data_ready(struct icm20602 *imu)
{
	bool ret = imu->is_data_ready = true;
	imu->is_data_ready = false;
	return ret;
}


// exti
void icm20602_irq_handler(struct icm20602 *imu)
{
	imu->is_data_ready = true;
}

bool icm20602_remove_gyro_bias_lsb(struct icm20602 *imu, int16_t x_bias_lsb, int16_t y_bias_lsb, int16_t z_bias_lsb)
{
	icm20602_write_spi_byte(imu, ICM20602_REG_XG_OFFS_USRH, (x_bias_lsb * -2) >> 8);	// gyro_lsb x offset high byte
	icm20602_write_spi_byte(imu, ICM20602_REG_XG_OFFS_USRL, x_bias_lsb * -2);	// gyro_lsb x offset low byte

	icm20602_write_spi_byte(imu, ICM20602_REG_YG_OFFS_USRH, (y_bias_lsb * -2) >> 8);	// gyro_lsb y offset high byte
	icm20602_write_spi_byte(imu, ICM20602_REG_YG_OFFS_USRL, y_bias_lsb * -2);	// gyro_lsb y offset low byte

	icm20602_write_spi_byte(imu, ICM20602_REG_ZG_OFFS_USRH, (z_bias_lsb * -2) >> 8);	// gyro_lsb z offset high byte
	icm20602_write_spi_byte(imu, ICM20602_REG_ZG_OFFS_USRL, z_bias_lsb * -2);	// gyro_lsb z offset low byte

	return true;
}

//LSB
bool icm20602_remove_accel_bias_lsb(struct icm20602 *imu, int16_t x_bias_lsb, int16_t y_bias_lsb, int16_t z_bias_lsb)
{
	x_bias_lsb &= 0x7FFE;
	y_bias_lsb &= 0x7FFE;
	z_bias_lsb &= 0x7FFE;

	icm20602_write_spi_byte(imu, ICM20602_REG_XA_OFFSET_H, (x_bias_lsb * -1) >> 7);
	icm20602_write_spi_byte(imu, ICM20602_REG_XA_OFFSET_L, x_bias_lsb * -1);

	icm20602_write_spi_byte(imu, ICM20602_REG_YA_OFFSET_H, (y_bias_lsb * -1) >> 7);
	icm20602_write_spi_byte(imu, ICM20602_REG_YA_OFFSET_L, y_bias_lsb * -1);

	icm20602_write_spi_byte(imu, ICM20602_REG_ZA_OFFSET_H, (z_bias_lsb * -1) >> 7);
	icm20602_write_spi_byte(imu, ICM20602_REG_ZA_OFFSET_L, z_bias_lsb * -1);

	return true;
}


bool icm20602_calibrate_gyro(struct icm20602 *imu)
{
	int16_t x_gyro_bias_lsb, y_gyro_bias_lsb, z_gyro_bias_lsb;
	int16_t tmp_x_gyro_bias_lsb, tmp_y_gyro_bias_lsb, tmp_z_gyro_bias_lsb;

//	for (i = 1; i < 10000; i++) {
//		icm20602_get_3Axisgyro_lsbRawData(imu, tmp_bias);
//
//		for (j = 0; j < 3; j++) {
//			bias_gyro_lsb[j] += tmp_bias[j];
//			bias_gyro_lsb[j] *= ((i-1) / i);	//int16_t이기 때문에 결과가 0보다 작으면 모두 0이 됨.
//		}
//	}

	for (int i = 0; i < 1000; i++) {
		icm20602_get_accel_lsb(imu, &tmp_x_gyro_bias_lsb, &tmp_y_gyro_bias_lsb, &tmp_z_gyro_bias_lsb);
		x_gyro_bias_lsb += tmp_x_gyro_bias_lsb;
		y_gyro_bias_lsb += tmp_y_gyro_bias_lsb;
		z_gyro_bias_lsb += tmp_z_gyro_bias_lsb;
	}

	x_gyro_bias_lsb /= 1000;
	y_gyro_bias_lsb /= 1000;
	z_gyro_bias_lsb /= 1000;

	return icm20602_remove_gyro_bias_lsb(imu, x_gyro_bias_lsb, y_gyro_bias_lsb, z_gyro_bias_lsb);
}

bool icm20602_calibrate_accel(struct icm20602 *imu)
{
	int16_t x_accel_bias_lsb, y_accel_bias_lsb, z_accel_bias_lsb;
	int16_t tmp_x_accel_bias_lsb, tmp_y_accel_bias_lsb, tmp_z_accel_bias_lsb;

//	for (i = 1; i < 10000; i++) {
//		icm20602_get_3Axisaccel_lsbRawData(imu, tmp_bias);
//
//		for (j = 0; j < 3; j++) {
//			accel_bias_lsb[j] += tmp_bias[j];
//			if (j == 2) {
//				accel_bias_lsb[j] -= 2048; //adc per 1g in +-16g
//			}
//			accel_bias_lsb[j] *= ((i-1) / i);
//		}
//	}

	for (int i = 0; i < 1000; i++) {
		icm20602_get_accel_lsb(imu, &tmp_x_accel_bias_lsb, &tmp_y_accel_bias_lsb, &tmp_z_accel_bias_lsb);
		x_accel_bias_lsb += tmp_x_accel_bias_lsb;
		y_accel_bias_lsb += tmp_y_accel_bias_lsb;
		z_accel_bias_lsb += tmp_z_accel_bias_lsb;

		z_accel_bias_lsb -= 2048;	// 중력 가속도
	}

	x_accel_bias_lsb /= 1000;
	y_accel_bias_lsb /= 1000;
	z_accel_bias_lsb /= 1000;

	return icm20602_remove_accel_bias_lsb(imu, x_accel_bias_lsb, y_accel_bias_lsb, z_accel_bias_lsb);
}

//	calculate

void icm20602_parse_gyro(const struct icm20602 *imu, int16_t x_gyro_lsb, int16_t y_gyro_lsb, int16_t z_gyro_lsb, float *x_gyro_dps, float *y_gyro_dps, float *z_gyro_dps)
{
	assert(imu);


	*x_gyro_dps = x_gyro_lsb * imu->degree_per_lsb;
	*y_gyro_dps = y_gyro_lsb * imu->degree_per_lsb;
	*z_gyro_dps = z_gyro_lsb * imu->degree_per_lsb;

}

void icm20602_parse_accel(const struct icm20602 *imu, int16_t x_accel_lsb, int16_t y_accel_lsb, int16_t z_accel_lsb, float *x_accel_g, float *y_accel_g, float *z_accel_g)
{
	*x_accel_g = x_accel_lsb * imu->g_per_lsb;
	*y_accel_g = y_accel_lsb * imu->g_per_lsb;
	*z_accel_g = z_accel_lsb * imu->g_per_lsb;

}






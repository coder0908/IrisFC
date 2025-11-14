/**

 * icm20602.c
 * @author ChrisP @ M-HIVE

 * This library source code has been created for STM32F4. Only supports SPI.
 *
 * Development environment specifics:
 * STM32CubeIDE 1.0.0
 * STM32CubeF4 FW V1.24.1
 * STM32F4 LL Driver(SPI) and HAL Driver(RCC for HAL_Delay() function)
 *
 * Created by ChrisP(Wonyeob Park) @ M-HIVE Embedded Academy, July, 2019
 * Rev. 1.0
 *
 * https://github.com/ChrisWonyeobPark/
 * https://www.udemy.com/course/stm32-drone-programming/?referralCode=E24CB7B1CD9993855D45
 * https://www.inflearn.com/course/stm32cubelde-stm32f4%EB%93%9C%EB%A1%A0-%EA%B0%9C%EB%B0%9C
*/

/**
 * @brief icm20602 structure definition.
 */

#include "../icm20602_driver/icm20602_driver.h"

#include "vmd.h"
#include <math.h>

#define RADIAN (180/M_PI)

static inline float DEG_TO_RAD(float degree)
{
	return degree / RADIAN;
}

static inline float RAD_TO_DEG(float radian)
{
	return radian * RADIAN;
}



static void en_cs(struct icm20602 *imu)
{
	VMD_ASSERT_PARAM(imu);

	HAL_GPIO_WritePin(imu->cs_port, imu->cs_pin, GPIO_PIN_RESET);
}

static void dis_cs(struct icm20602 *imu)
{
	VMD_ASSERT_PARAM(imu);

	HAL_GPIO_WritePin(imu->cs_port, imu->cs_pin, GPIO_PIN_SET);
}

static bool icm20602_read_spi(struct icm20602 *imu, uint8_t reg, uint8_t *buf, uint8_t size)
{
	VMD_ASSERT_PARAM(imu);
	VMD_ASSERT_PARAM(buf);
	VMD_ASSERT_PARAM(size != 0);

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
	VMD_ASSERT_PARAM(imu);
	VMD_ASSERT_PARAM(buf);
	VMD_ASSERT_PARAM(size != 0);

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

bool icm20602_init(struct icm20602 *imu, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin, GPIO_TypeDef *ready_port, uint16_t ready_pin, float degree_per_lsb, float g_per_lsb, float compfilter_alpha)
{
	VMD_ASSERT_PARAM(imu);
	VMD_ASSERT_PARAM(hspi);
	VMD_ASSERT_PARAM(cs_port);
	VMD_ASSERT_PARAM(ready_port);

	dis_cs(imu);

	imu->hspi = hspi;
	imu->cs_port = cs_port;
	imu->cs_pin = cs_pin;
	imu->ready_port = ready_port;
	imu->ready_pin = ready_pin;

	imu->degree_per_lsb = degree_per_lsb;
	imu->g_per_lsb = g_per_lsb;
	imu->compfilter_alpha = compfilter_alpha;

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
	VMD_ASSERT_PARAM(x_accel_lsb);
	VMD_ASSERT_PARAM(y_accel_lsb);
	VMD_ASSERT_PARAM(z_accel_lsb);
	VMD_ASSERT_PARAM(x_gyro_lsb);
	VMD_ASSERT_PARAM(y_gyro_lsb);
	VMD_ASSERT_PARAM(z_gyro_lsb);

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
	VMD_ASSERT_PARAM(x_gyro_lsb);
	VMD_ASSERT_PARAM(y_gyro_lsb);
	VMD_ASSERT_PARAM(z_gyro_lsb);
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
	VMD_ASSERT_PARAM(x_accel_lsb);
	VMD_ASSERT_PARAM(y_accel_lsb);
	VMD_ASSERT_PARAM(z_accel_lsb);



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

bool icm20602_is_data_ready(struct icm20602 *imu)
{
	VMD_ASSERT_PARAM(imu);

	GPIO_PinState gpio_state = HAL_GPIO_ReadPin(imu->ready_port, imu->ready_pin);
	if (gpio_state == GPIO_PIN_RESET) {
		return false;
	}

	return true;
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

bool icm20602_parse_gyro(const struct icm20602 *imu, int16_t x_gyro_lsb, int16_t y_gyro_lsb, int16_t z_gyro_lsb, float *x_gyro_dps, float *y_gyro_dps, float *z_gyro_dps)
{
	VMD_ASSERT_PARAM(imu);


	*x_gyro_dps = x_gyro_lsb * imu->degree_per_lsb;
	*y_gyro_dps = y_gyro_lsb * imu->degree_per_lsb;
	*z_gyro_dps = z_gyro_lsb * imu->degree_per_lsb;

	return true;
}

bool icm20602_parse_accel(const struct icm20602 *imu, int16_t x_accel_lsb, int16_t y_accel_lsb, int16_t z_accel_lsb, float *x_accel_g, float *y_accel_g, float *z_accel_g)
{
	*x_accel_g = x_accel_lsb * imu->g_per_lsb;
	*y_accel_g = y_accel_lsb * imu->g_per_lsb;
	*z_accel_g = z_accel_lsb * imu->g_per_lsb;

	return true;
}

bool icm20602_calc_angle_compfilter(struct icm20602 *imu, int16_t x_accel_lsb, int16_t y_accel_lsb, int16_t z_accel_lsb, int16_t x_gyro_lsb, int16_t y_gyro_lsb, int16_t z_gyro_lsb)
{
	VMD_ASSERT_PARAM(imu);


	float x_gyro_dps, y_gyro_dps, z_gyro_dps;
	icm20602_parse_gyro(imu, x_gyro_lsb, y_gyro_lsb, z_gyro_lsb, &x_gyro_dps, &y_gyro_dps, &z_gyro_dps);
	float x_accel_g, y_accel_g, z_accel_g;
	icm20602_parse_accel(imu, x_accel_lsb, y_accel_lsb, z_accel_lsb, &x_accel_g, &y_accel_g, &z_accel_g);

	float dt_s = (HAL_GetTick() - imu->prev_time_ms) / 1000.0;

	float add_x_angle_by_gyro_deg = x_gyro_dps * dt_s;	//현재 루프에서 추가된 각도
	float add_y_angle_by_gyro_deg = y_gyro_dps * dt_s;	//현재 루프에서 추가된 각도
	float add_z_angle_by_gyro_deg = z_gyro_dps * dt_s;	//현재 루프에서 추가된 각도


	imu->x_angle_deg -= add_y_angle_by_gyro_deg * sinf(DEG_TO_RAD(add_z_angle_by_gyro_deg));
	imu->y_angle_deg += add_x_angle_by_gyro_deg * sinf(DEG_TO_RAD(add_z_angle_by_gyro_deg));
	imu->z_angle_deg += add_z_angle_by_gyro_deg;

	imu->total_accel_vevtor = sqrt((x_accel_g*x_accel_g) + (y_accel_g*y_accel_g) + (z_accel_g* z_accel_g));

	float x_angle_by_accel_deg = atan2f(y_accel_g, sqrtf(x_accel_g*x_accel_g + z_accel_g*z_accel_g)) * RADIAN;
	float y_angle_by_accel_deg = atan2f(-x_accel_g, sqrtf(y_accel_g*y_accel_g + z_accel_g*z_accel_g)) * RADIAN;

	float alpha;
	if (fabsf(imu->total_accel_vevtor) - 1 > 3) {
		alpha = imu->compfilter_alpha;
	} else {
		alpha = 0.96;
	}

	imu->x_angle_deg = alpha * x_angle_by_accel_deg + (1 - alpha) * (imu->x_angle_deg + add_x_angle_by_gyro_deg);
	imu->y_angle_deg = alpha * y_angle_by_accel_deg + (1 - alpha) * (imu->y_angle_deg + add_y_angle_by_gyro_deg);

	if (imu->x_angle_deg >= 180) {
		imu->x_angle_deg -= 360;
	} else if (imu->x_angle_deg < -180) {
		imu->x_angle_deg += 360;
	}

	if (imu->y_angle_deg >= 180) {
		imu->y_angle_deg -= 360;
	} else if (imu->y_angle_deg < -180) {
		imu->y_angle_deg += 360;
	}

	if (imu->z_angle_deg >= 180) {
		imu->z_angle_deg -= 360;
	} else if (imu->z_angle_deg < -180) {
		imu->z_angle_deg += 360;
	}

	imu->prev_time_ms = HAL_GetTick();

	return true;
}

//
//
//bool icm20602_calc_angle(struct icm20602 *imu, int16_t x_gyro_lsb, int16_t y_gyro_lsb, int16_t z_gyro_lsb)
//{
//	VMD_ASSERT_PARAM(imu);
//
//	float x_gyro_dps, y_gyro_dps, z_gyro_dps;
//	icm20602_parse_gyro(imu, x_gyro_lsb, y_gyro_lsb, z_gyro_lsb, &x_gyro_dps, &y_gyro_dps, &z_gyro_dps);
//
//	float dt_s = (HAL_GetTick() - imu->prev_time_ms) / 1000.0;
//
//
//	float add_x_angle_by_gyro_deg = x_gyro_dps / imu->frequency;	//현재 루프에서 추가된 각도
//	float add_y_angle_by_gyro_deg = y_gyro_dps / imu->frequency;	//현재 루프에서 추가된 각도
//	float add_z_angle_by_gyro_deg = z_gyro_dps / imu->frequency;	//현재 루프에서 추가된 각도
//
//	imu->x_angle_deg += add_x_angle_by_gyro_deg;
//	imu->y_angle_deg += add_y_angle_by_gyro_deg;
//	imu->z_angle_deg += add_z_angle_by_gyro_deg;
//
//	imu->x_angle_deg -= add_y_angle_by_gyro_deg * sinf(DEG_TO_RAD(add_z_angle_by_gyro_deg));
//	imu->y_angle_deg += add_x_angle_by_gyro_deg * sinf(DEG_TO_RAD(add_z_angle_by_gyro_deg));
//
//	imu->prev_time_ms = HAL_GetTick();
//
//	return true;
//}






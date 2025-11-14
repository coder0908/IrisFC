#ifndef __ICM20602_DRIVER_H__
#define __ICM20602_DRIVER_H__

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"


/*
ICM-20602 SPI Operational Features
1. Data is delivered MSB first and LSB last
2. Data is latched on the rising edge of SPC
3. Data should be transitioned on the falling edge of SPC
4. The maximum frequency of SPC is 10MHz
5. SPI read and write operations are completed in 16 or more clock cycles (two or more bytes). The first byte contains the
SPI Address, and the following byte(s) contain(s) the SPI data. The first bit of the first byte contains the Read/Write bit
and indicates the Read (1) or Write (0) operation. The following 7 bits contain the Register Address. In cases of multiple byte Read/Writes, data is two or more bytes:
*/

#define	ICM20602_REG_XG_OFFS_TC_H	0x04
#define	ICM20602_REG_XG_OFFS_TC_L	0x05
#define	ICM20602_REG_YG_OFFS_TC_H	0x07
#define	ICM20602_REG_YG_OFFS_TC_L	0x08
#define	ICM20602_REG_ZG_OFFS_TC_H	0x0A
#define	ICM20602_REG_ZG_OFFS_TC_L	0x0B
#define	ICM20602_REG_SELF_TEST_X_ACCEL	0x0D
#define	ICM20602_REG_SELF_TEST_Y_ACCEL	0x0E
#define	ICM20602_REG_SELF_TEST_Z_ACCEL	0x0F
#define	ICM20602_REG_XG_OFFS_USRH	0x13
#define	ICM20602_REG_XG_OFFS_USRL	0x14
#define	ICM20602_REG_YG_OFFS_USRH	0x15
#define	ICM20602_REG_YG_OFFS_USRL	0x16
#define	ICM20602_REG_ZG_OFFS_USRH	0x17
#define	ICM20602_REG_ZG_OFFS_USRL	0x18
#define	ICM20602_REG_SMPLRT_DIV	0x19
#define	ICM20602_REG_CONFIG	0x1A    //The default value of the register is 0x80.
#define	ICM20602_REG_GYRO_CONFIG	0x1B
#define	ICM20602_REG_ACCEL_CONFIG	0x1C
#define	ICM20602_REG_ACCEL_CONFIG2	0x1D
#define	ICM20602_REG_LP_MODE_CFG	0x1E
#define	ICM20602_REG_ACCEL_WOM_X_THR	0x20
#define	ICM20602_REG_ACCEL_WOM_Y_THR	0x21
#define	ICM20602_REG_ACCEL_WOM_Z_THR	0x22
#define	ICM20602_REG_FIFO_EN	0x23
#define	ICM20602_REG_FSYNC_INT	0x36
#define	ICM20602_REG_INT_PIN_CFG	0x37
#define	ICM20602_REG_INT_ENABLE	0x38
#define	ICM20602_REG_FIFO_WM_INT_STATUS	0x39

#define	ICM20602_REG_INT_STATUS	0x3A
#define	ICM20602_REG_ACCEL_XOUT_H	0x3B
#define	ICM20602_REG_ACCEL_XOUT_L	0x3C
#define	ICM20602_REG_ACCEL_YOUT_H	0x3D
#define	ICM20602_REG_ACCEL_YOUT_L	0x3E
#define	ICM20602_REG_ACCEL_ZOUT_H	0x3F
#define	ICM20602_REG_ACCEL_ZOUT_L	0x40
#define	ICM20602_REG_TEMP_OUT_H	0x41
#define	ICM20602_REG_TEMP_OUT_L	0x42
#define	ICM20602_REG_GYRO_XOUT_H	0x43
#define	ICM20602_REG_GYRO_XOUT_L	0x44
#define	ICM20602_REG_GYRO_YOUT_H	0x45
#define	ICM20602_REG_GYRO_YOUT_L	0x46
#define	ICM20602_REG_GYRO_ZOUT_H	0x47
#define	ICM20602_REG_GYRO_ZOUT_L	0x48
#define	ICM20602_REG_SELF_TEST_X_GYRO	0x50
#define	ICM20602_REG_SELF_TEST_Y_GYRO	0x51
#define	ICM20602_REG_SELF_TEST_Z_GYRO	0x52
#define	ICM20602_REG_FIFO_WM_TH1	0x60
#define	ICM20602_REG_FIFO_WM_TH2	0x61
#define	ICM20602_REG_SIGNAL_PATH_RESET	0x68
#define	ICM20602_REG_ACCEL_INTEL_CTRL	0x69
#define	ICM20602_REG_USER_CTRL	0x6A
#define	ICM20602_REG_PWR_MGMT_1	0x6B //The default value of the register is 0x41.
#define	ICM20602_REG_PWR_MGMT_2	0x6C
#define	ICM20602_REG_I2C_IF	0x70
#define	ICM20602_REG_FIFO_COUNTH	0x72
#define	ICM20602_REG_FIFO_COUNTL	0x73
#define	ICM20602_REG_FIFO_R_W	0x74
#define	ICM20602_REG_WHO_AM_I	0x75 //The default value of the register is 0x12.
#define	ICM20602_REG_XA_OFFSET_H	0x77
#define	ICM20602_REG_XA_OFFSET_L	0x78
#define	ICM20602_REG_YA_OFFSET_H	0x7A
#define	ICM20602_REG_YA_OFFSET_L	0x7B
#define	ICM20602_REG_ZA_OFFSET_H	0x7D
#define	ICM20602_REG_ZA_OFFSET_L	0x7E


/**
 * @brief ICM20602 structure definition.
 */

struct icm20602 {
	 SPI_HandleTypeDef *hspi;

	 GPIO_TypeDef *cs_port;
	 uint16_t	cs_pin;

	 GPIO_TypeDef *ready_port;
	 uint16_t	ready_pin;

	 float degree_per_lsb;
	 float g_per_lsb;
	 float compfilter_alpha;

	 uint32_t prev_time_ms;

	 float x_angle_deg;
	 float y_angle_deg;
	 float z_angle_deg;

	 float total_accel_vevtor;
};




bool icm20602_read_spi_byte(struct icm20602 *imu, uint8_t reg, uint8_t *buf);
bool icm20602_write_spi_byte(struct icm20602 *imu, uint8_t reg, uint8_t buf);

bool icm20602_init(struct icm20602 *imu, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin, GPIO_TypeDef *ready_port, uint16_t ready_pin, float degree_per_lsb, float g_per_lsb, float compfilter_alpha);

bool icm20602_get_accel_gyro_lsb(struct icm20602 *imu, int16_t *x_accel_lsb, int16_t *y_accel_lsb, int16_t *z_accel_lsb, int16_t *x_gyro_lsb, int16_t *y_gyro_lsb, int16_t *z_gyro_lsb);
bool icm20602_get_gyro_lsb(struct icm20602 *imu, int16_t *x_gyro_lsb, int16_t *y_gyro_lsb, int16_t *z_gyro_lsb);
bool icm20602_get_accel_lsb(struct icm20602 *imu, int16_t *x_accel_lsb, int16_t *y_accel_lsb, int16_t *z_accel_lsb);

bool icm20602_is_data_ready(struct icm20602 *imu);

bool icm20602_remove_gyro_bias_lsb(struct icm20602 *imu, int16_t x_bias_lsb, int16_t y_bias_lsb, int16_t z_bias_lsb);
bool icm20602_remove_accel_bias_lsb(struct icm20602 *imu, int16_t x_bias_lsb, int16_t y_bias_lsb, int16_t z_bias_lsb);

bool icm20602_calibrate_gyro(struct icm20602 *imu);
bool icm20602_calibrate_accel(struct icm20602 *imu);

bool icm20602_parse_gyro(const struct icm20602 *imu, int16_t x_gyro_lsb, int16_t y_gyro_lsb, int16_t z_gyro_lsb, float *x_gyro_dps, float *y_gyro_dps, float *z_gyro_dps);

bool icm20602_calc_angle(struct icm20602 *imu, int16_t x_gyro_lsb, int16_t y_gyro_lsb, int16_t z_gyro_lsb);
bool icm20602_calc_angle_compfilter(struct icm20602 *imu, int16_t x_accel_lsb, int16_t y_accel_lsb, int16_t z_accel_lsb, int16_t x_gyro_lsb, int16_t y_gyro_lsb, int16_t z_gyro_lsb);



#endif /*__ICM20602_DRIVER_H__*/


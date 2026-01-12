#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

/* I2C Address */
#define HMC5883L_ADDR               (0x1E << 1)

/* Register Map */
#define HMC5883L_REG_CRA            0x00
#define HMC5883L_REG_CRB            0x01
#define HMC5883L_REG_MODE           0x02
#define HMC5883L_REG_OUT_X_MSB      0x03
#define HMC5883L_REG_OUT_X_LSB      0x04
#define HMC5883L_REG_OUT_Z_MSB      0x05
#define HMC5883L_REG_OUT_Z_LSB      0x06
#define HMC5883L_REG_OUT_Y_MSB      0x07
#define HMC5883L_REG_OUT_Y_LSB      0x08
#define HMC5883L_REG_STATUS         0x09
#define HMC5883L_REG_ID_A           0x0A
#define HMC5883L_REG_ID_B           0x0B
#define HMC5883L_REG_ID_C           0x0C

/* Status Register */
#define HMC5883L_STATUS_RDY_BIT     0x01
#define HMC5883L_STATUS_LOCK_BIT    0x02

/* Configuration Register A */
#define HMC5883L_SAMPLES_8          (3 << 5)
#define HMC5883L_RATE_15HZ          (4 << 2)
#define HMC5883L_BIAS_POSITIVE      (1 << 0)

/* Mode Register */
#define HMC5883L_MODE_CONTINUOUS    0x00
#define HMC5883L_MODE_SINGLE        0x01
#define HMC5883L_MODE_IDLE          0x02

#define HMC5883L_I2C_TIMEOUT_MS     200

#define HMC5883L_SELFTEST_MIN 200
#define HMC5883L_SELFTEST_MAX 900

struct hmc5883l {
	I2C_HandleTypeDef *hi2c;
	float mgauss_per_lsb;
};

/* Function Prototypes */
bool hmc5883l_init(struct hmc5883l *mag, I2C_HandleTypeDef *hi2c);
bool hmc5883l_self_test(struct hmc5883l *mag);
bool hmc5883l_is_data_ready(struct hmc5883l *mag);
bool hmc5883l_get_mag_lsb(struct hmc5883l *mag, int16_t *x, int16_t *y, int16_t *z);
void hmc5883l_parse_mag(const struct hmc5883l *mag,
		int16_t x_lsb, int16_t y_lsb, int16_t z_lsb,
		float *x_mgauss, float *y_mgauss, float *z_mgauss);
bool hmc5883l_read_reg(struct hmc5883l *mag, uint8_t reg, uint8_t *buf, uint16_t size);
bool hmc5883l_write_reg(struct hmc5883l *mag, uint8_t reg, uint8_t val);

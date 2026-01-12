#include <assert.h>
#include <stdlib.h>
#include "hmc5883l_driver.h"

bool hmc5883l_write_reg(struct hmc5883l *mag, uint8_t reg, uint8_t val)
{
	return (HAL_I2C_Mem_Write(
			mag->hi2c,
			HMC5883L_ADDR,
			reg,
			1,
			&val,
			1,
			HMC5883L_I2C_TIMEOUT_MS
	) == HAL_OK);
}

bool hmc5883l_read_reg(struct hmc5883l *mag, uint8_t reg, uint8_t *buf, uint16_t size)
{
	return (HAL_I2C_Mem_Read(
			mag->hi2c,
			HMC5883L_ADDR,
			reg,
			1,
			buf,
			size,
			HMC5883L_I2C_TIMEOUT_MS
	) == HAL_OK);
}

bool hmc5883l_init(struct hmc5883l *mag, I2C_HandleTypeDef *hi2c)
{
    assert(mag);
    assert(hi2c);

    mag->hi2c = hi2c;
    mag->mgauss_per_lsb = 0.92f;

    // 1. 센서 전원 안정화 대기
    HAL_Delay(50);

    // 2. ID 확인 전 I2C 버스 초기화 확인을 위해 ID 읽기 시도 (여러 번 재시도)
    uint8_t id[3];
    int retry = 3;
    while (retry--) {
        if (hmc5883l_read_reg(mag, HMC5883L_REG_ID_A, id, 3)) {
            if (id[0] == 'H' && id[1] == '4' && id[2] == '3') break;
        }
        HAL_Delay(10);
        if (retry == 0) return false;
    }

    // 3. 셀프 테스트 수행
//    if (!hmc5883l_self_test(mag)) return false;

    // 4. 셀프 테스트 후 반드시 기본 상태로 리셋 (중요!)
    // CRA: 8-average, 15Hz, Normal measurement configuration (0x70)
    if (!hmc5883l_write_reg(mag, HMC5883L_REG_CRA, HMC5883L_SAMPLES_8 | HMC5883L_RATE_15HZ)) return false;

    // CRB: Gain 1.3Ga (0x20)
    if (!hmc5883l_write_reg(mag, HMC5883L_REG_CRB, 0x20)) return false;

    // Mode: Continuous-measurement mode (0x00)
    if (!hmc5883l_write_reg(mag, HMC5883L_REG_MODE, HMC5883L_MODE_CONTINUOUS)) return false;

    return true;
}

bool hmc5883l_self_test(struct hmc5883l *mag)
{
    assert(mag);

    // 셀프 테스트를 위해 Bias를 Positive로 설정 (0x71)
    if (!hmc5883l_write_reg(mag, HMC5883L_REG_CRA, 0x71)) return false;
    // 테스트용 Gain 설정 (0xA0)
    if (!hmc5883l_write_reg(mag, HMC5883L_REG_CRB, 0xA0)) return false;
    // Single-measurement 모드로 전환하여 1회 측정 유도
    if (!hmc5883l_write_reg(mag, HMC5883L_REG_MODE, HMC5883L_MODE_SINGLE)) return false;

    HAL_Delay(60); // 측정 완료까지 충분한 시간 대기 (Datasheet 기준 15Hz 시 약 67ms)

    int16_t x, y, z;
    if (!hmc5883l_get_mag_lsb(mag, &x, &y, &z)) return false;

    // 셀프 테스트 종료 후 Normal 모드로 원복을 위해 CRA 리셋 (매우 중요)
    // 이 작업을 안하면 센서가 계속 Bias 전류를 흘려 데이터가 오염됩니다.
    hmc5883l_write_reg(mag, HMC5883L_REG_CRA, 0x70);

    x = abs(x); y = abs(y); z = abs(z);

    // 환경에 따라 범위를 조금 더 넓게 잡거나,
    // 실패 시 로그를 남겨 어느 축에서 실패했는지 확인하는 것이 좋습니다.
    return (x >= 200 && x <= 900) && (y >= 200 && y <= 900) && (z >= 200 && z <= 900);
}


bool hmc5883l_is_data_ready(struct hmc5883l *mag)
{
	assert(mag);

	uint8_t status;
	if (!hmc5883l_read_reg(mag, HMC5883L_REG_STATUS, &status, 1)) {
		return false;
	}

	return (status & HMC5883L_STATUS_RDY_BIT);
}

bool hmc5883l_get_mag_lsb(struct hmc5883l *mag, int16_t *x, int16_t *y, int16_t *z)
{
    assert(mag && x && y && z);
    uint8_t data[6];

    // 1. 읽기 실패 여부 확인
    if (!hmc5883l_read_reg(mag, HMC5883L_REG_OUT_X_MSB, data, 6)) {
        // 여기에 브레이크 포인트를 걸어 I2C 통신 에러인지 확인하세요.
        return false;
    }

    *x = (int16_t)((data[0] << 8) | data[1]);
    *z = (int16_t)((data[2] << 8) | data[3]);
    *y = (int16_t)((data[4] << 8) | data[5]);

    // 2. 포화(Saturation) 여부 확인
    if (*x == -4096 || *y == -4096 || *z == -4096) {
        // 자기장이 너무 강해 센서가 측정 범위를 벗어났을 때 발생합니다.
        return false;
    }

    return true;
}

void hmc5883l_parse_mag(const struct hmc5883l *mag,
		int16_t x_lsb, int16_t y_lsb, int16_t z_lsb,
		float *x_mgauss, float *y_mgauss, float *z_mgauss)
{
	assert(mag && x_mgauss && y_mgauss && z_mgauss);

	*x_mgauss = (float)x_lsb * mag->mgauss_per_lsb;
	*y_mgauss = (float)y_lsb * mag->mgauss_per_lsb;
	*z_mgauss = (float)z_lsb * mag->mgauss_per_lsb;
}

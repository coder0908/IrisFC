/*
 * crsf.c
 *
 *  Created on: Nov 13, 2025
 *      Author: unRR
 */


#include "crsf.h"
#include "vmd.h"
#include <stdio.h>
#include "usart.h"
/*
 * sync_char(1), length(1), type(1), payload(length-2), crc(1)
 *
 * length : includes 'type', 'payload','crc'
 * crc : include 'type', 'payload'
 * */

#define CRSF_SYNC_CHAR 0xc8

static void reverse_endian_u16(uint8_t *buf, uint16_t value)
{
	buf[0] = value >> 8;
	buf[1] = value & 0xff;
}

static void reverse_endian_u24(uint8_t *buf, uint32_t/*int로 바꿔야 할 수도*/ value)
{
	buf[0] = value >> 16;
	buf[1] = (value >> 8) & 0xff;
	buf[2] = value & 0xff;
}

static void reverse_endian_i32(uint8_t *buf, int32_t value)
{
	buf[0] = value >> 24;
	buf[1] = (value >> 16) & 0xff;
	buf[2] = (value >> 8) & 0xff;
	buf[3] = value & 0xff;
}

//buf : include type, payload
static uint8_t crsf_calc_crc8(const uint8_t *buf, uint8_t size)
{
	uint8_t crc = 0;
	for (uint8_t i=0; i<size; i++) {
		crc ^= buf[i];
		for (int j = 0; j < 8; j++) {
			if (crc & 0x80) {
				crc = (crc << 1) ^ 0xd5;

			} else {
				crc <<= 1;
			}
		}
	}

	return crc;
}

//데이터 수신 시에만 사용. 송신시 struct crsf_frame사용 하지 않고, 바로 frame생성후 전송.
bool crsf_parse_frame(struct crsf_frame *frame, const uint8_t *buf, uint8_t size)
{
	VMD_ASSERT_PARAM(frame);
	VMD_ASSERT_PARAM(buf);

	if (size < 4) {
		return false;
	}
	//sync byte
	if (buf[0] != CRSF_SYNC_CHAR) {
		return false;
	}
	//length
	if (buf[1] != size || buf[1] > CRSF_PLD_MAX_SIZE + 2) {
		return false;
	}
	//crc
	uint8_t crc = crsf_calc_crc8(buf+2, size-3);
	if (buf[size-1] != crc) {
		return false;
	}

	frame->length = buf[1];
	frame->type = buf[2];
	frame->crc = crc;

	for (int i=3; i<size; i++) {
		frame->payload[i-3] = buf[i];
	}

	return true;
}

uint8_t crsf_get_type(const struct crsf_frame *frame)
{
	VMD_ASSERT_PARAM(frame);

	return frame->type;
}

bool crsf_parse_gps(struct crsf_gps *gps, const struct crsf_frame *frame)
{
	VMD_ASSERT_PARAM(gps);
	VMD_ASSERT_PARAM(frame);

	if (frame->type != CRSF_TYPE_GPS) {
		return false;
	}

	reverse_endian_i32((uint8_t*)&gps->latitude_100ndeg, ((int32_t*)frame->payload)[0]);
	reverse_endian_i32((uint8_t*)&gps->longitude_100ndeg, ((int32_t*)frame->payload)[1]);
	reverse_endian_u16((uint8_t*)&gps->groundspeed_damph, ((uint16_t*)frame->payload)[4]);
	reverse_endian_u16((uint8_t*)&gps->heading_cdeg, ((uint16_t*)frame->payload)[5]);
	reverse_endian_u16((uint8_t*)&gps->altitude_m, ((uint16_t*)frame->payload)[6]);
	gps->satellites = frame->payload[CRSF_PLD_SIZE_GPS-1];

	gps->altitude_m -= 1000;

	return true;
}

bool crsf_parse_gps_time(struct crsf_gps_time *gps_time, const struct crsf_frame *frame)
{
	VMD_ASSERT_PARAM(gps_time);
	VMD_ASSERT_PARAM(frame);

}

bool crsf_parse_gps_ex(struct crsf_gps_ex *gps_ex, const struct crsf_frame *frame)
{
	VMD_ASSERT_PARAM(frame);

}

bool crsf_parse_variometer(struct crsf_variometer *vario, const struct crsf_frame *frame)
{
	VMD_ASSERT_PARAM(frame);

}

bool crsf_parse_battery(struct crsf_battery *batt, const struct crsf_frame *frame)
{
	VMD_ASSERT_PARAM(frame);

}

bool crsf_parse_airspeed(struct crsf_airspeed *airspeed, const struct crsf_frame *frame)
{
	VMD_ASSERT_PARAM(frame);

}

bool crsf_parse_rc_channels(struct crsf_rc_channels *rc_channels, const struct crsf_frame *frame)
{
	VMD_ASSERT_PARAM(frame);

}

bool crsf_parse_attitude(struct crsf_attitude *attitude, const struct crsf_frame *frame)
{
	VMD_ASSERT_PARAM(attitude);
	VMD_ASSERT_PARAM(frame);

	if (frame->type != CRSF_TYPE_ATTITUDE) {
		return false;
	}

	int16_t *ptr_i16 = (int16_t*)(frame->payload);

	attitude->pitch_angle_100urad = ptr_i16[0];
	attitude->roll_angle_100urad = ptr_i16[1];
	attitude->yaw_angle_100urad = ptr_i16[2];


	return true;
}

//0 means fail. non zero means success and size of buf
uint8_t crsf_make_gps(uint8_t *buf, uint8_t size, int32_t latitude_100ndeg, int32_t longitude_100ndeg,
		uint16_t groundspeed_damph, uint16_t heading_cdeg, uint16_t altitude_m, uint8_t satellites)
{
	VMD_ASSERT_PARAM(buf);

	if (size < CRSF_PLD_SIZE_GPS+4) {
		return 0;
	}

	buf[0] = CRSF_SYNC_CHAR;
	buf[1] = CRSF_PLD_SIZE_GPS + 2;
	buf[2] = CRSF_TYPE_GPS;

	reverse_endian_i32(buf+3, latitude_100ndeg);
	reverse_endian_i32(buf+7, longitude_100ndeg);
	reverse_endian_u16(buf+11, groundspeed_damph);
	reverse_endian_u16(buf+13, heading_cdeg);
	reverse_endian_u16(buf+15, altitude_m+1000);
	buf[17] = satellites;

	buf[18] = crsf_calc_crc8(buf+2, CRSF_PLD_SIZE_GPS+1);

	return CRSF_PLD_SIZE_GPS+4;
}

uint8_t crsf_make_attitude(uint8_t *buf, uint8_t size, int16_t pitch_angle_100urad, int16_t roll_angle_100urad, int16_t yaw_angle_100urad)
{
	VMD_ASSERT_PARAM(buf);

	if (size < CRSF_PLD_SIZE_ATTITUDE+4) {
		return 0;
	}

	buf[0] = CRSF_SYNC_CHAR;
	buf[1] = CRSF_PLD_SIZE_ATTITUDE + 2;
	buf[2] = CRSF_TYPE_ATTITUDE;

	int16_t *ptr_i16 = (int16_t*)(buf+3);

	reverse_endian_u16(buf+3, pitch_angle_100urad);
	reverse_endian_u16(buf+5, roll_angle_100urad);
	reverse_endian_u16(buf+7, yaw_angle_100urad);

//	ptr_i16[0] = pitch_angle_100urad;
//	ptr_i16[1] = roll_angle_100urad;
//	ptr_i16[2] = yaw_angle_100urad;

	buf[9] = crsf_calc_crc8(buf+2, CRSF_PLD_SIZE_ATTITUDE+1);

	return CRSF_PLD_SIZE_ATTITUDE+4;
}















/*
 * ubx.c
 *
 *  Created on: Jul 12, 2025
 *      Author: coder0908
 */


#include "ubx.h"

static uint32_t buf_to_u32_little_endian(const uint8_t* buf)
{
	return (uint32_t)buf[3] | ((uint32_t)buf[2] << 8) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[0] << 24);
}

static uint32_t buf_to_u24_little_endian(const uint8_t* buf)
{
	return (uint32_t)buf[2] | ((uint32_t)buf[1] << 8) | ((uint32_t)buf[0] << 16);
}

static uint16_t buf_to_u16_little_endian(const uint8_t* buf)
{
	return buf[1] | (buf[0] << 8);
}

static void ubx_checksum(uint8_t *ck_a, uint8_t *ck_b, uint8_t val)
{
	*ck_a = *ck_a + val;
	*ck_b = *ck_b + *ck_a;
}

static void ubx_set_sync_a(struct ubx_frame *frame, uint8_t sync_a)
{
	assert(frame);

	frame->frame[UBX_IDX_SYNC_A] = sync_a;

}

static void ubx_set_sync_b(struct ubx_frame *frame, uint8_t sync_b)
{
	assert(frame);

	frame->frame[UBX_IDX_SYNC_B] = sync_b;

}

static void ubx_set_class(struct ubx_frame *frame, enum ubx_class class)
{
	assert(frame);
	frame->frame[UBX_IDX_CLASS] = (uint8_t)class;

}

static void ubx_set_id(struct ubx_frame *frame, enum ubx_id id)
{
	assert(frame);

	frame->frame[UBX_IDX_ID] = (uint8_t)id;

}

static void ubx_set_len(struct ubx_frame *frame, uint16_t len)
{
	assert(frame);

	uint16_t *ptr = (uint16_t*)&(frame->frame[UBX_IDX_LEN]);
	*ptr = len;
}

static void ubx_set_payload(struct ubx_frame *frame, const uint8_t payload[], uint16_t payload_len)
{
	assert(frame);

	for (uint16_t i=0; i<ubx_get_len(frame); i++) {
		 frame->frame[UBX_IDX_PAYLOAD+i] = payload[i];
	}
}

static void ubx_set_ck_a(struct ubx_frame *frame, uint8_t ck_a)
{
	assert(frame);
	frame->frame[UBX_IDX_CK_A(ubx_get_len(frame))] = ck_a;
}

static void ubx_set_ck_b(struct ubx_frame *frame, uint8_t ck_b)
{
	assert(frame);
	frame->frame[UBX_IDX_CK_B(ubx_get_len(frame))] = ck_b;

}


void ubx_calc_checksum_buf(const uint8_t *buf, uint16_t buf_len, uint8_t *ck_a, uint8_t *ck_b)
{
	assert(buf);
	assert(ck_a);
	assert(ck_b);

	*ck_a = 0;
	*ck_b = 0;

	for (uint16_t i=2; i < buf_len-2; i++) {
		ubx_checksum(ck_a, ck_b, buf[i]);
	}
}

void ubx_calc_checksum_frame(const struct ubx_frame *frame, uint8_t *ck_a, uint8_t *ck_b)
{
	assert(frame);
	assert(ck_a);
	assert(ck_b);

	const uint16_t impact_len = UBX_LEN_CLASS+UBX_LEN_ID+UBX_LEN_LEN+ubx_get_len(frame);
	const uint8_t *buf = &(frame->frame[UBX_IDX_CLASS]);

	*ck_a = 0;
	*ck_b = 0;

	for (uint16_t i=0; i<impact_len; i++) {
		ubx_checksum(ck_a, ck_b, buf[i]);
	}
}


bool ubx_parse_frame(struct ubx_frame *frame, const uint8_t *buf, uint16_t buf_len, uint16_t *read_len)
{
	assert(frame);
	assert(buf);

	if (buf_len < UBX_LEN_METADATA) {
		*read_len = buf_len;
		return false;
	}

	if (buf[0] != UBX_SYNC_A) {
		*read_len = 1;
		return false;
	}

	if (buf[1] != UBX_SYNC_B) {
		*read_len = 2;
		return false;
	}

	ubx_set_sync_a(frame, buf[UBX_IDX_SYNC_A]);
	ubx_set_sync_b(frame, buf[UBX_IDX_SYNC_B]);
	ubx_set_class(frame, buf[UBX_IDX_CLASS]);
	ubx_set_id(frame, buf[UBX_IDX_ID]);

	uint16_t *len = (uint16_t*)(&buf[UBX_IDX_LEN]);

	if (buf_len < *len+UBX_LEN_METADATA || *len > UBX_PLDLEN_MAX) {
		*read_len = buf_len;
		return false;
	}

	ubx_set_len(frame, *len);

	uint8_t ck_a = 0, ck_b = 0;
	ubx_calc_checksum_buf(buf, ubx_get_frame_length(frame), &ck_a, &ck_b);
	if (ck_a != buf[UBX_IDX_CK_A(*len)] || ck_b != buf[UBX_IDX_CK_B(*len)]) {
		return false;
	}

	ubx_set_ck_a(frame, ck_a);
	ubx_set_ck_b(frame, ck_b);

	ubx_set_payload(frame, &buf[UBX_IDX_PAYLOAD], *len);

	return true;
}


bool ubx_parse_nav_posllh(const struct ubx_frame *frame, struct ubx_nav_posllh *nav_posllh)
{
	assert(frame);
	assert(nav_posllh);


	if (ubx_get_class(frame) != UBX_CLASS_NAV || ubx_get_id(frame) != UBX_ID_POSLLH) {
		return false;
	}

	if (ubx_get_len(frame) != UBX_PLDLEN_NAV_POSLLH) {
		return false;
	}

	const uint8_t *payload = &(frame->frame[UBX_IDX_PAYLOAD]);

	nav_posllh->tow_ms = *((uint32_t*)(payload));
	nav_posllh->longitude_100ndeg =  *((int32_t*)(payload+4));
	nav_posllh->latitude_100ndeg =  *((int32_t*)(payload+8));
	nav_posllh->alti_ellipsoid_mm =  *((int32_t*)(payload+12));
	nav_posllh->alti_msl_mm = *((int32_t*)(payload+16));
	nav_posllh->horizontal_accuracy_mm =  *((uint32_t*)(payload+20));
	nav_posllh->vertical_accuracy_mm =  *((uint32_t*)(payload+24));

	return true;
}

bool ubx_parse_nav_pvt(struct ubx_frame *frame, struct ubx_nav_pvt *nav_pvt)
{
	assert(frame);
	assert(nav_pvt);

	if (ubx_get_class(frame) != UBX_CLASS_NAV || ubx_get_id(frame) != UBX_ID_PVT) {
		return false;
	}

	if (ubx_get_len(frame) != UBX_PLDLEN_NAV_PVT) {
		return false;
	}

	const uint8_t *payload = &(frame->frame[UBX_IDX_PAYLOAD]);

	nav_pvt->tow_ms = *((uint32_t*)(payload));
	nav_pvt->year = *((uint16_t*)(payload+4));
	nav_pvt->month = payload[6];
	nav_pvt->day = payload[12];
	nav_pvt->hour = payload[8];
	nav_pvt->minuate = payload[9];
	nav_pvt->second = payload[10];
	nav_pvt->valid = payload[11];
	nav_pvt->time_accuracy_ns = *((uint32_t*)(payload+12));
	nav_pvt->fraction_of_second_ns = *((int32_t*)(payload+16));
	nav_pvt->gnss_fix_type = payload[20];
	nav_pvt->flags = payload[21];
	nav_pvt->flags2 = payload[22];
	nav_pvt->satellite = payload[23];
	nav_pvt->longitude_100ndeg = *((int32_t*)(payload+24));
	nav_pvt->latitude_100ndeg = *((int32_t*)(payload+28));
	nav_pvt->alti_ellipsoid_mm = *((int32_t*)(payload+32));
	nav_pvt->alti_msl_mm = *((int32_t*)(payload+36));
	nav_pvt->horizontal_accuracy_mm = *((uint32_t*)(payload+40));
	nav_pvt->vertical_accuracy_mm = *((uint32_t*)(payload+44));
	nav_pvt->northward_velocity_mmps = *((int32_t*)(payload+48));
	nav_pvt->eastward_velocity_mmps = *((int32_t*)(payload+52));
	nav_pvt->down_velocity_mmps = *((int32_t*)(payload+56));
	nav_pvt->groundspeed_mmps = *((int32_t*)(payload+60));
	nav_pvt->heading_of_motion_10udeg = *((int32_t*)(payload+64));
	nav_pvt->velocity_accuracy_mmps = *((uint32_t*)(payload+68));
	nav_pvt->heading_accuracy_10udeg = *((uint32_t*)(payload+72));
	nav_pvt->position_dop_centi = *((uint16_t*)(payload+76));
	nav_pvt->flags3 = *((uint16_t*)(payload+78));
	nav_pvt->reserved1 = payload[80];
	nav_pvt->reserved2 = payload[81];
	nav_pvt->reserved3 = payload[82];
	nav_pvt->reserved4 = payload[83];
	nav_pvt->heading_of_vehicle_10udeg = *((int32_t*)(payload+84));
	nav_pvt->magnetic_declination_cdeg = *((int16_t*)(payload+88));
	nav_pvt->magnetic_declination_accuracy_cdeg = *((uint16_t*)(payload+90));

	return true;
}




/*
 * crsf.h
 *
 *  Created on: Nov 13, 2025
 *      Author: unRR
 */

#ifndef __CRSF_H__
#define __CRSF_H__

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include <math.h>

#define CRSF_PLD_MAX_SIZE 60	//include type, payload, crc

#define CRSF_TYPE_GPS			0x02
#define CRSF_TYPE_GPS_TIME		0x03
#define CRSF_TYPE_GPS_EX		0x06
#define CRSF_TYPE_VARIOMETER		0x07
#define CRSF_TYPE_BATTERY		0x08
#define CRSF_TYPE_BAROMETER		0x09
#define CRSF_TYPE_AIRSPEED		0x0A
#define CRSF_TYPE_TEMPERATURE		0x0D
#define CRSF_TYPE_RC_CHANNELS		0x16
#define CRSF_TYPE_ATTITUDE		0x1E

//crsf payload size
#define CRSF_PLD_SIZE_GPS		15
#define CRSF_PLD_SIZE_GPS_TIME		9
#define CRSF_PLD_SIZE_GPS_EX		20
#define CRSF_PLD_SIZE_VARIOMETER	2
#define CRSF_PLD_SIZE_BATTERY		8
#define CRSF_PLD_SIZE_BAROMETER		3
#define CRSF_PLD_SIZE_AIRSPEED		2
#define CRSF_PLD_SIZE_TEMP		3
#define CRSF_PLD_SIZE_RC_CHANNELS	22
#define CRSF_PLD_SIZE_ATTITUDE		6

#define RADIAN (180/M_PI)

static inline float DEG_TO_RAD(float degree)
{
	return degree / RADIAN;
}

static inline float RAD_TO_DEG(float radian)
{
	return radian * RADIAN;
}

struct crsf_frame {
	//uint8_t sync_char;
	uint8_t length;
	uint8_t type;
	uint8_t payload[CRSF_PLD_MAX_SIZE];
	uint8_t crc;
};

struct crsf_gps {
	int32_t latitude_100ndeg;      	 // degree/1e-7
	int32_t longitude_100ndeg;    	  // degree/1e-7
	uint16_t groundspeed_damph; 	  // decameter/hour
	uint16_t heading_cdeg;      	 // cnetidegree
	uint16_t altitude_m;      	// meter
	uint8_t satellites;
};

struct crsf_gps_time {
	int16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t millisecond;
};

struct crsf_gps_ex {
	uint8_t fix_type;      				 // Current GPS fix quality
	int16_t northward_speed_cmps;        		// Northward (north = positive) Speed [cm/sec]
	int16_t eastward_speed_cmps;        			// Eastward (east = positive) Speed [cm/sec]
	int16_t vertical_speed_cmps;       			 // Vertical (up = positive) Speed [cm/sec]
	int16_t horizontal_speed_accuracy_cmps;    		// Horizontal Speed accuracy cm/sec
	int16_t track_accuracy_deg;      			// Heading accuracy in degrees scaled with 1e-1 degrees times 10)
	int16_t ellipsoid_height_m;  			// Meters Height above GPS Ellipsoid (not MSL)
	int16_t horizontal_accuracy_cm;         		 // horizontal accuracy in cm
	int16_t vertical_accuracy_cm;          		// vertical accuracy in cm
	uint8_t reserved;
	uint8_t horizontal_dop;          		 // Horizontal dilution of precision,Dimensionless in units of 0.1.
	uint8_t vertical_dop;           		// vertical dilution of precision, Dimensionless in units of 0.1.
};

struct crsf_variometer {
	int16_t vertical_speed_cmps;	// cm/s
};

struct crsf_battery {
	int16_t voltage_10uv;        // Voltage µV	//LSB first
	int16_t current_10ua;        // Current 10 µA	//LSB first
	uint32_t capacity_used; 	// Capacity used (mAh)	actually 24bit, ignore highest 8bit
	uint8_t remaining_percent;      // Battery remaining (percent)
};

//어려움 나중에 구현
//struct crsf_barometer {
//
//};

struct crsf_airspeed {
	uint16_t airspeed_hmph;		//hectometer/h
};

//variable length
//struct crsf_temperature {
//	uint8_t temp_source_id;		// Identifies the source of the temperature data (e.g., 0 = FC including all ESCs, 1 = Ambient, etc.)
//	int16_t temperature[20]; 	// up to 20 temperature values in deci-degree (tenths of a degree) Celsius (e.g., 250 = 25.0°C, -50 = -5.0°C)
//};

// Center (1500µs) = 992
struct crsf_rc_channels {
	int channel_01: 11;
	int channel_02: 11;
	int channel_03: 11;
	int channel_04: 11;
	int channel_05: 11;
	int channel_06: 11;
	int channel_07: 11;
	int channel_08: 11;
	int channel_09: 11;
	int channel_10: 11;
	int channel_11: 11;
	int channel_12: 11;
	int channel_13: 11;
	int channel_14: 11;
	int channel_15: 11;
	int channel_16: 11;
};

struct crsf_attitude {
	int16_t pitch_angle_100urad;	// 100micro radian	//LSB first
	int16_t roll_angle_100urad;	// 100micro radian	//LSB first
	int16_t yaw_angle_100urad;	// 100micro radian	//LSB first
};

bool crsf_parse_frame(struct crsf_frame *frame, const uint8_t *buf, uint8_t size);
uint8_t crsf_get_type(const struct crsf_frame *frame);

bool crsf_parse_gps(struct crsf_gps *gps, const struct crsf_frame *frame);


uint8_t crsf_make_gps(uint8_t *buf, uint8_t size, int32_t latitude_100ndeg, int32_t longitude_100ndeg,
		uint16_t groundspeed_damph, uint16_t heading_cdeg, uint16_t altitude_m, uint8_t satellites);
uint8_t crsf_make_attitude(uint8_t *buf, uint8_t size, int16_t pitch_angle_100urad, int16_t roll_angle_100urad, int16_t yaw_angle_100urad);
#endif /* __CRSF_H__ */

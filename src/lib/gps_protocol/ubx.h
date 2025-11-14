/*
 * neom8n_ubx.h
 *
 *  Created on: Jul 12, 2025
 *      Author: mia00
 */

#ifndef __NEOM8N_UBX_H__
#define __NEOM8N_UBX_H__

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

//// UBX Class Definitions
//enum Neom8nUBX_Class {
//	NEOM8N_UBX_NAV = 0x01,
//	NEOM8N_UBX_RXM = 0x02,
//	NEOM8N_UBX_INF = 0x04,
//	NEOM8N_UBX_ACK = 0x05,
//	NEOM8N_UBX_CFG = 0x06,
//	NEOM8N_UBX_UPD = 0x09,
//	NEOM8N_UBX_MON = 0x0A,
//	NEOM8N_UBX_AID = 0x0B,
//	NEOM8N_UBX_TIM = 0x0D,
//	NEOM8N_UBX_ESF = 0x10,
//	NEOM8N_UBX_MGA = 0x13,
//	NEOM8N_UBX_LOG = 0x21,
//	NEOM8N_UBX_SEC = 0x27,
//	NEOM8N_UBX_HNR = 0x28
//};
//
//// UBX Class ACK - Ack/Nak Messages
//enum Neom8nUBX_ACK_Id {
//	NEOM8N_UBX_ACK_ACK = 0x0501,
//	NEOM8N_UBX_ACK_NCK = 0x0500
//};
//
//// UBX Class AID - AssistNow Aiding Messages
//enum Neom8nUBX_AID_Id {
//	NEOM8N_UBX_AID_ALM = 0x0B30,
//	NEOM8N_UBX_AID_AOP = 0x0B33,
//	NEOM8N_UBX_AID_EPH = 0x0B31,
//	NEOM8N_UBX_AID_HUI = 0x0B02,
//	NEOM8N_UBX_AID_INI = 0x0B01
//};
//
//// UBX Class CFG - Configuration Input Messages
//enum Neom8nUBX_CFG_Id {
//	NEOM8N_UBX_CFG_ANT = 0x0613,
//	NEOM8N_UBX_CFG_BATCH = 0x0693,
//	NEOM8N_UBX_CFG_CFG = 0x0609,
//	NEOM8N_UBX_CFG_DAT = 0x0606,
//	NEOM8N_UBX_CFG_DGNSS = 0x0670,
//	NEOM8N_UBX_CFG_DOSC = 0x0661,
//	NEOM8N_UBX_CFG_ESFALG = 0x0656,
//	NEOM8N_UBX_CFG_ESFA = 0x064C,
//	NEOM8N_UBX_CFG_ESFG = 0x064D,
//	NEOM8N_UBX_CFG_ESFWT = 0x0682,
//	NEOM8N_UBX_CFG_ESRC = 0x0660,
//	NEOM8N_UBX_CFG_GEOFENCE = 0x0669,
//	NEOM8N_UBX_CFG_GNSS = 0x063E,
//	NEOM8N_UBX_CFG_HNR = 0x065C,
//	NEOM8N_UBX_CFG_INF = 0x0602,
//	NEOM8N_UBX_CFG_ITFM = 0x0639,
//	NEOM8N_UBX_CFG_LOGFILTER = 0x0647,
//	NEOM8N_UBX_CFG_MSG = 0x0601,
//	NEOM8N_UBX_CFG_NAV5 = 0x0624,
//	NEOM8N_UBX_CFG_NAVX5 = 0x0623,
//	NEOM8N_UBX_CFG_NMEA = 0x0617,
//	NEOM8N_UBX_CFG_ODO = 0x061E,
//	NEOM8N_UBX_CFG_PM2 = 0x063B,
//	NEOM8N_UBX_CFG_PMS = 0x0686,
//	NEOM8N_UBX_CFG_PRT = 0x0600,
//	NEOM8N_UBX_CFG_PWR = 0x0657,
//	NEOM8N_UBX_CFG_RATE = 0x0608,
//	NEOM8N_UBX_CFG_RINV = 0x0634,
//	NEOM8N_UBX_CFG_RST = 0x0604,
//	NEOM8N_UBX_CFG_RXM = 0x0611,
//	NEOM8N_UBX_CFG_SBAS = 0x0616,
//	NEOM8N_UBX_CFG_SENIF = 0x0688,
//	NEOM8N_UBX_CFG_SLAS = 0x068D,
//	NEOM8N_UBX_CFG_SMGR = 0x0662,
//	NEOM8N_UBX_CFG_SPT = 0x0664,
//	NEOM8N_UBX_CFG_TMODE2 = 0x063D,
//	NEOM8N_UBX_CFG_TMODE3 = 0x0671,
//	NEOM8N_UBX_CFG_TP5 = 0x0631,
//	NEOM8N_UBX_CFG_TXSLOT = 0x0653,
//	NEOM8N_UBX_CFG_USB = 0x061B
//};
//
//// UBX Class ESF - External Sensor Fusion Messages
//enum Neom8nUBX_ESF_Id {
//	NEOM8N_UBX_ESF_ALG = 0x1014,
//	NEOM8N_UBX_ESF_INS = 0x1015,
//	NEOM8N_UBX_ESF_MEAS = 0x1002,
//	NEOM8N_UBX_ESF_RAW = 0x1003,
//	NEOM8N_UBX_ESF_STATUS = 0x1010
//};
//
//// UBX Class HNR - High Rate Navigation Results Messages
//enum Neom8nUBX_HNR_Id {
//	NEOM8N_UBX_HNR_ATT = 0x2801,
//	NEOM8N_UBX_HNR_INS = 0x2802,
//	NEOM8N_UBX_HNR_PVT = 0x2800
//};
//
//// UBX Class INF - Information Messages
//enum Neom8nUBX_INF_Id {
//	NEOM8N_UBX_INF_DEBUG = 0x0404,
//	NEOM8N_UBX_INF_ERROR = 0x0400,
//	NEOM8N_UBX_INF_NOTICE = 0x0402,
//	NEOM8N_UBX_INF_TEST = 0x0403,
//	NEOM8N_UBX_INF_WARNING = 0x0401
//};
//
//// UBX Class LOG - Logging Messages
//enum Neom8nUBX_LOG_Id {
//	NEOM8N_UBX_LOG_BATCH = 0x2111,
//	NEOM8N_UBX_LOG_CREATE = 0x2107,
//	NEOM8N_UBX_LOG_ERASE = 0x2103,
//	NEOM8N_UBX_LOG_FINDTIME = 0x210E,
//	NEOM8N_UBX_LOG_INFO = 0x2108,
//	NEOM8N_UBX_LOG_RETRIEVEBATCH = 0x2110,
//	NEOM8N_UBX_LOG_RETRIEVEPOS = 0x210B,
//	NEOM8N_UBX_LOG_RETRIEVEPOSEXTRA = 0x210F,
//	NEOM8N_UBX_LOG_RETRIEVESTRING = 0x210D,
//	NEOM8N_UBX_LOG_RETRIEVE = 0x2109,
//	NEOM8N_UBX_LOG_STRING = 0x2104
//};
//
//// UBX Class MGA - Multiple GNSS Assistance Messages
//enum Neom8nUBX_MGA_Id {
//	NEOM8N_UBX_MGA_ACK_DATA0 = 0x1360,
//	NEOM8N_UBX_MGA_ANO = 0x1320,
//	NEOM8N_UBX_MGA_BDS_EPH = 0x1303,
//	NEOM8N_UBX_MGA_BDS_ALM = 0x1303,
//	NEOM8N_UBX_MGA_BDS_HEALTH = 0x1303,
//	NEOM8N_UBX_MGA_BDS_UTC = 0x1303,
//	NEOM8N_UBX_MGA_BDS_IONO = 0x1303,
//	NEOM8N_UBX_MGA_DBD = 0x1380,
//	NEOM8N_UBX_MGA_FLASH_DATA = 0x1321,
//	NEOM8N_UBX_MGA_FLASH_STOP = 0x1321,
//	NEOM8N_UBX_MGA_FLASH_ACK = 0x1321,
//	NEOM8N_UBX_MGA_GAL_EPH = 0x1302,
//	NEOM8N_UBX_MGA_GAL_ALM = 0x1302,
//	NEOM8N_UBX_MGA_GAL_TIMEOFFSET = 0x1302,
//	NEOM8N_UBX_MGA_GAL_UTC = 0x1302,
//	NEOM8N_UBX_MGA_GLO_EPH = 0x1306,
//	NEOM8N_UBX_MGA_GLO_ALM = 0x1306,
//	NEOM8N_UBX_MGA_GLO_TIMEOFFSET = 0x1306,
//	NEOM8N_UBX_MGA_GPS_EPH = 0x1300,
//	NEOM8N_UBX_MGA_GPS_ALM = 0x1300,
//	NEOM8N_UBX_MGA_GPS_HEALTH = 0x1300,
//	NEOM8N_UBX_MGA_GPS_UTC = 0x1300,
//	NEOM8N_UBX_MGA_GPS_IONO = 0x1300,
//	NEOM8N_UBX_MGA_INI_POS_XYZ = 0x1340,
//	NEOM8N_UBX_MGA_INI_POS_LLH = 0x1340,
//	NEOM8N_UBX_MGA_INI_TIME_UTC = 0x1340,
//	NEOM8N_UBX_MGA_INI_TIME_GNSS = 0x1340,
//	NEOM8N_UBX_MGA_INI_CLKD = 0x1340,
//	NEOM8N_UBX_MGA_INI_FREQ = 0x1340,
//	NEOM8N_UBX_MGA_INI_EOP = 0x1340,
//	NEOM8N_UBX_MGA_QZSS_EPH = 0x1305,
//	NEOM8N_UBX_MGA_QZSS_ALM = 0x1305,
//	NEOM8N_UBX_MGA_QZSS_HEALTH = 0x1305
//};
//
//// UBX Class MON - Monitoring Messages
//enum Neom8nUBX_MON_Id {
//	NEOM8N_UBX_MON_BATCH = 0x0A32,
//	NEOM8N_UBX_MON_GNSS = 0x0A28,
//	NEOM8N_UBX_MON_HW2 = 0x0A0B,
//	NEOM8N_UBX_MON_HW = 0x0A09,
//	NEOM8N_UBX_MON_IO = 0x0A02,
//	NEOM8N_UBX_MON_MSGPP = 0x0A06,
//	NEOM8N_UBX_MON_PATCH = 0x0A27,
//	NEOM8N_UBX_MON_RXBUF = 0x0A07,
//	NEOM8N_UBX_MON_RXR = 0x0A21,
//	NEOM8N_UBX_MON_SMGR = 0x0A2E,
//	NEOM8N_UBX_MON_SPT = 0x0A2F,
//	NEOM8N_UBX_MON_TXBUF = 0x0A08,
//	NEOM8N_UBX_MON_VER = 0x0A04
//};
//
//// UBX Class NAV - Navigation Results Messages
//enum Neom8nUBX_NAV_Id {
//	NEOM8N_UBX_NAV_AOPSTATUS = 0x0160,
//	NEOM8N_UBX_NAV_ATT = 0x0105,
//	NEOM8N_UBX_NAV_CLOCK = 0x0122,
//	NEOM8N_UBX_NAV_COV = 0x0136,
//	NEOM8N_UBX_NAV_DGPS = 0x0131,
//	NEOM8N_UBX_NAV_DOP = 0x0104,
//	NEOM8N_UBX_NAV_EELL = 0x013D,
//	NEOM8N_UBX_NAV_EOE = 0x0161,
//	NEOM8N_UBX_NAV_GEOFENCE = 0x0139,
//	NEOM8N_UBX_NAV_HPPOSECEF = 0x0113,
//	NEOM8N_UBX_NAV_HPPOSLLH = 0x0114,
//	NEOM8N_UBX_NAV_NMI = 0x0128,
//	NEOM8N_UBX_NAV_ODO = 0x0109,
//	NEOM8N_UBX_NAV_ORB = 0x0134,
//	NEOM8N_UBX_NAV_POSECEF = 0x0101,
//	NEOM8N_UBX_NAV_POSLLH = 0x0102,
//	NEOM8N_UBX_NAV_PVT = 0x0107,
//	NEOM8N_UBX_NAV_RELPOSNED = 0x013C,
//	NEOM8N_UBX_NAV_RESETODO = 0x0110,
//	NEOM8N_UBX_NAV_SAT = 0x0135,
//	NEOM8N_UBX_NAV_SBAS = 0x0132,
//	NEOM8N_UBX_NAV_SLAS = 0x0142,
//	NEOM8N_UBX_NAV_SOL = 0x0106,
//	NEOM8N_UBX_NAV_STATUS = 0x0103,
//	NEOM8N_UBX_NAV_SVINFO = 0x0130,
//	NEOM8N_UBX_NAV_SVIN = 0x013B,
//	NEOM8N_UBX_NAV_TIMEBDS = 0x0124,
//	NEOM8N_UBX_NAV_TIMEGAL = 0x0125,
//	NEOM8N_UBX_NAV_TIMEGLO = 0x0123,
//	NEOM8N_UBX_NAV_TIMEGPS = 0x0120,
//	NEOM8N_UBX_NAV_TIMELS = 0x0126,
//	NEOM8N_UBX_NAV_TIMEUTC = 0x0121,
//	NEOM8N_UBX_NAV_VELECEF = 0x0111,
//	NEOM8N_UBX_NAV_VELNED = 0x0112
//};
//
//// UBX Class RXM - Receiver Manager Messages
//enum Neom8nUBX_RXM_Id {
//	NEOM8N_UBX_RXM_IMES = 0x0261,
//	NEOM8N_UBX_RXM_MEASX = 0x0214,
//	NEOM8N_UBX_RXM_PMREQ = 0x0241,
//	NEOM8N_UBX_RXM_RAWX = 0x0215,
//	NEOM8N_UBX_RXM_RLM = 0x0259,
//	NEOM8N_UBX_RXM_RTCM = 0x0232,
//	NEOM8N_UBX_RXM_SFRBX = 0x0213,
//	NEOM8N_UBX_RXM_SVSI = 0x0220
//};
//
//// UBX Class SEC - Security Feature Messages
//enum Neom8nUBX_SEC_Id {
//	NEOM8N_UBX_SEC_UNIQID = 0x2703
//};
//
//// UBX Class TIM - Timing Messages
//enum Neom8nUBX_TIM_Id {
//	NEOM8N_UBX_TIM_DOSC = 0x0D11,
//	NEOM8N_UBX_TIM_FCHG = 0x0D16,
//	NEOM8N_UBX_TIM_HOC = 0x0D17,
//	NEOM8N_UBX_TIM_SMEAS = 0x0D13,
//	NEOM8N_UBX_TIM_SVIN = 0x0D04,
//	NEOM8N_UBX_TIM_TM2 = 0x0D03,
//	NEOM8N_UBX_TIM_TOS = 0x0D12,
//	NEOM8N_UBX_TIM_TP = 0x0D01,
//	NEOM8N_UBX_TIM_VCOCAL = 0x0D15,
//	NEOM8N_UBX_TIM_VRFY = 0x0D06
//};
//
//// UBX Class UPD - Firmware Update Messages
//enum Neom8nUBX_UPD_Id {
//	NEOM8N_UBX_UPD_SOS = 0x0914
//};


struct ubx_gps {
	UART_HandleTypeDef *huart;

	uint8_t *packet_buf;
	uint32_t packet_buf_size;	//max payload length is uint16_t
	uint32_t packet_buf_cnt;	//packet current length

	uint8_t *uart_rx_buf;
	uint32_t uart_rx_buf_size;
	uint32_t uart_rx_buf_cnt;
};

struct ubx_nav_posllh {
	uint32_t tow_ms;		//time of week
	int32_t longitude_100ndeg;
	int32_t latitude_100ndeg;
	int32_t height_ellipsoid_mm;	//height above ellipsoid
	int32_t height_msl_mm;	//height above mean sea level
	uint32_t horizontal_accuracy_mm;	//horizontal accuracy estimate
	uint32_t vertical_accuracy_mm;	//vertical accuracy estimate

	double longitude_deg;		//deivde 1e-7 raw data
	double latitude_deg;		//deivde 1e-7 raw data
};

struct ubx_nav_pvt {
	uint32_t tow_ms;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minuate;
//	uint8_t valid;


	uint32_t time_accuracy_ns;
	int32_t nano_ns;			// fraction of second, range-1e9 .. 1e9(UTC)
	uint8_t gnss_fix_type;			// 0 : no fix, 1 : dead reckoning only, 2 : 2d fix, 3 : 3d fix, 4 : gndss + dead reckoning combined, 5 : time only fix
	uint8_t flags;				// fix status flags
	uint8_t flags2;				// additional flags
	uint8_t satellite;
	int32_t longitude_100ndeg;
	int32_t latitude_100ndeg;
	int32_t height_ellipsoid_mm;
	int32_t height_msl_mm;
	uint32_t horizontal_accuracy;
	uint32_t vertical_accuracy;
	int32_t northern_velocity_mmps;		// NED
	int32_t eastern_velocity_mmps;		// NED
	int32_t down_velocity_mmps;		// NED
	int32_t groundspeed_mmps;		// 2D
	int32_t heading_of_motion_10mdeg;	//2D
	uint32_t speed_accuracy_mmps;
	uint32_t heading_accuracy_10mdeg;	// (both motionand vehicle)
	uint16_t position_dop_centi;
	uint16_t flags3;			// additional flags
	uint8_t reserved1;
	uint8_t reserved2;
	uint8_t reserved3;
	uint8_t reserved4;
	int32_t heading_of_vehicle_10mdeg;		//Heading of vehicle (2-D), this is only valid when headVehValid is set, otherwise the  output is set to the heading of motion
	int16_t magnetic_declination_cdeg;		//Magnetic declination. Only supported in ADR 4.10 and later
	uint16_t magnetic_declination_accuracy_cdeg;	//Magnetic declination accuracy. Only supported in ADR 4.10 and later.


};


extern const uint8_t UBX_CFG_PRT[];

extern const uint8_t UBX_CFG_MSG[];

extern const uint8_t UBX_CFG_RATE[];

extern const uint8_t UBX_CFG_CFG[];

bool calc_ubx_checksum(const struct ubx_gps *gps, uint8_t *ck_a, uint8_t *ck_b);
bool init_ubx_gps(struct ubx_gps *gps, UART_HandleTypeDef *huart, uint8_t *packet_buf, uint32_t packet_buf_size, uint8_t *uart_rx_buf, uint32_t uart_rx_buf_size);
bool begin_ubx_gps(struct ubx_gps *gps);
bool parse_ubx_nav_posllh(const struct ubx_gps *gps, struct ubx_nav_posllh *nav_posllh);
bool transmit_ubx_data(struct ubx_gps *gps, const uint8_t *data, uint32_t size);
void ubx_uart_rx_cplt_callback(UART_HandleTypeDef *huart, struct ubx_gps *gps);
__weak bool ubx_callback_callback();

#endif /* __NEOM8N_UBX_H__ */

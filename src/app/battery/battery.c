/*
 * battery.c
 *
 *  Created on: Nov 19, 2025
 *      Author: coder0908
 */


#include "platform/hal/platform_hal.h"
#include "services/msgbox/msgbox.h"
#include "protocols/crsf/utils/crsf_frame_queue.h"
#include "app/tranceiver/tranceiver.h"
#include "adc.h"
#include "battery.h"

#define ADC1_FULLSCALE				(4095.0f)
#define BATTERY_VOLTAGESCALE_ADC1CH4		((47.0f+10.0f)/10.0f)
#define BATTERY_VOLTAGESCALE_ADC1CH7		((47.0f+10.0f)/10.0f)
#define BATTERY_CHANNELCNT_ADC1			2

static uint32_t s_trcivr_tx_crsf_msgbox_id;

static uint32_t s_adc1_values_lsb[BATTERY_CHANNELCNT_ADC1 + 1];	// last one's for vrefint

static float s_battery_adc1_voltages_v[BATTERY_CHANNELCNT_ADC1];

bool battery_init()
{
	bool tmp = false;
	HAL_StatusTypeDef status = HAL_ERROR;

	status = HAL_ADC_Start_DMA(&hadc1, s_adc1_values_lsb, BATTERY_CHANNELCNT_ADC1+1);	//+1 for vrefint channel
	if (status != HAL_OK) {
		return false;
	}

	tmp = msgbox_get(TRCEIVER_MSGBOX_NAME_TX_CRSF, TRCEIVER_MSGBOX_NAMELEN_TX_CRSF, &s_trcivr_tx_crsf_msgbox_id);
	if (!tmp) {
		return false;
	}

	return true;
}


static void battery_read_voltage()
{
	float vrefplus_mv = (float)VREFINT_CAL_VREF * (float)(*VREFINT_CAL_ADDR) / (float)s_adc1_values_lsb[BATTERY_CHANNELCNT_ADC1];	// battery_adc1_values[BATTERY_CHANNELCNT_ADC1] = vrefint data

	s_battery_adc1_voltages_v[0] = ((float)s_adc1_values_lsb[0]/ADC1_FULLSCALE) * (vrefplus_mv/1000.0f) * BATTERY_VOLTAGESCALE_ADC1CH4;
	s_battery_adc1_voltages_v[1] = ((float)s_adc1_values_lsb[1]/ADC1_FULLSCALE) * (vrefplus_mv/1000.0f) * BATTERY_VOLTAGESCALE_ADC1CH7;

}

void battery_loop()
{
	battery_read_voltage();
	struct crsf_frame frame = {0,};

	crsf_framing_battery(&frame, (int16_t)(s_battery_adc1_voltages_v[0]*10), (int16_t)(s_battery_adc1_voltages_v[1]*10), 0, 0);
	uint8_t len = crsf_get_frame_length(&frame);
	assert(msgbox_publish(s_trcivr_tx_crsf_msgbox_id, frame.frame, len));
}



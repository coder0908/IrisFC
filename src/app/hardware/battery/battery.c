/*
 * battery.c
 *
 *  Created on: Nov 19, 2025
 *      Author: coder0908
 */


#include "platform/hal/platform_hal.h"
#include "middleware/msgbox/msgbox.h"
#include "lib/protocols/crsf/utils/crsf_frame_queue.h"
#include "app/hardware/tranceiver/tranceiver.h"
#include "adc.h"
#include "battery.h"

#define ADC1_FULLSCALE				(4095.0f)
#define ADC1_VOLTAGESCALE_CH4		((47.0f+10.0f)/10.0f)
#define ADC1_VOLTAGESCALE_CH7		((47.0f+10.0f)/10.0f)
#define ADC1_CNT_CHANNEL			2

static uint32_t s_crsf_tx_msgbox_id;

static uint32_t s_adc1_values_lsb[ADC1_CNT_CHANNEL + 1];	// last one's for vrefint

static float s_adc1_voltages_v[ADC1_CNT_CHANNEL];

bool battery_init()
{

	if (HAL_ADC_Start_DMA(&hadc1, s_adc1_values_lsb, ADC1_CNT_CHANNEL+1) != HAL_OK) {
		return false;
	}

	if (!msgbox_get(TRCIVR_MSGBOX_NAME_CRSF_TX, TRCIVR_MSGBOX_NAMELEN_CRSF_TX, &s_crsf_tx_msgbox_id)) {
		return false;
	}

	return true;
}


static void battery_read_voltage()
{
	float vrefplus_mv = (float)VREFINT_CAL_VREF * (float)(*VREFINT_CAL_ADDR) / (float)s_adc1_values_lsb[ADC1_CNT_CHANNEL];	// battery_adc1_values[ADC1_CNT_CHANNEL] = vrefint data

	s_adc1_voltages_v[0] = ((float)s_adc1_values_lsb[0]/ADC1_FULLSCALE) * (vrefplus_mv/1000.0f) * ADC1_VOLTAGESCALE_CH4;
	s_adc1_voltages_v[1] = ((float)s_adc1_values_lsb[1]/ADC1_FULLSCALE) * (vrefplus_mv/1000.0f) * ADC1_VOLTAGESCALE_CH7;

}

void battery_loop()
{
	battery_read_voltage();
	struct crsf_frame frame = {0,};


	uint16_t fc_mv = (uint16_t)(s_adc1_voltages_v[0]*1000.0f);
	uint16_t esc_mv = (uint16_t)(s_adc1_voltages_v[1]*1000.0f);

	uint16_t tmp[2] = {fc_mv, esc_mv};

	crsf_framing_voltages(&frame, 128, tmp, 2);
	uint8_t len = crsf_get_frame_length(&frame);
	msgbox_publish(s_crsf_tx_msgbox_id, frame.frame, len);



}



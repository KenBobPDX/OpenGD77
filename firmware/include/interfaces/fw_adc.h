/*
 * Copyright (C)2019 Kai Ludwig, DG4KLU
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _FW_ADC_H_
#define _FW_ADC_H_

#include "FreeRTOS.h"
#include "task.h"

#include "fsl_adc16.h"

#define CUTOFF_VOLTAGE_UPPER_HYST 64
#define CUTOFF_VOLTAGE_LOWER_HYST 62

extern volatile uint32_t adc_channel;
extern volatile uint32_t adc0_dp0;
extern volatile uint32_t adc0_dp1;
extern volatile uint32_t adc0_dp2;
extern volatile uint32_t adc0_dp3;

void trigger_adc();
void adc_init();
void ADC0_IRQHandler(void);
int get_battery_voltage();

#endif /* _FW_ADC_H_ */

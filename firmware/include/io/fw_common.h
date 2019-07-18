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

#ifndef _FW_COMMON_H_
#define _FW_COMMON_H_

#include "fsl_gpio.h"
#include "fsl_port.h"

extern gpio_pin_config_t pin_config_input;
extern gpio_pin_config_t pin_config_output;

// Power On/Off logic
#define Port_Keep_Power_On  PORTE
#define GPIO_Keep_Power_On  GPIOE
#define Pin_Keep_Power_On	26
#define Port_Power_Switch   PORTA
#define GPIO_Power_Switch 	GPIOA
#define Pin_Power_Switch	13

// Other connections
// OUT/OFF A17 - RF_ant_switch
// OUT/OFF B0  - speaker_mute
// OUT/ON  C5  - RX_audio_mux
// OUT/OFF C6  - TX_audio_mux
// OUT/OFF C13 - VHF_RX_amp_power
// OUT/OFF C15 - UHF_RX_amp_power
// OUT/OFF E2  - UHF_TX_amp_power
// OUT/OFF E3  - VHF_TX_amp_power
#define Port_RF_ant_switch    PORTA
#define GPIO_RF_ant_switch    GPIOA
#define Pin_RF_ant_switch     17
#define Port_speaker_mute     PORTB
#define GPIO_speaker_mute     GPIOB
#define Pin_speaker_mute      0
#define Port_RX_audio_mux     PORTC
#define GPIO_RX_audio_mux     GPIOC
#define Pin_RX_audio_mux      5
#define Port_TX_audio_mux     PORTC
#define GPIO_TX_audio_mux     GPIOC
#define Pin_TX_audio_mux      6
#define Port_VHF_RX_amp_power PORTC
#define GPIO_VHF_RX_amp_power GPIOC
#define Pin_VHF_RX_amp_power  13
#define Port_UHF_RX_amp_power PORTC
#define GPIO_UHF_RX_amp_power GPIOC
#define Pin_UHF_RX_amp_power  15
#define Port_UHF_TX_amp_power PORTE
#define GPIO_UHF_TX_amp_power GPIOE
#define Pin_UHF_TX_amp_power  2
#define Port_VHF_TX_amp_power PORTE
#define GPIO_VHF_TX_amp_power GPIOE
#define Pin_VHF_TX_amp_power  3

void fw_init_common();

#endif /* _FW_COMMON_H_ */

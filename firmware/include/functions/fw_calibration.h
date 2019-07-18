/*
 * Copyright (C)2019 Roger Clark. VK3KYY / G4KYF
 * 				and  Kai Ludwig, DG4KLU
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
#ifndef _FW_CALIBRATION_H_
#define _FW_CALIBRATION_H_

#include "fw_common.h"
#include "fw_SPI_Flash.h"

typedef struct calibrationStruct
{
	int masterOscillator;
	uint8_t powerLevels[16];
} calibrationStruct_t;

extern calibrationStruct_t calibrationVHF;
extern calibrationStruct_t calibrationUHF;

#define EXT_DACDATA_shift 0x0008F05D
#define EXT_twopoint_mod  0x0008F008
#define EXT_Q_MOD2_offset 0x0008F00A
#define EXT_phase_reduce  0x0008F055

#define EXT_pga_gain      0x0008F065
#define EXT_voice_gain_tx 0x0008F066
#define EXT_gain_tx       0x0008F067
#define EXT_padrv_ibit    0x0008F064

#define EXT_xmitter_dev_wideband   0x0008F068
#define EXT_xmitter_dev_narrowband 0x0008F06A

#define EXT_dac_vgain_analog 0x0008F06C
#define EXT_volume_analog    0x0008F06D

#define EXT_noise1_th_wideband   0x0008F047
#define EXT_noise2_th_wideband   0x0008F049
#define EXT_rssi3_th_wideband    0x0008F04b
#define EXT_noise1_th_narrowband 0x0008F04d
#define EXT_noise2_th_narrowband 0x0008F04f
#define EXT_rssi3_th_narrowband  0x0008F051

#define EXT_squelch_th 0x0008F03f

void read_val_DACDATA_shift(int offset, uint8_t* val_shift);
void read_val_twopoint_mod(int offset, uint8_t* val_0x47, uint8_t* val_0x48);
void read_val_Q_MOD2_offset(int offset, uint8_t* val_0x04);
void read_val_phase_reduce(int offset, uint8_t* val_0x46);

void read_val_pga_gain(int offset, uint8_t* value);
void read_val_voice_gain_tx(int offset, uint8_t* value);
void read_val_gain_tx(int offset, uint8_t* value);
void read_val_padrv_ibit(int offset, uint8_t* value);

void read_val_xmitter_dev_wideband(int offset, uint16_t* value);
void read_val_xmitter_dev_narrowband(int offset, uint16_t* value);

void read_val_dac_vgain_analog(int offset, uint8_t* value);
void read_val_volume_analog(int offset, uint8_t* value);

void read_val_noise1_th_wideband(int offset, uint16_t* value);
void read_val_noise2_th_wideband(int offset, uint16_t* value);
void read_val_rssi3_th_wideband(int offset, uint16_t* value);
void read_val_noise1_th_narrowband(int offset, uint16_t* value);
void read_val_noise2_th_narrowband(int offset, uint16_t* value);
void read_val_rssi3_th_narrowband(int offset, uint16_t* value);

void read_val_squelch_th(int offset, uint16_t* value);

#endif

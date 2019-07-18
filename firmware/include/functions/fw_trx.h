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

#ifndef _FW_TRX_H_
#define _FW_TRX_H_

#include "fw_sound.h"
#include "fw_i2c.h"

extern const int RADIO_VHF_MIN;
extern const int RADIO_VHF_MAX;
extern const int RADIO_UHF_MIN;
extern const int RADIO_UHF_MAX;

enum RADIO_MODE { RADIO_MODE_NONE,RADIO_MODE_ANALOG,RADIO_MODE_DIGITAL};
enum DMR_ADMIT_CRITERIA { ADMIT_CRITERIA_ALWAYS,ADMIT_CRITERIA_CHANNEL_FREE,ADMIT_CRITERIA_COLOR_CODE};


extern bool open_squelch;
extern bool HR_C6000_datalogging;

extern bool trxIsTransmitting;
extern uint32_t trxTalkGroup;
extern uint32_t trxDMRID;
extern int trx_measure_count;
extern int txstopdelay;

void trx_check_analog_squelch();
int	trxGetMode();
int	trxGetFrequency();
void trxSetMode(int mode);
void trxSetFrequency(int frequency);
void trx_setRX();
void trx_setTX();
void trx_deactivateTX();
void trx_activateTX();
void trxSetPower(uint32_t powerVal);
uint16_t trxGetPower();
void trxSetBandWidth(bool bandWidthis25kHz);
void trxUpdateC6000Calibration();
void trxUpdateAT1846SCalibration();
void trxSetDMRColourCode(int colourCode);
int trxGetDMRColourCode();
bool trxCheckFrequencyIsVHF(int frequency);
bool trxCheckFrequencyIsUHF(int frequency);
bool trxCheckFrequency(int tmp_frequency);
void trxSetTxCTCSS(int toneFreqX10);
void trxSetRxCTCSS(int toneFreqX10);
bool trxCheckCTCSSFlag();

#endif /* _FW_TRX_H_ */

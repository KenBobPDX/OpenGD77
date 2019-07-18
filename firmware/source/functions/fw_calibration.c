/*
 * Copyright (C)2019 	Roger Clark, VK3KYY / G4KYF
 * 				and 	Kai Ludwig, DG4KLU
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

#include "fw_calibration.h"

calibrationStruct_t calibrationVHF;
calibrationStruct_t calibrationUHF;

void read_val_DACDATA_shift(int offset, uint8_t* val_shift)
{
	uint8_t buffer[1];
	SPI_Flash_read(EXT_DACDATA_shift+offset,buffer,1);

	*val_shift=buffer[0]+1;
	if (*val_shift>31)
	{
		*val_shift=31;
	}
	*val_shift|=0x80;
}

void read_val_twopoint_mod(int offset, uint8_t* val_0x47, uint8_t* val_0x48)
{
	uint8_t buffer[2];
	SPI_Flash_read(EXT_twopoint_mod+offset,buffer,2);
	*val_0x47=buffer[0];
	*val_0x48=buffer[1];
}

void read_val_Q_MOD2_offset(int offset, uint8_t* val_0x04)
{
	uint8_t buffer[1];
	SPI_Flash_read(EXT_Q_MOD2_offset+offset,buffer,1);
	*val_0x04=buffer[0];
}

void read_val_phase_reduce(int offset, uint8_t* val_0x46)
{
	uint8_t buffer[1];
	SPI_Flash_read(EXT_phase_reduce+offset,buffer,1);
	*val_0x46=buffer[0];
}

void read_val_pga_gain(int offset, uint8_t* value)
{
	uint8_t buffer[1];
	SPI_Flash_read(EXT_pga_gain+offset,buffer,1);
	*value=buffer[0] & 0x1f;
}

void read_val_voice_gain_tx(int offset, uint8_t* value)
{
	uint8_t buffer[1];
	SPI_Flash_read(EXT_voice_gain_tx+offset,buffer,1);
	*value=buffer[0] & 0x7f;
}

void read_val_gain_tx(int offset, uint8_t* value)
{
	uint8_t buffer[1];
	SPI_Flash_read(EXT_gain_tx+offset,buffer,1);
	*value=buffer[0] & 0x0f;
}

void read_val_padrv_ibit(int offset, uint8_t* value)
{
	uint8_t buffer[1];
	SPI_Flash_read(EXT_padrv_ibit+offset,buffer,1);
	*value=buffer[0] & 0x0f;
}

void read_val_xmitter_dev_wideband(int offset, uint16_t* value)
{
	uint8_t buffer[2];
	SPI_Flash_read(EXT_xmitter_dev_wideband+offset,buffer,2);
	*value=buffer[0] + ((buffer[1] & 0x03) << 8);
}

void read_val_xmitter_dev_narrowband(int offset, uint16_t* value)
{
	uint8_t buffer[2];
	SPI_Flash_read(EXT_xmitter_dev_narrowband+offset,buffer,2);
	*value=buffer[0] + ((buffer[1] & 0x03) << 8);
}

void read_val_dac_vgain_analog(int offset, uint8_t* value)
{
	uint8_t buffer[1];
	SPI_Flash_read(EXT_dac_vgain_analog+offset,buffer,1);
	*value=buffer[0] & 0x0f;
}

void read_val_volume_analog(int offset, uint8_t* value)
{
	uint8_t buffer[1];
	SPI_Flash_read(EXT_volume_analog+offset,buffer,1);
	*value=buffer[0] & 0x0f;
}

void read_val_noise1_th_wideband(int offset, uint16_t* value)
{
	uint8_t buffer[2];
	SPI_Flash_read(EXT_noise1_th_wideband+offset,buffer,2);
	*value=((buffer[0] & 0x7f) << 7) + ((buffer[1] & 0x7f) << 0);
}

void read_val_noise2_th_wideband(int offset, uint16_t* value)
{
	uint8_t buffer[2];
	SPI_Flash_read(EXT_noise2_th_wideband+offset,buffer,2);
	*value=((buffer[0] & 0x7f) << 7) + ((buffer[1] & 0x7f) << 0);
}

void read_val_rssi3_th_wideband(int offset, uint16_t* value)
{
	uint8_t buffer[2];
	SPI_Flash_read(EXT_rssi3_th_wideband+offset,buffer,2);
	*value=((buffer[0] & 0x7f) << 7) + ((buffer[1] & 0x7f) << 0);
}

void read_val_noise1_th_narrowband(int offset, uint16_t* value)
{
	uint8_t buffer[2];
	SPI_Flash_read(EXT_noise1_th_narrowband+offset,buffer,2);
	*value=((buffer[0] & 0x7f) << 7) + ((buffer[1] & 0x7f) << 0);
}

void read_val_noise2_th_narrowband(int offset, uint16_t* value)
{
	uint8_t buffer[2];
	SPI_Flash_read(EXT_noise2_th_narrowband+offset,buffer,2);
	*value=((buffer[0] & 0x7f) << 7) + ((buffer[1] & 0x7f) << 0);
}

void read_val_rssi3_th_narrowband(int offset, uint16_t* value)
{
	uint8_t buffer[2];
	SPI_Flash_read(EXT_rssi3_th_narrowband+offset,buffer,2);
	*value=((buffer[0] & 0x7f) << 7) + ((buffer[1] & 0x7f) << 0);
}

void read_val_squelch_th(int offset, uint16_t* value)
{
	uint8_t buffer[1];
	SPI_Flash_read(EXT_squelch_th+offset,buffer,1);
	uint8_t v1 = buffer[0]-3;
	uint8_t v2 = buffer[0]-6;
	if ( v1 >= 127 || v2 >= 127 || v1 < v2 )
	{
	  v1 = 24;
	  v2 = 21;
	}
	*value=(v1 << 7) + (v2 << 0);
}

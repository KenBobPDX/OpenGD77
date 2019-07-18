/*
 * Copyright (C)2019 Roger Clark. VK3KYY / G4KYF
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
#include <fw_codeplug.h>
#include "fw_EEPROM.h"
#include "fw_SPI_Flash.h"

const int CODEPLUG_ADDR_EX_ZONE_BASIC = 0x8000;
const int CODEPLUG_ADDR_EX_ZONE_INUSE_PACKED_DATA =  0x8010;
const int CODEPLUG_ADDR_EX_ZONE_INUSE_PACKED_DATA_SIZE =  32;
const int CODEPLUG_ADDR_EX_ZONE_LIST =  0x8030;
const int CODEPLUG_ZONE_DATA_SIZE = 48;
const int CODEPLUG_ZONE_MAX_COUNT = 250;
const int CODEPLUG_CHANNEL_DATA_SIZE = 56;
const int CODEPLUG_ADDR_CHANNEL_EEPROM = 0x3790;
const int CODEPLUG_ADDR_CHANNEL_FLASH = 0x7B1C0;

const int CODEPLUG_ADDR_RX_GROUP = 0x8D6A0;//
const int CODEPLUG_RX_GROUP_LEN = 0x50;

const int CODEPLUG_ADDR_CONTACTS = 0x87620;
const int CODEPLUG_CONTACTS_LEN = 0x18;

const int CODEPLUG_ADDR_USER_DMRID = 0x00E8;
const int CODEPLUG_ADDR_USER_CALLSIGN = 0x00E0;

const int CODEPLUG_ADDR_BOOT_LINE1 = 0x7540;
const int CODEPLUG_ADDR_BOOT_LINE2 = 0x7550;
const int CODEPLUG_ADDR_VFO_A_CHANNEL = 0x7590;

uint32_t byteSwap32(uint32_t n)
{
    return ((((n)&0x000000FFU) << 24U) | (((n)&0x0000FF00U) << 8U) | (((n)&0x00FF0000U) >> 8U) | (((n)&0xFF000000U) >> 24U));// from usb_misc.h
}

// BCD encoding to integer conversion
uint32_t bcd2int(uint32_t i)
{
    int result = 0;
    int multiplier = 1;
    while (i)
    {
        result +=  (i &0x0f)* multiplier;
        multiplier *=10;
        i = i >> 4;
    }
    return result;
}

void codeplugUtilConvertBufToString(char *inBuf,char *outBuf,int len)
{
	for(int i=0;i<len;i++)
	{
		if (inBuf[i]==0xff)
		{
			inBuf[i]=0;
		}
		outBuf[i]=inBuf[i];
	}
	outBuf[len]=0;
	return;
}

int codeplugZonesGetCount()
{
	uint8_t buf[CODEPLUG_ADDR_EX_ZONE_INUSE_PACKED_DATA_SIZE];
	int numZones = 0;

	EEPROM_Read(CODEPLUG_ADDR_EX_ZONE_INUSE_PACKED_DATA, (uint8_t*)&buf, CODEPLUG_ADDR_EX_ZONE_INUSE_PACKED_DATA_SIZE);
	for(int i=0;i<CODEPLUG_ADDR_EX_ZONE_INUSE_PACKED_DATA_SIZE;i++)
	{
		numZones += __builtin_popcount(buf[i]);
	}
	return numZones;
}

void codeplugZoneGetDataForIndex(int index,struct_codeplugZone_t *returnBuf)
{
	// IMPORTANT. read size is different from the size of the data, because I added a extra property to the struct to hold the number of channels in the zone.
	EEPROM_Read(CODEPLUG_ADDR_EX_ZONE_LIST + (index * CODEPLUG_ZONE_DATA_SIZE), (uint8_t*)returnBuf, sizeof(struct_codeplugZone_t));
	for(int i=0;i<32;i++)
	{
		// Empty channels seem to be filled with zeros
		if (returnBuf->channels[i] == 0)
		{
			returnBuf->NOT_IN_MEMORY_numChannelsInZone = i;
			return;
		}
	}
	returnBuf->NOT_IN_MEMORY_numChannelsInZone=32;
}

void codeplugChannelGetDataForIndex(int index, struct_codeplugChannel_t *channelBuf)
{
	// lower 128 channels are in EEPROM. Remaining channels are in Flash ! (What a mess...)
	index--; // I think the channel index numbers start from 1 not zero.
	if (index<128)
	{
		EEPROM_Read(CODEPLUG_ADDR_CHANNEL_EEPROM + index*sizeof(struct_codeplugChannel_t),(uint8_t *)channelBuf,sizeof(struct_codeplugChannel_t));
	}
	else
	{
		int flashReadPos = CODEPLUG_ADDR_CHANNEL_FLASH;

		index -= 128;// First 128 channels are in the EEPOM, so subtract 128 from the number when looking in the Flash

		// Every 128 bytes there seem to be 16 bytes gaps. I don't know why,bits since 16*8 = 128 bits, its likely they are flag bytes to indicate which channel in the next block are in use
		flashReadPos += 16 * (index/128);// we just need to skip over that these flag bits when calculating the position of the channel data in memory

		SPI_Flash_read(flashReadPos + index*sizeof(struct_codeplugChannel_t),(uint8_t *)channelBuf,sizeof(struct_codeplugChannel_t));
	}

	channelBuf->chMode = (channelBuf->chMode==0)?RADIO_MODE_ANALOG:RADIO_MODE_DIGITAL;
	// Convert the the legacy codeplug tx and rx freq values into normal integers
	channelBuf->txFreq = bcd2int(channelBuf->txFreq)/10;
	channelBuf->rxFreq = bcd2int(channelBuf->rxFreq)/10;
	if (channelBuf->rxTone != 0xffff)
	{
		channelBuf->rxTone = bcd2int(channelBuf->rxTone);
	}
	if (channelBuf->txTone != 0xffff)
	{
		channelBuf->txTone = bcd2int(channelBuf->txTone);
	}
}


void codeplugRxGroupGetDataForIndex(int index, struct_codeplugRxGroup_t *rxGroupBuf)
{
	int i=0;
	index--; //Index numbers start from 1 not zero
// Not our struct contains an extra property to hold the number of TGs in the group
	SPI_Flash_read(CODEPLUG_ADDR_RX_GROUP + index*(sizeof(struct_codeplugRxGroup_t) - sizeof(int)),(uint8_t *)rxGroupBuf,sizeof(struct_codeplugRxGroup_t) - sizeof(int));
	for(i=0;i<32;i++)
	{
		// Empty groups seem to be filled with zeros
		if (rxGroupBuf->contacts[i] == 0)
		{
			break;
		}
	}
	rxGroupBuf->NOT_IN_MEMORY_numTGsInGroup = i;
}

void codeplugContactGetDataForIndex(int index, struct_codeplugContact_t *contact)
{
	index--;
	SPI_Flash_read(CODEPLUG_ADDR_CONTACTS + index*sizeof(struct_codeplugContact_t),(uint8_t *)contact,sizeof(struct_codeplugContact_t));
	contact->tgNumber = bcd2int(byteSwap32(contact->tgNumber));
}

int codeplugGetUserDMRID()
{
	int dmrId;
	EEPROM_Read(CODEPLUG_ADDR_USER_DMRID,(uint8_t *)&dmrId,4);
	return bcd2int(byteSwap32(dmrId));
}

// Max length the user can enter is 8. Hence buf must be 16 chars to allow for the termination
void codeplugGetRadioName(char *buf)
{
	memset(buf,0,9);
	EEPROM_Read(CODEPLUG_ADDR_USER_CALLSIGN,(uint8_t *)buf,8);
	codeplugUtilConvertBufToString(buf,buf,8);
}

// Max length the user can enter is 15. Hence buf must be 16 chars to allow for the termination
void codeplugGetBootItemTexts(char *line1, char *line2)
{
	memset(line1,0,16);
	memset(line2,0,16);

	EEPROM_Read(CODEPLUG_ADDR_BOOT_LINE1,(uint8_t *)line1,15);
	codeplugUtilConvertBufToString(line1,line1,15);
	EEPROM_Read(CODEPLUG_ADDR_BOOT_LINE2,(uint8_t *)line2,15);
	codeplugUtilConvertBufToString(line2,line2,15);
}


void codeplugVFO_A_ChannelData(struct_codeplugChannel_t *vfoBuf)
{
	EEPROM_Read(CODEPLUG_ADDR_VFO_A_CHANNEL,(uint8_t *)vfoBuf,sizeof(struct_codeplugChannel_t));

	// Convert the the legacy codeplug tx and rx freq values into normal integers
	vfoBuf->chMode = (vfoBuf->chMode==0)?RADIO_MODE_ANALOG:RADIO_MODE_DIGITAL;
	vfoBuf->txFreq = bcd2int(vfoBuf->txFreq)/10;
	vfoBuf->rxFreq = bcd2int(vfoBuf->rxFreq)/10;
}

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
#include "menu/menuSystem.h"
#include "menu/menuUtilityQSOData.h"
#include "fw_trx.h"
#include "fw_codeplug.h"
#include "fw_settings.h"


static void updateScreen();
static void handleEvent(int buttons, int keys, int events);
static void loadChannelData(bool useChannelDataInMemory);
static struct_codeplugZone_t currentZone;
static struct_codeplugRxGroup_t rxGroupData;
static struct_codeplugContact_t contactData;
static int currentIndexInTRxGroup=0;

int menuChannelMode(int buttons, int keys, int events, bool isFirstRun)
{
	if (isFirstRun)
	{
		nonVolatileSettings.initialMenuNumber = MENU_CHANNEL_MODE;// This menu.
		codeplugZoneGetDataForIndex(nonVolatileSettings.currentZone,&currentZone);
		if (channelScreenChannelData.rxFreq != 0)
		{
			loadChannelData(true);
		}
		else
		{
			loadChannelData(false);
		}
		currentChannelData = &channelScreenChannelData;
		menuDisplayQSODataState = QSO_DISPLAY_DEFAULT_SCREEN;
		updateScreen();
	}
	else
	{
		if (events==0)
		{
			// is there an incoming DMR signal
			if (menuDisplayQSODataState != QSO_DISPLAY_IDLE)
			{
				updateScreen();
			}
		}
		else
		{
			handleEvent(buttons, keys, events);
		}
	}
	return 0;
}
uint16_t byteSwap16(uint16_t in)
{
	return ((in &0xff << 8) | (in >>8));
}

static void loadChannelData(bool useChannelDataInMemory)
{
	if (!useChannelDataInMemory)
	{
		codeplugChannelGetDataForIndex(currentZone.channels[nonVolatileSettings.currentChannelIndexInZone],&channelScreenChannelData);
	}
	trxSetFrequency(channelScreenChannelData.rxFreq);
	trxSetMode(channelScreenChannelData.chMode);
	if (channelScreenChannelData.chMode == RADIO_MODE_ANALOG)
	{
		trxSetBandWidth((channelScreenChannelData.flag4 & 0x02)  == 0x02);// set the bandwidth after the mode, because mode probably sets it back to 12.5kHz (note this needs to be tidied up ;-) )
	}
	trxSetDMRColourCode(channelScreenChannelData.rxColor);
	trxSetPower(nonVolatileSettings.txPower);
	trxSetTxCTCSS(channelScreenChannelData.txTone);
	trxSetRxCTCSS(channelScreenChannelData.rxTone);
	codeplugRxGroupGetDataForIndex(channelScreenChannelData.rxGroupList,&rxGroupData);
	codeplugContactGetDataForIndex(rxGroupData.contacts[currentIndexInTRxGroup],&contactData);
	if (nonVolatileSettings.overrideTG == 0)
	{
		trxTalkGroup = contactData.tgNumber;
	}
	else
	{
		trxTalkGroup = nonVolatileSettings.overrideTG;
	}
}

static void updateScreen()
{
	char nameBuf[17];

	UC1701_clearBuf();

	menuUtilityRenderHeader();

	switch(menuDisplayQSODataState)
	{
		case QSO_DISPLAY_DEFAULT_SCREEN:
			codeplugUtilConvertBufToString(channelScreenChannelData.name,nameBuf,16);
			UC1701_printCentered(32, (char *)nameBuf,UC1701_FONT_GD77_8x16);

			if (trxGetMode() == RADIO_MODE_DIGITAL)
			{
				if (nonVolatileSettings.overrideTG != 0)
				{
					sprintf(nameBuf,"TG %d",trxTalkGroup);
				}
				else
				{
					codeplugUtilConvertBufToString(contactData.name,nameBuf,16);
				}
				UC1701_printCentered(16, (char *)nameBuf,UC1701_FONT_GD77_8x16);
			}

			displayLightTrigger();
			UC1701_render();
			break;

		case QSO_DISPLAY_CALLER_DATA:
			menuUtilityRenderQSOData();
			displayLightTrigger();
			UC1701_render();
			break;
	}

	menuDisplayQSODataState = QSO_DISPLAY_IDLE;
}

static void handleEvent(int buttons, int keys, int events)
{
	if (events & 0x02)
	{
		if (buttons & BUTTON_ORANGE)
		{
			menuSystemPushNewMenu(MENU_ZONE_LIST);
			return;
		}
	}

	if ((keys & KEY_GREEN)!=0)
	{
		if (buttons & BUTTON_SK2 )
		{
			menuSystemPushNewMenu(MENU_CHANNEL_DETAILS);
		}
		else
		{
			menuSystemPushNewMenu(MENU_MAIN_MENU);
		}
		return;
	}
	else if ((keys & KEY_HASH)!=0)
	{
		menuSystemPushNewMenu(MENU_NUMERICAL_ENTRY);
		return;
	}
	else if ((keys & KEY_RED)!=0)
	{
		menuSystemSetCurrentMenu(MENU_VFO_MODE);
		return;
	}


	if ((keys & KEY_RIGHT)!=0)
	{
		currentIndexInTRxGroup++;
		if (currentIndexInTRxGroup > (rxGroupData.NOT_IN_MEMORY_numTGsInGroup -1))
		{
			currentIndexInTRxGroup =  0;
		}
		codeplugContactGetDataForIndex(rxGroupData.contacts[currentIndexInTRxGroup],&contactData);

		nonVolatileSettings.overrideTG = 0;// setting the override TG to 0 indicates the TG is not overridden
		trxTalkGroup = contactData.tgNumber;

		menuDisplayQSODataState = QSO_DISPLAY_DEFAULT_SCREEN;
		updateScreen();
	}
	else if ((keys & KEY_LEFT)!=0)
	{
		// To Do change TG in on same channel freq
		currentIndexInTRxGroup--;
		if (currentIndexInTRxGroup < 0)
		{
			currentIndexInTRxGroup =  rxGroupData.NOT_IN_MEMORY_numTGsInGroup - 1;
		}

		codeplugContactGetDataForIndex(rxGroupData.contacts[currentIndexInTRxGroup],&contactData);
		nonVolatileSettings.overrideTG = 0;// setting the override TG to 0 indicates the TG is not overridden
		trxTalkGroup = contactData.tgNumber;

		menuDisplayQSODataState = QSO_DISPLAY_DEFAULT_SCREEN;
		updateScreen();
	}
	else if ((keys & KEY_STAR)!=0)
	{
		if (trxGetMode() == RADIO_MODE_ANALOG)
		{
			trxSetMode(RADIO_MODE_DIGITAL);
		}
		else
		{
			trxSetMode(RADIO_MODE_ANALOG);
			trxSetTxCTCSS(currentChannelData->rxTone);
		}
		menuDisplayQSODataState = QSO_DISPLAY_DEFAULT_SCREEN;
		updateScreen();
	}
	else if ((keys & KEY_DOWN)!=0)
	{
		nonVolatileSettings.currentChannelIndexInZone--;
		if (nonVolatileSettings.currentChannelIndexInZone < 0)
		{
			nonVolatileSettings.currentChannelIndexInZone =  currentZone.NOT_IN_MEMORY_numChannelsInZone - 1;
		}
		loadChannelData(false);
		menuDisplayQSODataState = QSO_DISPLAY_DEFAULT_SCREEN;
		updateScreen();
	}
	else if ((keys & KEY_UP)!=0)
	{
		nonVolatileSettings.currentChannelIndexInZone++;
		if (nonVolatileSettings.currentChannelIndexInZone > currentZone.NOT_IN_MEMORY_numChannelsInZone - 1)
		{
			nonVolatileSettings.currentChannelIndexInZone = 0;
		}
		loadChannelData(false);
		menuDisplayQSODataState = QSO_DISPLAY_DEFAULT_SCREEN;
		updateScreen();
	}
}

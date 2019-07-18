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
#include "UC1701.h"
#include "fw_settings.h"

static void updateScreen();
static void handleEvent(int buttons, int keys, int events);

static uint8_t originalBrightness;
static uint8_t contrast;
static uint8_t inverseVideo;
static int8_t 	backLightTimeout;// used int_8 to save space

const int BACKLIGHT_MAX_TIMEOUT = 30;
const int CONTRAST_MAX_VALUE = 30;// Maximum value which still seems to be readable
const int CONTRAST_MIN_VALUE = 12;// Minimum value which still seems to be readable

int menuDisplayOptions(int buttons, int keys, int events, bool isFirstRun)
{
	if (isFirstRun)
	{
		gMenusCurrentItemIndex=0;
		originalBrightness = nonVolatileSettings.displayBacklightPercentage;
		contrast = nonVolatileSettings.displayContrast;
		inverseVideo = nonVolatileSettings.displayInverseVideo;
		backLightTimeout = 	nonVolatileSettings.backLightTimeout;
		updateScreen();
	}
	else
	{
		if (events!=0 && keys!=0)
		{
			handleEvent(buttons, keys, events);
		}
	}
	return 0;
}

static void updateScreen()
{
	char buf[20];
	UC1701_clearBuf();
	UC1701_printCentered(0, "Display options",UC1701_FONT_GD77_8x16);

	if (gMenusCurrentItemIndex==0)
	{
		UC1701_fillRect(0,16,128,16,false);
	}
	sprintf(buf,"Brightness %d%%",nonVolatileSettings.displayBacklightPercentage);
	UC1701_printCore(0,16,buf,UC1701_FONT_GD77_8x16,0,(gMenusCurrentItemIndex==0));

	if (gMenusCurrentItemIndex==1)
	{
		UC1701_fillRect(0,32,128,16,false);
	}
	sprintf(buf,"Contrast %d",contrast);
	UC1701_printCore(0,32,buf,UC1701_FONT_GD77_8x16,0,(gMenusCurrentItemIndex==1));

	if (gMenusCurrentItemIndex==2)
	{
		UC1701_fillRect(0,48,128,16,false);
	}
	sprintf(buf,"Timeout %d",backLightTimeout);
	UC1701_printCore(0,48,buf,UC1701_FONT_GD77_8x16,0,(gMenusCurrentItemIndex==2));

	UC1701_render();
	displayLightTrigger();
}

static void handleEvent(int buttons, int keys, int events)
{
	if ((keys & KEY_DOWN)!=0 && gMenusEndIndex!=0)
	{
		gMenusCurrentItemIndex++;
		if (gMenusCurrentItemIndex > 2)
		{
			gMenusCurrentItemIndex = 0;
		}
	}
	else if ((keys & KEY_UP)!=0)
	{
		gMenusCurrentItemIndex--;
		if (gMenusCurrentItemIndex < 0)
		{
			gMenusCurrentItemIndex=2;
		}
	}
	else if ((keys & KEY_RIGHT)!=0)
	{
		switch(gMenusCurrentItemIndex)
		{
			case 0:
				if (nonVolatileSettings.displayBacklightPercentage<10)
				{
					nonVolatileSettings.displayBacklightPercentage += 1;
				}
				else
				{
					nonVolatileSettings.displayBacklightPercentage += 10;
				}

				if (nonVolatileSettings.displayBacklightPercentage>100)
				{
					nonVolatileSettings.displayBacklightPercentage=100;
				}
				break;
			case 1:
				contrast++;
				if (contrast > CONTRAST_MAX_VALUE)
				{
					contrast = CONTRAST_MAX_VALUE;
				}
				UC1701_setContrast(contrast);
				break;
			case 2:
				backLightTimeout += 5;
				if (backLightTimeout > BACKLIGHT_MAX_TIMEOUT)
				{
					backLightTimeout=0;
				}
				break;
		}
	}
	else if ((keys & KEY_LEFT)!=0)
	{
		switch(gMenusCurrentItemIndex)
		{
			case 0:
				if (nonVolatileSettings.displayBacklightPercentage <= 10)
				{
					nonVolatileSettings.displayBacklightPercentage -= 1;
				}
				else
				{
					nonVolatileSettings.displayBacklightPercentage -= 10;
				}

				if (nonVolatileSettings.displayBacklightPercentage<0)
				{
					nonVolatileSettings.displayBacklightPercentage=0;
				}
				break;
			case 1:
				contrast--;
				if (contrast < CONTRAST_MIN_VALUE)
				{
					contrast = CONTRAST_MIN_VALUE;
				}
				UC1701_setContrast(contrast);
				break;
			case 2:
				backLightTimeout -= 5;
				if (backLightTimeout < 0)
				{
					backLightTimeout = BACKLIGHT_MAX_TIMEOUT;
				}
				break;
		}
	}
	else if ((keys & KEY_GREEN)!=0)
	{
		nonVolatileSettings.displayInverseVideo = inverseVideo;
		nonVolatileSettings.displayContrast = contrast;
		nonVolatileSettings.backLightTimeout = backLightTimeout;
		menuSystemPopAllAndDisplayRootMenu();
	}
	else if ((keys & KEY_RED)!=0)
	{
		if (nonVolatileSettings.displayContrast != contrast || nonVolatileSettings.displayInverseVideo != inverseVideo)
		{
			UC1701_begin(nonVolatileSettings.displayInverseVideo);
		}
		nonVolatileSettings.displayBacklightPercentage = originalBrightness;

		menuSystemPopPreviousMenu();
		return;
	}
	updateScreen();
}

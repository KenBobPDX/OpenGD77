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
#include "fw_settings.h"


static void updateScreen();
static void handleEvent(int buttons, int keys, int events);
static const int NUM_MENUS=5;
static bool	doFactoryReset;
static const int MAX_SAFE_POWER = 3000;// Note 3000 gives about 5.5W on 144Mhz on one of Roger's radios.

int menuUtilities(int buttons, int keys, int events, bool isFirstRun)
{
	if (isFirstRun)
	{
		gMenusCurrentItemIndex=0;
		doFactoryReset=false;
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
	int mNum=0;
	char buf[17];
	UC1701_clearBuf();
	UC1701_printCentered(0, "Utilities",UC1701_FONT_GD77_8x16);

	// Can only display 3 of the options at a time menu at -1, 0 and +1
	for(int i=-1;i<=1;i++)
	{
		mNum = gMenusCurrentItemIndex+i;
		if (mNum<0)
		{
			mNum = NUM_MENUS + mNum;
		}
		if (mNum >= NUM_MENUS)
		{
			mNum = mNum - NUM_MENUS;
		}

		switch(mNum)
		{
		case 0:
			sprintf(buf,"Power %d",trxGetPower());
			break;
		case 1:
			if (open_squelch)
			{
				strcpy(buf,"SQ:OFF");
			}
			else
			{
				strcpy(buf,"SQ:ON");
			}
			break;
		case 2:
			if (HR_C6000_datalogging)
			{
				strcpy(buf,"LOG:ON");
			}
			else
			{
				strcpy(buf,"LOG:OFF");
			}
			break;
		case 3:
			if (doFactoryReset==true)
			{
				strcpy(buf,"Fact Reset:YES");
			}
			else
			{
				strcpy(buf,"Fact Reset:NO");
			}
			break;
		case 4:
			if (nonVolatileSettings.useCalibration!=0)
			{
				strcpy(buf,"Calibration:ON");
			}
			else
			{
				strcpy(buf,"Calibration:OFF");
			}

			break;
		}
		if (gMenusCurrentItemIndex==mNum)
		{
			UC1701_fillRect(0,(i+2)*16,128,16,false);
		}

		UC1701_printCore(0,(i+2)*16,buf,UC1701_FONT_GD77_8x16,0,(gMenusCurrentItemIndex==mNum));
	}

	UC1701_render();
	displayLightTrigger();
}

static void handleEvent(int buttons, int keys, int events)
{
	int tmpPower;
	if ((keys & KEY_DOWN)!=0 && gMenusEndIndex!=0)
	{
		gMenusCurrentItemIndex++;
		if (gMenusCurrentItemIndex>=NUM_MENUS)
		{
			gMenusCurrentItemIndex=0;
		}
	}
	else if ((keys & KEY_UP)!=0)
	{
		gMenusCurrentItemIndex--;
		if (gMenusCurrentItemIndex<0)
		{
			gMenusCurrentItemIndex=NUM_MENUS-1;
		}
	}
	else if ((keys & KEY_RIGHT)!=0)
	{
		switch(gMenusCurrentItemIndex)
		{
			case 1:
				open_squelch = 1;
				break;
			case 2:
				HR_C6000_datalogging = 1;
				break;
			case 0:
				tmpPower = trxGetPower() + 100;
				if (tmpPower<=MAX_SAFE_POWER)
				{
					trxSetPower(tmpPower);
				}
				break;
			case 3:
				doFactoryReset = true;
				break;
			case 4:
				nonVolatileSettings.useCalibration=0x01;
				break;
		}
	}
	else if ((keys & KEY_LEFT)!=0)
	{
		switch(gMenusCurrentItemIndex)
		{
			case 1:
				open_squelch = 0;
				break;
			case 2:
				HR_C6000_datalogging = 0;
				break;
			case 0:
				if (trxGetPower() >= 100)
				{
					trxSetPower(trxGetPower() - 100);
				}
				break;
			case 3:
				doFactoryReset = false;
				break;
			case 4:
				nonVolatileSettings.useCalibration=0x00;
				break;
		}
	}
	else if ((keys & KEY_GREEN)!=0)
	{
		if (doFactoryReset==true)
		{
			settingsRestoreDefaultSettings();
		}
		menuSystemPopAllAndDisplayRootMenu();
		return;
	}
	else if ((keys & KEY_RED)!=0)
	{
		menuSystemPopPreviousMenu();
		return;
	}
	updateScreen();
}

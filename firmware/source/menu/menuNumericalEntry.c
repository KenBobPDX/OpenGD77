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

static char digits[9];
static void updateScreen();
static void handleEvent(int buttons, int keys, int events);

static const char *menuName[2]={"TG entry","Manual dial"};

// public interface
int menuNumericalEntry(int buttons, int keys, int events, bool isFirstRun)
{
	if (isFirstRun)
	{
		gMenusCurrentItemIndex=0;
		digits[0]=0x00;
		updateScreen();
	}
	else
	{
		if (events==0)
		{
			//updateScreen();
		}
		else
		{
			handleEvent(buttons, keys, events);
		}
	}
	return 0;
}


static void updateScreen()
{
	UC1701_clearBuf();

	UC1701_printCentered(8, (char *)menuName[gMenusCurrentItemIndex],UC1701_FONT_GD77_8x16);

	UC1701_printCentered(32, (char *)digits,UC1701_FONT_GD77_8x16);
	displayLightTrigger();

	UC1701_render();
}

static void handleEvent(int buttons, int keys, int events)
{
	if ((keys & KEY_RED)!=0)
	{
		menuSystemPopPreviousMenu();
		return;
	}
	else if ((keys & KEY_GREEN)!=0)
	{
		trxTalkGroup = atoi(digits);
		nonVolatileSettings.overrideTG = trxTalkGroup;
		menuSystemPopAllAndDisplayRootMenu();
	}
	else if ((keys & KEY_HASH)!=0)
	{
		gMenusCurrentItemIndex = 1 - gMenusCurrentItemIndex;
		updateScreen();
	}

	if (strlen(digits)<7)
	{
		char c[2]={0,0};
		if ((keys & KEY_0)!=0)
		{
			c[0]='0';
		}
		else if ((keys & KEY_1)!=0)
		{
			c[0]='1';
		}
		else if ((keys & KEY_2)!=0)
		{
			c[0]='2';
		}
		else if ((keys & KEY_3)!=0)
		{
			c[0]='3';
		}
		else if ((keys & KEY_4)!=0)
		{
			c[0]='4';
		}
		else if ((keys & KEY_5)!=0)
		{
			c[0]='5';
		}
		else if ((keys & KEY_6)!=0)
		{
			c[0]='6';
		}
		else if ((keys & KEY_7)!=0)
		{
			c[0]='7';
		}
		else if ((keys & KEY_8)!=0)
		{
			c[0]='8';
		}
		else if ((keys & KEY_9)!=0)
		{
			c[0]='9';
		}
		else if ((keys & KEY_LEFT)!=0 && strlen(digits)>0)
		{
			digits[strlen(digits)-1]=0;
			updateScreen();
		}
		if (c[0]!=0)
		{
			strcat(digits,c);
			updateScreen();
		}
	}
}

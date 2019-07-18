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

static void updateScreen();
static void handleEvent(int buttons, int keys, int events);
static int updateCounter;

int menuBattery(int buttons, int keys, int events, bool isFirstRun)
{
	if (isFirstRun)
	{
		updateCounter=0;
		updateScreen();
	}
	else
	{
		if (++updateCounter > 10000)
		{
			updateCounter=0;
			updateScreen();// update the screen once per second to show any changes to the battery voltage
		}
		if (events!=0 && keys!=0)
		{
			handleEvent(buttons, keys, events);
		}
	}
	return 0;
}

static void updateScreen()
{
	const int BATTERY_MIN_VOLTAGE = 7;
	const float BATTERY_MAX_VOLTAGE = 8.4;
	char buffer[8];

	UC1701_clearBuf();
	UC1701_printCentered(0, "Battery",UC1701_FONT_GD77_8x16);

	int val1 = battery_voltage/10;
	int val2 = battery_voltage - (val1 * 10);

	sprintf(buffer,"%d.%dV", val1,val2);
	UC1701_printAt(24,16, buffer,UC1701_FONT_16x32);
	uint32_t h = (uint32_t)(((battery_voltage - BATTERY_MIN_VOLTAGE) * 50)/(BATTERY_MAX_VOLTAGE-BATTERY_MIN_VOLTAGE));

	if (h>50)
	{
		h=50;
	}
	// draw frame
	UC1701_fillRect(100,10,24,52,false);
	UC1701_fillRect(101,11,22,50,true);

	UC1701_fillRect(101, 11 + 50 - h ,22,h,false);// fill battery

	for(int y=56; y > 11;y-=5)
	{
		UC1701_fillRect(101,y,22,1,true);
	}

	UC1701_render();
	displayLightTrigger();
}


static void handleEvent(int buttons, int keys, int events)
{
	if ((keys & KEY_RED)!=0)
	{
		menuSystemPopPreviousMenu();
		return;
	}
}

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


int menuPowerOff(int buttons, int keys, int events, bool isFirstRun)
{
	if (isFirstRun)
	{
		menuTimer = 1200;// Not sure why its this value. But never mind ;-)
		updateScreen();
	}
	else
	{
		handleEvent(buttons, keys, events);
	}
	return 0;
}

static void updateScreen()
{
	UC1701_clearBuf();
	UC1701_printCentered(12, "Power off ..",UC1701_FONT_GD77_8x16);
	UC1701_printCentered(32, "73",UC1701_FONT_GD77_8x16);
	UC1701_render();
	displayLightTrigger();
}

static void handleEvent(int buttons, int keys, int events)
{
	if ((GPIO_PinRead(GPIO_Power_Switch, Pin_Power_Switch)==0) && (battery_voltage>CUTOFF_VOLTAGE_LOWER_HYST))
	{
		// I think this is to handle if the power button is turned back on during shutdown
		menuSystemPopPreviousMenu();
		return;
	}

	menuTimer--;

	if (menuTimer == 0)
	{
		// This turns the power off to the CPU.
		GPIO_PinWrite(GPIO_Keep_Power_On, Pin_Keep_Power_On, 0);
	}
}

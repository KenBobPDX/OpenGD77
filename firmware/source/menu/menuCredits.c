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
static void scrollDownOneLine();

#define CREDIT_TEXT_LENGTH 33
static const int NUM_LINES_PER_SCREEN = 7;
static const char creditTexts[][CREDIT_TEXT_LENGTH] = {"Conceived & developed","by","Kai DG4KLU","with help from","Roger VK3KYY","Colin G4EML"};
static int currentDisplayIndex=0;

int menuCredits(int buttons, int keys, int events, bool isFirstRun)
{
	if (isFirstRun)
	{
		gMenusCurrentItemIndex=5000;
		menuTimer = 3000;
		updateScreen();
	}
	else
	{
		if (events!=0 && keys!=0)
		{
			handleEvent(buttons, keys, events);
		}
/*
 * Uncomment to enable auto scrolling
		if ((gMenusCurrentItemIndex--)==0)
		{
			scrollDownOneLine();
			gMenusCurrentItemIndex=1000;
		}
*/
	}
	return 0;
}

static void updateScreen()
{
	int numCredits = sizeof(creditTexts)/CREDIT_TEXT_LENGTH;
	UC1701_clearBuf();

	UC1701_printCentered(0,	"OpenGD77",UC1701_FONT_GD77_8x16);

	for(int i=0;i<6;i++)
	{
		if ((i+currentDisplayIndex) < numCredits)
		{
			UC1701_printCentered(i*8 + 16,(char *)creditTexts[i+currentDisplayIndex],0);
		}
	}
	UC1701_render();
	displayLightTrigger();
}

static void scrollDownOneLine()
{
	int numCredits = sizeof(creditTexts)/CREDIT_TEXT_LENGTH;
	if (currentDisplayIndex < (numCredits - NUM_LINES_PER_SCREEN) )
	{
		currentDisplayIndex++;
	}
	updateScreen();
}

static void handleEvent(int buttons, int keys, int events)
{

	if ((keys & KEY_RED)!=0)
	{
		menuSystemPopPreviousMenu();
		return;
	}
	else if ((keys & KEY_DOWN)!=0)
	{
		scrollDownOneLine();
	}
	else if ((keys & KEY_UP)!=0)
	{
		if (currentDisplayIndex>0)
		{
			currentDisplayIndex--;
		}
		updateScreen();
	}
}

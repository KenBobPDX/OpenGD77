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
#include "fw_main.h"

static void updateScreen();
static void handleEvent(int buttons, int keys, int events);

int menuDisplayMenuList(int buttons, int keys, int events, bool isFirstRun)
{
	if (isFirstRun)
	{

		gMenuCurrentMenuList = (menuItemNew_t *)menusData[menuSystemGetCurrentMenuNumber()];
		gMenusEndIndex = gMenuCurrentMenuList[0].stringNumber;// first entry actually contains the number of entries
		gMenuCurrentMenuList = & gMenuCurrentMenuList[1];// move to first real index
		gMenusCurrentItemIndex=1;
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
	const int headerOffset = 12;
	int rPos;
	UC1701_clearBuf();
	UC1701_printCentered(0, "Menu",UC1701_FONT_GD77_8x16);
	for(int i=-1; i <= 1 ;i++)
		{
			rPos= i + gMenusCurrentItemIndex;
			if (rPos >= gMenusEndIndex)
			{
				rPos=0;
			}
			if (rPos<0)
			{
				rPos = gMenusEndIndex+rPos;
			}
			if (rPos == gMenusCurrentItemIndex)
			{
				UC1701_fillRect(0,2+headerOffset + ((i+1)*16),128,16,false);
			}
		UC1701_printCore(5,(i+1)*16 + (headerOffset) ,(char *)menuStringTable[gMenuCurrentMenuList[rPos].stringNumber], UC1701_FONT_GD77_8x16,0,(rPos==gMenusCurrentItemIndex));
	}


	UC1701_render();
	displayLightTrigger();
}

static void handleEvent(int buttons, int keys, int events)
{
	if ((keys & KEY_DOWN)!=0)
	{
		gMenusCurrentItemIndex++;
		if (gMenusCurrentItemIndex >= gMenusEndIndex)
		{
			gMenusCurrentItemIndex=0;
		}
	}
	else if ((keys & KEY_UP)!=0)
	{
		gMenusCurrentItemIndex--;
		if (gMenusCurrentItemIndex < 0)
		{
			gMenusCurrentItemIndex=gMenusEndIndex-1;
		}
	}
	else if ((keys & KEY_GREEN)!=0)
	{
		if (gMenuCurrentMenuList[gMenusCurrentItemIndex].menuNum!=-1)
		{
			menuSystemPushNewMenu(gMenuCurrentMenuList[gMenusCurrentItemIndex].menuNum);
		}
		return;
	}
	else if ((keys & KEY_RED)!=0)
	{
		menuSystemPopPreviousMenu();
		return;
	}

	updateScreen();
}

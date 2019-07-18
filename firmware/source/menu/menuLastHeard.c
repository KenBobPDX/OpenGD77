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

static void updateScreen();
static void handleEvent(int buttons, int keys, int events);

int menuLastHeard(int buttons, int keys, int events, bool isFirstRun)
{
	if (isFirstRun)
	{
		gMenusCurrentItemIndex=0;
		gMenusStartIndex = LinkHead->id;// reuse this global to store the ID of the first item in the list
		gMenusEndIndex=0;
		updateScreen();
	}
	else
	{
		// do live update by checking if the item at the top of the list has changed
		if (gMenusStartIndex != LinkHead->id || menuDisplayQSODataState==QSO_DISPLAY_CALLER_DATA)
		{
			gMenusStartIndex = LinkHead->id;
			gMenusCurrentItemIndex=0;
			gMenusEndIndex=0;
			updateScreen();
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
	char buffer[17];
	dmrIdDataStruct_t foundRecord;
	int numDisplayed=0;
	LinkItem_t *item = LinkHead;

	UC1701_clearBuf();
	UC1701_printCentered(0, "Last heard",UC1701_FONT_GD77_8x16);

	// skip over the first gMenusCurrentItemIndex in the listing
	for(int i=0;i<gMenusCurrentItemIndex;i++)
	{
		item=item->next;
	}
	while((item != NULL) && item->id != 0)
	{
		if (dmrIDLookup(item->id,&foundRecord))
		{
			UC1701_printCentered(16+(numDisplayed*16), foundRecord.text,UC1701_FONT_GD77_8x16);
		}
		else
		{
			if (item->talkerAlias[0] != 0x00)
			{
				memcpy(buffer,item->talkerAlias,16);// limit to 1 line of the display which is 16 chars at the normal font size
				buffer[16]=0x00;
			}
			else
			{
				sprintf(buffer,"ID:%d",item->id);
			}
			UC1701_printCentered(16+(numDisplayed*16), buffer,UC1701_FONT_GD77_8x16);
		}

		numDisplayed++;

		item=item->next;
		if (numDisplayed>3)
		{
			if (item!=NULL && item->id != 0)
			{
				gMenusEndIndex=0x01;
			}
			else
			{
				gMenusEndIndex=0;
			}
			break;
		}
	}

	UC1701_render();
	displayLightTrigger();
	menuDisplayQSODataState = QSO_DISPLAY_IDLE;
}


static void handleEvent(int buttons, int keys, int events)
{

	if ((keys & KEY_DOWN)!=0 && gMenusEndIndex!=0)
	{
		gMenusCurrentItemIndex++;
	}
	else if ((keys & KEY_UP)!=0)
	{
		gMenusCurrentItemIndex--;
		if (gMenusCurrentItemIndex<0)
		{
			gMenusCurrentItemIndex=0;
		}
	}
	else if ((keys & KEY_RED)!=0)
	{
		menuSystemPopPreviousMenu();
		return;
	}
	updateScreen();
}

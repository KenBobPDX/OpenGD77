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
#ifndef _FW_MENUSYSTEM_H_
#define _FW_MENUSYSTEM_H_
#include "fw_main.h"

extern int menuDisplayLightTimer;
extern int menuTimer;

typedef int (*MenuFunctionPointer_t)(int,int,int,bool); // Typedef for menu function pointers.  Functions are passed the key, the button and the event data. Event can be a Key or a button or both. Last arg is for when the function is only called to initialise and display its screen.
typedef struct menuControlDataStruct
{
	int currentMenuNumber;
	int stackPosition;
	int stack[16];
} menuControlDataStruct_t;

typedef struct menuItemNew
{
       int stringNumber;
       int menuNum;
} menuItemNew_t;

void menuInitMenuSystem();
void displayLightTrigger();
void displayLightOverrideTimeout(int timeout);
void menuSystemPushNewMenu(int menuNumber);

void menuSystemSetCurrentMenu(int menuNumber);
int menuSystemGetCurrentMenuNumber();

void menuSystemPopPreviousMenu();
void menuSystemPopAllAndDisplayRootMenu();
void menuSystemPopAllAndDisplaySpecificRootMenu(int newRootMenu);

void menuSystemCallCurrentMenuTick(int buttons, int keys, int events);

/*
 * ---------------------- IMPORTANT ----------------------------
 *
 * These enums must match the menuFunctions array in menuSystem.c
 *
 * ---------------------- IMPORTANT ----------------------------
 */
enum MENU_SCREENS { MENU_SPLASH_SCREEN=0,
					MENU_POWER_OFF,
					MENU_VFO_MODE,
					MENU_CHANNEL_MODE,
					MENU_MAIN_MENU,
					MENU_CONTACTS_MENU,
					MENU_ZONE_LIST,
					MENU_BATTERY,
					MENU_FIRMWARE_INFO,
					MENU_NUMERICAL_ENTRY,
					MENU_TX_SCREEN,
					MENU_RSSI_SCREEN,
					MENU_LAST_HEARD,
					MENU_UTILITIES,
					MENU_DISPLAY,
					MENU_CREDITS,
					MENU_CHANNEL_DETAILS,
};

extern int gMenusCurrentItemIndex;
extern int gMenusStartIndex;
extern int gMenusEndIndex;
extern menuItemNew_t *gMenuCurrentMenuList;

extern const char menuStringTable[32][16];

extern const menuItemNew_t menuDataMainMenu[];
extern const menuItemNew_t menuDataContact[];
extern const menuItemNew_t menuDataContactContact [];
extern const menuItemNew_t * menusData[];

#endif

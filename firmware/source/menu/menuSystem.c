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

int menuDisplayLightTimer=-1;
int menuTimer;
menuItemNew_t *gMenuCurrentMenuList;

menuControlDataStruct_t menuControlData = { .stackPosition = 0, .stack = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};


int menuVFOMode(int buttons, int keys, int events, bool isFirstRun);
int menuChannelMode(int buttons, int keys, int events, bool isFirstRun);
int menuZoneList(int buttons, int keys, int events, bool isFirstRun);
int menuDisplayMenuList(int buttons, int keys, int events, bool isFirstRun);
int menuBattery(int buttons, int keys, int events, bool isFirstRun);
int menuSplashScreen(int buttons, int keys, int events, bool isFirstRun);
int menuPowerOff(int buttons, int keys, int events, bool isFirstRun);
int menuFirmwareInfoScreen(int buttons, int keys, int events, bool isFirstRun);
int menuNumericalEntry(int buttons, int keys, int events, bool isFirstRun);
int menuTxScreen(int buttons, int keys, int events, bool isFirstRun);
int menuRSSIScreen(int buttons, int keys, int events, bool isFirstRun);
int menuLastHeard(int buttons, int keys, int events, bool isFirstRun);
int menuUtilities(int buttons, int keys, int events, bool isFirstRun);
int menuDisplayOptions(int buttons, int keys, int events, bool isFirstRun);
int menuCredits(int buttons, int keys, int events, bool isFirstRun);
int menuChannelDetails(int buttons, int keys, int events, bool isFirstRun);


/*
 * ---------------------- IMPORTANT ----------------------------
 *
 * The menuFunctions array and the menusData array.....
 *
 * MUST match the enum MENU_SCREENS in menuSystem.h
 *
 * ---------------------- IMPORTANT ----------------------------
 */
const menuItemNew_t * menusData[] = { 	NULL,// splash
										NULL,// power off
										NULL,// vfo mode
										NULL,// channel mode
										menuDataMainMenu,
										menuDataContact,
										NULL,// zone
										NULL,// Battery
										NULL,// Firmwareinfo
										NULL,// Numerical entry
										NULL,// Tx
										NULL,// RSSI
										NULL,// LastHeard
										NULL,// Debug
								};

const MenuFunctionPointer_t menuFunctions[] = { menuSplashScreen,
												menuPowerOff,
												menuVFOMode,
												menuChannelMode,
												menuDisplayMenuList,
												menuDisplayMenuList,
												menuZoneList,
												menuBattery,
												menuFirmwareInfoScreen,
												menuNumericalEntry,
												menuTxScreen,
												menuRSSIScreen,
												menuLastHeard,
												menuUtilities,
												menuDisplayOptions,
												menuCredits,
												menuChannelDetails};

void menuSystemPushNewMenu(int menuNumber)
{
	menuControlData.stackPosition++;
	menuControlData.stack[menuControlData.stackPosition] = menuNumber;
	menuFunctions[menuControlData.stack[menuControlData.stackPosition]](0,0,0,true);
}
void menuSystemPopPreviousMenu()
{
	menuControlData.stackPosition--;
	menuFunctions[menuControlData.stack[menuControlData.stackPosition]](0,0,0,true);
}
void menuSystemPopAllAndDisplayRootMenu()
{
	menuControlData.stackPosition=0;
	menuFunctions[menuControlData.stack[menuControlData.stackPosition]](0,0,0,true);
}

void menuSystemPopAllAndDisplaySpecificRootMenu(int newRootMenu)
{
	menuControlData.stack[0]  = newRootMenu;
	menuControlData.stackPosition=0;
	menuFunctions[menuControlData.stack[menuControlData.stackPosition]](0,0,0,true);
}



void menuSystemSetCurrentMenu(int menuNumber)
{
	menuControlData.stack[menuControlData.stackPosition]  = menuNumber;
	menuFunctions[menuControlData.stack[menuControlData.stackPosition]](0,0,0,true);
}
int menuSystemGetCurrentMenuNumber()
{
	return menuControlData.stack[menuControlData.stackPosition];
}

void menuSystemCallCurrentMenuTick(int buttons, int keys, int events)
{
	menuFunctions[menuControlData.stack[menuControlData.stackPosition]](buttons,keys,events,false);
}

void displayLightTrigger()
{
	menuDisplayLightTimer = nonVolatileSettings.backLightTimeout * 1000;
	fw_displayEnableBacklight(true);
}

// use -1 to force LED on all the time
void displayLightOverrideTimeout(int timeout)
{
	menuDisplayLightTimer = timeout;
	fw_displayEnableBacklight(true);
}

const int MENU_EVENT_SAVE_SETTINGS = -1;
int gMenusCurrentItemIndex; // each menu can re-use this var to hold the position in their display list. To save wasted memory if they each had their own variable
int gMenusStartIndex;// as above
int gMenusEndIndex;// as above


void menuInitMenuSystem()
{
	menuDisplayLightTimer = -1;
	menuControlData.stack[menuControlData.stackPosition]  = MENU_SPLASH_SCREEN;// set the very first screen as the splash screen
	menuFunctions[menuControlData.stack[menuControlData.stackPosition]](0,0,0,true);// Init and display this screen
}

const char menuStringTable[32][16] = { "",//0
                                         "Menu",//1
                                         "Contact",//2
                                         "Message",//3
                                         "Call Logs",//4
                                         "Set",//5
                                         "Zone",//6
                                         "New Contact",//7
                                         "Manual Dial",//8
                                         "InBox",//9
                                         "New Message",//10
                                         "Manual Dial",//11
                                         "OutBox",//12
                                         "Draft",//13
                                         "Quick test",//14
										 "Battery",//15
										 "Firmware info",//16
										 "RSSI",//17
										 "Last heard",//18
										 "Utilities",//19
										 "Display options",//20
										 "Credits",//21
										 "Channel details",//22
};


const menuItemNew_t menuDataMainMenu[] = {
	{9,9},// number of menus
	{ 21, MENU_CREDITS },
	{ 6, MENU_ZONE_LIST },
	{ 17, MENU_RSSI_SCREEN },
	{ 15, MENU_BATTERY },
	{ 18, MENU_LAST_HEARD },
	{ 16, MENU_FIRMWARE_INFO },
	{ 19, MENU_UTILITIES },
	{ 20, MENU_DISPLAY},
	{ 22, MENU_CHANNEL_DETAILS},
};
const menuItemNew_t menuDataContact[] = {
	{ 3, 3 } ,// length
	{ 2, -1 },// Contact
	{ 7 , -1 },// New Contact
	{ 8, -1 } // Manual Dial
};

const menuItemNew_t menuDataContactContact [] = {
	{ 6,6 },// length
	{ 9, -1 },// InBox
	{ 10, -1 },// New Message
	{ 11, -1 },// Manual Dial
	{ 12, -1 },// OutBox
	{ 13, -1 },// Draft
	{ 14, -1 }// Quick Test
};



int menuDisplayList(int buttons, int keys, int events, bool isFirstRun)
{
	return 0;
}


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
#include "fw_trx.h"
#include "fw_codeplug.h"
#include "fw_settings.h"

static void updateScreen();
static void handleEvent(int buttons, int keys, int events);
static const int CTCSS_TONE_NONE = 65535;
static char digits[69];// For CTCSS entry
static int NUM_MENUS=4;
static struct_codeplugChannel_t tmpChannel;// update a temporary copy of the channel and only write back if green menu is pressed
static enum {DISPLAY_MODE_NORMAL,CTCSS_NUMBER_INPUT} displayMode;

int menuChannelDetails(int buttons, int keys, int events, bool isFirstRun)
{
	if (isFirstRun)
	{
		memcpy(&tmpChannel,currentChannelData,sizeof(struct_codeplugChannel_t));
		digits[0]=0x00;// clear CTCSS input digits string
		displayMode=DISPLAY_MODE_NORMAL;
		gMenusCurrentItemIndex=0;
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
	UC1701_printCentered(0, "Channel info",UC1701_FONT_GD77_8x16);

	switch (displayMode)
	{
		case CTCSS_NUMBER_INPUT:
			if(gMenusCurrentItemIndex==2)
			{
			UC1701_printCentered(16, "Tx CTCSS freq",UC1701_FONT_GD77_8x16);
			}
			else
			{
			UC1701_printCentered(16, "Rx CTCSS freq",UC1701_FONT_GD77_8x16);
			}
			UC1701_printCentered(48, (char *)digits,UC1701_FONT_GD77_8x16);
			break;
		default:
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
				if (trxGetMode()==RADIO_MODE_ANALOG)
				{
					strcpy(buf,"Color Code:N/A");
				}
				else
				{
					sprintf(buf,"Color Code:%d",tmpChannel.rxColor);
				}
				break;
			case 1:
				if (trxGetMode()==RADIO_MODE_ANALOG)
				{
					strcpy(buf,"Timeslot:N/A");
				}
				else
				{
					sprintf(buf,"Timeslot:%d",((tmpChannel.flag2 & 0x40) >> 6) + 1);
				}
				break;
			case 2:
				if (trxGetMode()==RADIO_MODE_ANALOG)
				{
					if (tmpChannel.txTone==CTCSS_TONE_NONE)
					{
						strcpy(buf,"Tx CTCSS:None");
					}
					else
					{
						sprintf(buf,"Tx CTCSS:%d.%dHz",tmpChannel.txTone/10 ,tmpChannel.txTone%10 );
					}
				}
				else
				{
					strcpy(buf,"Tx CTCSS:N/A");
				}
				break;
			case 3:
				if (trxGetMode()==RADIO_MODE_ANALOG)
				{
					if (tmpChannel.rxTone==CTCSS_TONE_NONE)
					{
						strcpy(buf,"Rx CTCSS:None");
					}
					else
					{
						sprintf(buf,"Rx CTCSS:%d.%dHz",tmpChannel.rxTone/10 ,tmpChannel.rxTone%10 );
					}
				}
				else
				{
					strcpy(buf,"Rx CTCSS:N/A");
				}
				break;
			}

			if (gMenusCurrentItemIndex==mNum)
			{
				UC1701_fillRect(0,(i+2)*16,128,16,false);
			}

			UC1701_printCore(0,(i+2)*16,buf,UC1701_FONT_GD77_8x16,0,(gMenusCurrentItemIndex==mNum));
		}
		break;
	}
	UC1701_render();
	displayLightTrigger();
}

static void handleEvent(int buttons, int keys, int events)
{
	switch (displayMode)
	{
		case CTCSS_NUMBER_INPUT:
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
				else if ((keys & KEY_HASH)!=0)
				{
					c[0]='.';
				}
				else if ((keys & KEY_LEFT)!=0 && strlen(digits)>0)
				{
					digits[strlen(digits)-1]=0;
				}
				if (c[0]!=0)
				{
					strcat(digits,c);
				}
			}

			if ((keys & KEY_GREEN)!=0)
			{
				if(gMenusCurrentItemIndex==2)
				{
					tmpChannel.txTone=atof(digits)*10;
					trxSetTxCTCSS(tmpChannel.txTone);
				}
				else
				{
					tmpChannel.rxTone=atof(digits)*10;
					trxSetRxCTCSS(tmpChannel.rxTone);
				}
				digits[0]=0x00;
				displayMode = DISPLAY_MODE_NORMAL;
			}
			else if ((keys & KEY_RED)!=0)
			{
				digits[0]=0x00;
				displayMode = DISPLAY_MODE_NORMAL;
			}
			break;
		default:
			if ((keys & KEY_DOWN)!=0)
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
					case 0:
						if (tmpChannel.rxColor<15)
						{
							tmpChannel.rxColor++;
						}
						break;
					case 1:
						tmpChannel.flag2 |= 0x40;// set TS 2 bit
						break;
					case 2:
					case 3:
						if (trxGetMode()==RADIO_MODE_ANALOG)
						{
							displayMode = CTCSS_NUMBER_INPUT;
						}
						break;
				}
			}
			else if ((keys & KEY_LEFT)!=0)
			{
				switch(gMenusCurrentItemIndex)
				{
					case 0:
						if (tmpChannel.rxColor>0)
						{
							tmpChannel.rxColor--;
							trxSetDMRColourCode(tmpChannel.rxColor);
						}
						break;
					case 1:
						tmpChannel.flag2 &= 0xBF;// Clear TS 2 bit
						break;
					case 2:
						if (trxGetMode()==RADIO_MODE_ANALOG)
						{
							tmpChannel.txTone=CTCSS_TONE_NONE;// Set CTCSS to none
							trxSetTxCTCSS(tmpChannel.txTone);
						}
						break;
					case 3:
						if (trxGetMode()==RADIO_MODE_ANALOG)
						{
							tmpChannel.rxTone=CTCSS_TONE_NONE;// Set Rx CTCSS to none
							trxSetRxCTCSS(tmpChannel.rxTone);
						}
						break;
				}
			}
			else if ((keys & KEY_GREEN)!=0)
			{
				memcpy(currentChannelData,&tmpChannel,sizeof(struct_codeplugChannel_t));
				menuSystemPopPreviousMenu();
				return;
			}
			else if ((keys & KEY_RED)!=0)
			{
				menuSystemPopPreviousMenu();
				return;
			}
			break;
	}
	updateScreen();
}


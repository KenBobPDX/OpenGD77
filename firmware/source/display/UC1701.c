/*
 * Copyright (C)2019 Roger Clark, VK3KYY / G4KYF
 *
 * Development informed by work from  Rustem Iskuzhin (in 2014)
 * and https://github.com/bitbank2/uc1701/
 * and https://os.mbed.com/users/Anaesthetix/code/UC1701/file/7494bdca926b/
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

#include "fw_display.h"
#include "UC1701.h"
#include "UC1701_charset.h"
#include "fw_settings.h"

static uint8_t screenBuf[1024];
int activeBufNum=0;

void UC1701_setCommandMode(bool isCommand)
{
	GPIO_PinWrite(GPIO_Display_RS, Pin_Display_RS, !isCommand);
}

void UC1701_transfer(register uint8_t data1)
{
	for (register int i=0; i<8; i++)
	{
		GPIO_Display_SCK->PCOR = 1U << Pin_Display_SCK;

		if ((data1&0x80) == 0U)
		{
			GPIO_Display_SDA->PCOR = 1U << Pin_Display_SDA;// Hopefully the compiler will otimise this to a value rather than using a shift
		}
		else
		{
			GPIO_Display_SDA->PSOR = 1U << Pin_Display_SDA;// Hopefully the compiler will otimise this to a value rather than using a shift
		}
		GPIO_Display_SCK->PSOR = 1U << Pin_Display_SCK;// Hopefully the compiler will otimise this to a value rather than using a shift

		data1=data1<<1;
	}
}

int UC1701_setPixel(int x, int y, bool color)
{
int i;

  i = ((y >> 3) << 7) + x;
  if (i < 0 || i > 1023)
  {
    return -1;// off the screen
  }

  if (color)
  {
	  screenBuf[i] |= (0x1 << (y & 7));
  }
  else
  {
	  screenBuf[i] &= ~(0x1 << (y & 7));
  }
  return 0;
}

void UC1701_render()
{
	uint8_t *rowPos = screenBuf;
	for(int row=0;row<8;row++)
	{
		UC1701_setCommandMode(true);
		UC1701_transfer(0xb0 | row); // set Y
		UC1701_transfer(0x10 | 0); // set X (high MSB)

// Note there are 4 pixels at the left which are no in the hardware of the LCD panel, but are in the RAM buffer of the controller
		UC1701_transfer(0x00 | 4); // set X (low MSB).

		UC1701_setCommandMode(false);
		uint8_t data1;
		for(int line=0;line<128;line++)
		{
			//UC1701_transfer(*rowPos++);
			data1= *rowPos;
			for (register int i=0; i<8; i++)
			{
				GPIO_Display_SCK->PCOR = 1U << Pin_Display_SCK;

				if ((data1&0x80) == 0U)
				{
					GPIO_Display_SDA->PCOR = 1U << Pin_Display_SDA;// Hopefully the compiler will otimise this to a value rather than using a shift
				}
				else
				{
					GPIO_Display_SDA->PSOR = 1U << Pin_Display_SDA;// Hopefully the compiler will otimise this to a value rather than using a shift
				}
				GPIO_Display_SCK->PSOR = 1U << Pin_Display_SCK;// Hopefully the compiler will otimise this to a value rather than using a shift

				data1=data1<<1;
			}
			rowPos++;
		}
	}
}

int UC1701_printCore(int x, int y, char *szMsg, int iSize, int alignment, bool isInverted)
{
int i, sLen;
uint8_t *currentCharData;
int charWidthPixels;
int charHeightPixels;
int bytesPerChar;
int startCode;
uint8_t *currentFont;
uint8_t *writePos;
uint8_t *readPos;

    sLen = strlen(szMsg);

    switch(iSize)
    {
    	case UC1701_FONT_6X8:
    		currentFont = (uint8_t *) font_6x8;
    		break;
    	case UC1701_FONT_6X8_bold:
			currentFont = (uint8_t *) font_6x8_bold;
    		break;
    	case UC1701_FONT_8X8:
    		currentFont = (uint8_t *) font_8x8;
    		break;
    	case UC1701_FONT_GD77_8x16:
    		currentFont = (uint8_t *) font_gd77_8x16;
			break;
    	case UC1701_FONT_16x32:
    		currentFont = (uint8_t *) font_16x32;
			break;

    	default:
    		return -2;// Invalid font selected
    		break;
    }

    startCode   		= currentFont[2];  // get first defined character
    charWidthPixels   	= currentFont[4];  // width in pixel of one char
    charHeightPixels  	= currentFont[5];  // page count per char
    bytesPerChar 		= currentFont[7];  // bytes per char

    if ((charWidthPixels*sLen) + x > 128)
	{
    	sLen = (128-x)/charWidthPixels;
	}

	if (sLen < 0)
	{
		return -1;
	}

	switch(alignment)
	{
		case 0:
			// left aligned, do nothing.
			break;
		case 1:// Align centre
			x = (128 - (charWidthPixels * sLen))/2;
			break;
		case 2:// align right
			x = 128 - (charWidthPixels * sLen);
			break;
	}

	for (i=0; i<sLen; i++)
	{
		currentCharData = (unsigned char *)&currentFont[8 + ((szMsg[i] - startCode) * bytesPerChar)];

		for(int row=0;row < charHeightPixels / 8 ;row++)
		{
			readPos = (currentCharData + row*charWidthPixels);
			writePos = (screenBuf + x + (i*charWidthPixels) + ((y>>3) + row)*128) ;

			if ((y&0x07)==0)
			{
				// y position is aligned to a row
				for(int p=0;p<charWidthPixels;p++)
				{
					if (isInverted)
					{
						*writePos++ &= ~(*readPos++);
					}
					else
					{
						*writePos++ |= *readPos++;
					}
				}
			}
			else
			{
				int shiftNum = y & 0x07;
				// y position is NOT aligned to a row

				for(int p=0;p<charWidthPixels;p++)
				{
					if (isInverted)
					{
						*writePos++ &= ~((*readPos++) << shiftNum);
					}
					else
					{
						*writePos++ |= ((*readPos++) << shiftNum);
					}
				}

				readPos = (currentCharData + row*charWidthPixels);
				writePos = (screenBuf + x + (i*charWidthPixels) + ((y>>3) + row + 1)*128) ;

				for(int p=0;p<charWidthPixels;p++)
				{
					if (isInverted)
					{
						*writePos++ &= ~((*readPos++) >> (8 - shiftNum));
					}
					else
					{
						*writePos++ |= ((*readPos++) >> (8 - shiftNum));
					}
				}
			}
		}
	}
	return 0;
}

void UC1701_setInverseVideo(bool isInverted)
{
	UC1701_setCommandMode(true);
	if (isInverted)
	{
		UC1701_transfer(0xA7); // Black background, white pixels
	}
	else
	{
		UC1701_transfer(0xA4); // White background, black pixels
	}

    UC1701_transfer(0xAF); // Set Display Enable
    UC1701_setCommandMode(false);
}

void UC1701_begin(bool isInverted)
{
	GPIO_PinWrite(GPIO_Display_CS, Pin_Display_CS, 0);// Enable CS permanently
    // Set the LCD parameters...
	UC1701_setCommandMode(true);
	UC1701_transfer(0xE2); // System Reset
	UC1701_transfer(0x2F); // Voltage Follower On
	UC1701_transfer(0x81); // Set Electronic Volume = 15
	UC1701_transfer(nonVolatileSettings.displayContrast); //
	UC1701_transfer(0xA2); // Set Bias = 1/9
	UC1701_transfer(0xA1); // A0 Set SEG Direction
	UC1701_transfer(0xC0); // Set COM Direction
	if (isInverted)
	{
		UC1701_transfer(0xA7); // Black background, white pixels
	}
	else
	{
		UC1701_transfer(0xA4); // White background, black pixels
	}

    UC1701_setCommandMode(true);
    UC1701_transfer(0xAF); // Set Display Enable
    UC1701_clearBuf();
    UC1701_render();
}

void UC1701_setContrast(uint8_t contrast)
{
	UC1701_setCommandMode(true);
	UC1701_transfer(0x81);              // command to set contrast
	UC1701_transfer(contrast);          // set contrast
	UC1701_setCommandMode(false);
}

void UC1701_clearBuf()
{
	memset(screenBuf,0x00,1024);
}

void UC1701_printCentered(uint8_t y, char *text, int fontSize)
{
	UC1701_printCore(0, y, text, fontSize, 1, false);
}

void UC1701_printAt(uint8_t x, uint8_t y, char *text,int fontSize)
{
	UC1701_printCore(x, y, text, fontSize, 0, false);
}

void UC1701_fillRect(int x,int y,int width,int height,bool isInverted)
{
uint8_t *addPtr;
int endStripe 	= x+width;
int startRow 	= y>>3;
int endRow 		= ((y+height)>>3);
uint8_t bitPatten;
int shiftNum;

	if (startRow==endRow)
	{
		addPtr = screenBuf + (startRow << 7);
		bitPatten = (0xff >> (8-(height&0x07))) << (y&0x07);
		//bitPatten = bitPatten ;
		for(int stripe=x;stripe < endStripe;stripe++)
		{
			if (isInverted)
			{
				*(addPtr + stripe) &= ~ bitPatten;
			}
			else
			{
				*(addPtr + stripe) |= bitPatten;
			}
		}
	}
	else
	{
		for(int row=startRow;row<=endRow;row++)
		{
			if (row==startRow)
			{
				shiftNum = y & 0x07;
				bitPatten = (0xff << shiftNum);
			}
			else
			{
				if (row == endRow)
				{
					shiftNum = (y+height) & 0x07;
					bitPatten = (0xff >> (8 - shiftNum));
				}
				else
				{
					// middle rows
					bitPatten = 0xff;
				}
			}
			addPtr = screenBuf + (row << 7);
			for(int stripe=x;stripe < endStripe;stripe++)
			{
				if (isInverted)
				{
					*(addPtr + stripe) &= ~ bitPatten;
				}
				else
				{
					*(addPtr + stripe) |= bitPatten;
				}
			}
		}
	}
}

/*
 * Initial work for port to MK22FN512xxx12 Copyright (C)2019 Kai Ludwig, DG4KLU
 *
 * Code mainly re-written by Roger Clark. VK3KYY / G4KYF
 * based on information and code references from various sources, including
 * https://github.com/bitbank2/uc1701 and
 * https://os.mbed.com/users/Anaesthetix/code/UC1701/file/7494bdca926b/
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

#ifndef __UC1701_H__
#define __UC1701_H__

#include "FreeRTOS.h"
#include "task.h"
#include "fw_common.h"

#define UC1701_FONT_6X8 				0
#define UC1701_FONT_6X8_bold			1
#define UC1701_FONT_8X8 				2
#define UC1701_FONT_GD77_8x16 			3
#define UC1701_FONT_16x32 				4

void UC1701_begin(bool isInverted);
void UC1701_clearBuf();
void UC1701_render();
void UC1701_printCentered(uint8_t y, char *text,int fontSize);
void UC1701_printAt(uint8_t x, uint8_t y, char *text,int fontSize);
int UC1701_printCore(int x, int y, char *szMsg, int iSize, int alignment, bool isInverted);
int UC1701_setPixel(int x, int y, bool color);
void UC1701_fillRect(int x,int y,int width,int height,bool isInverted);
void UC1701_setContrast(uint8_t contrast);
void UC1701_setInverseVideo(bool isInverted);

#endif /* __UC1701_H__ */

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
#ifndef _MENU_UTILITY_QSO_DATA_H_
#define _MENU_UTILITY_QSO_DATA_H_                    /**< Symbol preventing repeated inclusion */
#include "fw_common.h"

#define NUM_LASTHEARD_STORED 16

typedef struct dmrIdDataStruct
{
	int id;
	char text[20];
} dmrIdDataStruct_t;


typedef struct LinkItem
{
    struct LinkItem *prev;
    int id;
    int talkGroup;
    char talkerAlias[32];// 4 blocks of data. 6 bytes + 6 bytes + 7 bytes + 7 bytes . plus 1 for termination some more for safety
    struct LinkItem *next;
} LinkItem_t;

enum QSO_DISPLAY_STATE
{
	QSO_DISPLAY_IDLE,
	QSO_DISPLAY_DEFAULT_SCREEN,
	QSO_DISPLAY_CALLER_DATA
};

extern LinkItem_t *LinkHead;
extern int menuDisplayQSODataState;

bool dmrIDLookup( int targetId,dmrIdDataStruct_t *foundRecord);
void menuUtilityRenderQSOData();
void menuUtilityRenderHeader();
void lastheardInitList();
void lastHeardListUpdate(uint8_t *dmrDataBuffer);
#endif

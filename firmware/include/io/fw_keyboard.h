/*
 * Copyright (C)2019 Kai Ludwig, DG4KLU
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

#ifndef _FW_KEYBOARD_H_
#define _FW_KEYBOARD_H_

#include "fw_common.h"

// column lines
#define Port_Key_Col0   PORTC
#define GPIO_Key_Col0 	GPIOC
#define Pin_Key_Col0	0
#define Port_Key_Col1   PORTC
#define GPIO_Key_Col1 	GPIOC
#define Pin_Key_Col1 	1
#define Port_Key_Col2   PORTC
#define GPIO_Key_Col2 	GPIOC
#define Pin_Key_Col2 	2
#define Port_Key_Col3   PORTC
#define GPIO_Key_Col3 	GPIOC
#define Pin_Key_Col3 	3

// row lines
#define Port_Key_Row0   PORTB
#define GPIO_Key_Row0 	GPIOB
#define Pin_Key_Row0	19
#define Port_Key_Row1   PORTB
#define GPIO_Key_Row1 	GPIOB
#define Pin_Key_Row1	20
#define Port_Key_Row2   PORTB
#define GPIO_Key_Row2 	GPIOB
#define Pin_Key_Row2	21
#define Port_Key_Row3   PORTB
#define GPIO_Key_Row3 	GPIOB
#define Pin_Key_Row3	22
#define Port_Key_Row4   PORTB
#define GPIO_Key_Row4 	GPIOB
#define Pin_Key_Row4	23

#define KEY_UP    0x00000100
#define KEY_DOWN  0x00002000
#define KEY_LEFT  0x00000200
#define KEY_RIGHT 0x00000010
#define KEY_GREEN 0x00000008
#define KEY_RED   0x00040000
#define KEY_0     0x00010000
#define KEY_1     0x00000001
#define KEY_2     0x00000002
#define KEY_3     0x00000004
#define KEY_4     0x00000020
#define KEY_5     0x00000040
#define KEY_6     0x00000080
#define KEY_7     0x00000400
#define KEY_8     0x00000800
#define KEY_9     0x00001000
#define KEY_STAR  0x00008000
#define KEY_HASH  0x00020000

#define EVENT_KEY_NONE   0
#define EVENT_KEY_CHANGE 1

void fw_init_keyboard();
uint8_t fw_read_keyboard_col();
uint32_t fw_read_keyboard();
void fw_check_key_event(uint32_t *keys, int *event);

#endif /* _FW_KEYBOARD_H_ */

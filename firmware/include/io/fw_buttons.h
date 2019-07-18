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

#ifndef _FW_BUTTONS_H_
#define _FW_BUTTONS_H_

#include "fw_common.h"

#define Port_PTT		PORTA
#define GPIO_PTT		GPIOA
#define Pin_PTT			1
#define Port_SK1		PORTB
#define GPIO_SK1		GPIOB
#define Pin_SK1			1
#define Port_SK2		PORTB
#define GPIO_SK2		GPIOB
#define Pin_SK2			9
#define Port_Orange		PORTA
#define GPIO_Orange		GPIOA
#define Pin_Orange		2

#define BUTTON_PTT    0x00000001
#define BUTTON_SK1    0x00000002
#define BUTTON_SK2    0x00000004
#define BUTTON_ORANGE 0x00000008

#define EVENT_BUTTON_NONE   0
#define EVENT_BUTTON_CHANGE 1

void fw_init_buttons();
uint32_t fw_read_buttons();
void fw_check_button_event(uint32_t *buttons, int *event);

#endif /* _FW_BUTTONS_H_ */

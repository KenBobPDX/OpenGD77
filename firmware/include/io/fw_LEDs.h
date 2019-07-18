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

#ifndef _FW_LEDS_H_
#define _FW_LEDS_H_

#include "fw_common.h"

#define Port_LEDgreen	PORTB
#define GPIO_LEDgreen	GPIOB
#define Pin_LEDgreen	18
#define Port_LEDred		PORTC
#define GPIO_LEDred		GPIOC
#define Pin_LEDred		14

void fw_init_LEDs();

#endif /* _FW_LEDS_H_ */

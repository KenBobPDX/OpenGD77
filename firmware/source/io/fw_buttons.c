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

#include "fw_buttons.h"

static uint32_t old_button_state;

void fw_init_buttons()
{
    PORT_SetPinMux(Port_PTT, Pin_PTT, kPORT_MuxAsGpio);
    PORT_SetPinMux(Port_SK1, Pin_SK1, kPORT_MuxAsGpio);
    PORT_SetPinMux(Port_SK2, Pin_SK2, kPORT_MuxAsGpio);
    PORT_SetPinMux(Port_Orange, Pin_Orange, kPORT_MuxAsGpio);

    GPIO_PinInit(GPIO_PTT, Pin_PTT, &pin_config_input);
    GPIO_PinInit(GPIO_SK1, Pin_SK1, &pin_config_input);
    GPIO_PinInit(GPIO_SK2, Pin_SK2, &pin_config_input);
    GPIO_PinInit(GPIO_Orange, Pin_Orange, &pin_config_input);

    old_button_state = 0;
}

uint32_t fw_read_buttons()
{
	uint32_t result = 0;
	if (GPIO_PinRead(GPIO_PTT, Pin_PTT)==0)
	{
		result |= BUTTON_PTT;
	}
	if (GPIO_PinRead(GPIO_SK1, Pin_SK1)==0)
	{
		result |= BUTTON_SK1;
	}
	if (GPIO_PinRead(GPIO_SK2, Pin_SK2)==0)
	{
		result |= BUTTON_SK2;
	}
	if (GPIO_PinRead(GPIO_Orange, Pin_Orange)==0)
	{
		result |= BUTTON_ORANGE;
	}

	return result;
}

void fw_check_button_event(uint32_t *buttons, int *event)
{
	*buttons = fw_read_buttons();

	if (old_button_state!=*buttons)
	{
		old_button_state=*buttons;
		*event = EVENT_BUTTON_CHANGE;
	}
	else
	{
		*event = EVENT_BUTTON_NONE;
	}
}

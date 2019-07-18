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

#include "fw_keyboard.h"

static uint32_t old_keyboard_state;

void fw_init_keyboard()
{
    // column lines
    PORT_SetPinMux(Port_Key_Col0, Pin_Key_Col0, kPORT_MuxAsGpio);
    PORT_SetPinMux(Port_Key_Col1, Pin_Key_Col1, kPORT_MuxAsGpio);
    PORT_SetPinMux(Port_Key_Col2, Pin_Key_Col2, kPORT_MuxAsGpio);
    PORT_SetPinMux(Port_Key_Col3, Pin_Key_Col3, kPORT_MuxAsGpio);

    // row lines
    PORT_SetPinMux(Port_Key_Row0, Pin_Key_Row0, kPORT_MuxAsGpio);
    PORT_SetPinMux(Port_Key_Row1, Pin_Key_Row1, kPORT_MuxAsGpio);
    PORT_SetPinMux(Port_Key_Row2, Pin_Key_Row2, kPORT_MuxAsGpio);
    PORT_SetPinMux(Port_Key_Row3, Pin_Key_Row3, kPORT_MuxAsGpio);
    PORT_SetPinMux(Port_Key_Row4, Pin_Key_Row4, kPORT_MuxAsGpio);

    // column lines
    GPIO_PinInit(GPIO_Key_Col0, Pin_Key_Col0, &pin_config_input);
    GPIO_PinInit(GPIO_Key_Col1, Pin_Key_Col1, &pin_config_input);
    GPIO_PinInit(GPIO_Key_Col2, Pin_Key_Col2, &pin_config_input);
    GPIO_PinInit(GPIO_Key_Col3, Pin_Key_Col3, &pin_config_input);

    // row lines
    GPIO_PinInit(GPIO_Key_Row0, Pin_Key_Row0, &pin_config_input);
    GPIO_PinInit(GPIO_Key_Row1, Pin_Key_Row1, &pin_config_input);
    GPIO_PinInit(GPIO_Key_Row2, Pin_Key_Row2, &pin_config_input);
    GPIO_PinInit(GPIO_Key_Row3, Pin_Key_Row3, &pin_config_input);
    GPIO_PinInit(GPIO_Key_Row4, Pin_Key_Row4, &pin_config_input);

    old_keyboard_state = 0;
}

uint8_t fw_read_keyboard_col()
{
	uint8_t result=0;
	if (GPIO_PinRead(GPIO_Key_Row0, Pin_Key_Row0)==0)
	{
		result|=0x01;
	}
	if (GPIO_PinRead(GPIO_Key_Row1, Pin_Key_Row1)==0)
	{
		result|=0x02;
	}
	if (GPIO_PinRead(GPIO_Key_Row2, Pin_Key_Row2)==0)
	{
		result|=0x04;
	}
	if (GPIO_PinRead(GPIO_Key_Row3, Pin_Key_Row3)==0)
	{
		result|=0x08;
	}
	if (GPIO_PinRead(GPIO_Key_Row4, Pin_Key_Row4)==0)
	{
		result|=0x10;
	}
	return result;
}

uint32_t fw_read_keyboard()
{
    GPIO_PinInit(GPIO_Key_Col3, Pin_Key_Col3, &pin_config_output);
	GPIO_PinWrite(GPIO_Key_Col3, Pin_Key_Col3, 0);
	uint32_t result=fw_read_keyboard_col();
	GPIO_PinWrite(GPIO_Key_Col3, Pin_Key_Col3, 1);
    GPIO_PinInit(GPIO_Key_Col3, Pin_Key_Col3, &pin_config_input);

    GPIO_PinInit(GPIO_Key_Col2, Pin_Key_Col2, &pin_config_output);
	GPIO_PinWrite(GPIO_Key_Col2, Pin_Key_Col2, 0);
	result=(result<<5)|fw_read_keyboard_col();
	GPIO_PinWrite(GPIO_Key_Col2, Pin_Key_Col2, 1);
    GPIO_PinInit(GPIO_Key_Col2, Pin_Key_Col2, &pin_config_input);

    GPIO_PinInit(GPIO_Key_Col1, Pin_Key_Col1, &pin_config_output);
	GPIO_PinWrite(GPIO_Key_Col1, Pin_Key_Col1, 0);
	result=(result<<5)|fw_read_keyboard_col();
	GPIO_PinWrite(GPIO_Key_Col1, Pin_Key_Col1, 1);
    GPIO_PinInit(GPIO_Key_Col1, Pin_Key_Col1, &pin_config_input);

    GPIO_PinInit(GPIO_Key_Col0, Pin_Key_Col0, &pin_config_output);
	GPIO_PinWrite(GPIO_Key_Col0, Pin_Key_Col0, 0);
	result=(result<<5)|fw_read_keyboard_col();
	GPIO_PinWrite(GPIO_Key_Col0, Pin_Key_Col0, 1);
    GPIO_PinInit(GPIO_Key_Col0, Pin_Key_Col0, &pin_config_input);

    return result;
}

void fw_check_key_event(uint32_t *keys, int *event)
{
	*keys = fw_read_keyboard();

	if (old_keyboard_state!=*keys)
	{
		old_keyboard_state=*keys;
		*event = EVENT_KEY_CHANGE;
	}
	else
	{
		*event = EVENT_KEY_NONE;
	}
}

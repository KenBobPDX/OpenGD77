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

#ifndef _FW_MAIN_H_
#define _FW_MAIN_H_

#include <stdint.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "virtual_com.h"
#include "fw_usb_com.h"

#include "fw_common.h"
#include "fw_buttons.h"
#include "fw_LEDs.h"
#include "fw_keyboard.h"
#include "fw_display.h"

#include "UC1701.h"

#include "fw_i2c.h"
#include "fw_spi.h"
#include "fw_i2s.h"
#include "fw_AT1846S.h"
#include "fw_HR-C6000.h"
#include "fw_wdog.h"
#include "fw_adc.h"
#include "fw_dac.h"
#include "fw_pit.h"

#include "fw_sound.h"
#include "fw_trx.h"
#include "fw_SPI_Flash.h"
#include "fw_EEPROM.h"

extern int Display_light_Timer;
extern bool Display_light_Touched;
extern const char *FIRMWARE_VERSION_STRING;

void fw_init();

#endif /* _FW_MAIN_H_ */

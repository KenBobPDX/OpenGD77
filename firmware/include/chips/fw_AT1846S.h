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

#ifndef _FW_AT1846S_H_
#define _FW_AT1846S_H_

#include "FreeRTOS.h"
#include "task.h"
#include "fw_i2c.h"

#define AT1846_BYTES_PER_COMMAND 3
#define BANDWIDTH_12P5KHZ false
#define BANDWIDTH_25KHZ true

void I2C_AT1846S_init();
void I2C_AT1846_Postinit();
void I2C_AT1846_SetBandwidth(bool bandWidthIs25kHz);
void I2C_AT1846_SetMode(int theMode);

#endif /* _FW_AT1846S_H_ */

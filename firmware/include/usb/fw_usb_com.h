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

#ifndef _FW_USB_COM_H_
#define _FW_USB_COM_H_

#include "virtual_com.h"

#include "FreeRTOS.h"
#include "task.h"

#include "fw_common.h"

#include "fw_main.h"

#define COM_BUFFER_SIZE 8192
#define COM_REQUESTBUFFER_SIZE 64

extern uint8_t tmp_val_0x82;
extern uint8_t tmp_val_0x86;
extern uint8_t tmp_val_0x51;
extern uint8_t tmp_val_0x52;
extern uint8_t tmp_val_0x57;
extern uint8_t tmp_val_0x5f;
extern uint8_t tmp_ram[256];
extern uint8_t tmp_ram1[256];
extern uint8_t tmp_ram2[256];

extern volatile uint8_t com_buffer[COM_BUFFER_SIZE];
extern int com_buffer_write_idx;
extern int com_buffer_read_idx;
extern volatile int com_buffer_cnt;

extern volatile int com_request;
extern volatile uint8_t com_requestbuffer[COM_REQUESTBUFFER_SIZE];

void tick_com_request();
void send_packet(uint8_t val_0x82, uint8_t val_0x86, int ram);
void send_packet_big(uint8_t val_0x82, uint8_t val_0x86, int ram1, int ram2);
void add_to_commbuffer(uint8_t value);

#endif /* _FW_USB_COM_H_ */

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

#ifndef _FW_CODEC_H_
#define _FW_CODEC_H_

#include "fw_common.h"
#include "fw_HR-C6000.h"

#include "fw_sound.h"
#include "fw_mbelib.h"

#define QUAUX(X) #X
#define QU(X) QUAUX(X)

#define AMBE_DECODE 0x00054319;
#define AMBE_ENCODE 0x00053E70;
#define AMBE_ENCODE_ECC 0x00054228;
#define AMBE_DECODE_BUFFER 0x1FFF859C;
#define AMBE_ENCODE_BUFFER 0x1FFF6B60;
#define AMBE_ENCODE_ECC_BUFFER 0x1FFF8244;

void init_codec();
void tick_codec_decode(uint8_t *indata_ptr);
void tick_codec_encode(uint8_t *outdata_ptr);

#endif /* _FW_CODEC_H_ */

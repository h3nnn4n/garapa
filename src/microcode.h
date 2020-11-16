/******************************************************************************
 * Copyright (C) 2016-2018  Renan S. Silva                                    *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgment in the product documentation would be   *
 *    appreciated but is not required.                                        *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 ******************************************************************************/
#ifndef MICROCODE_H
#define MICROCODE_H

#include "types.h"

uint16_t read_bc(_cpu_info *cpu);
uint16_t read_de(_cpu_info *cpu);
uint16_t read_hl(_cpu_info *cpu);

void write_hl_16(_cpu_info *cpu, uint16_t data);
void write_de_16(_cpu_info *cpu, uint16_t data);
void write_bc_16(_cpu_info *cpu, uint16_t data);

uint8_t read_byte_at_pc(_cpu_info *cpu);
uint8_t read_byte_at_sp(_cpu_info *cpu);
uint8_t read_byte_with_tick(_cpu_info *cpu, uint16_t addr);

void write_byte_at_sp(_cpu_info *cpu, uint8_t data);
void write_byte_with_tick(_cpu_info *cpu, uint16_t addr, uint8_t data);

void write_hl(_cpu_info *cpu, uint8_t lo, uint8_t hi);

#endif /* MICROCODE_H */

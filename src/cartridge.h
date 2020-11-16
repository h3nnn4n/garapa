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
#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "types.h"

void     print_rom_info(_cpu_info *cpu);
void     cartridge_write(_cpu_info *cpu, uint16_t addr, uint8_t data);
uint32_t get_rom_size(uint8_t *addr);
uint32_t get_ram_size(uint8_t *addr);
uint16_t cartridge_read(_cpu_info *cpu, uint16_t addr);
uint8_t  get_ram_code(uint8_t *addr);
uint8_t  get_rom_code(uint8_t *addr);

#endif /* CARTRIDGE_H */

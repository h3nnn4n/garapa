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
#ifndef INSTRUCTIONS_DATA_TRANSFER_H
#define INSTRUCTIONS_DATA_TRANSFER_H

#include <assert.h>
#include <stdint.h>

#include "types.h"

uint8_t *get_reg_ref(_cpu_info *cpu, uint8_t opcode);
void     emulate_XCHG(_cpu_info *cpu);
void     emulate_LDAX(_cpu_info *cpu);
void     emulate_STAX(_cpu_info *cpu);
void     emulate_SHLD(_cpu_info *cpu);
void     emulate_LHLD(_cpu_info *cpu);
void     emulate_LDA(_cpu_info *cpu);
void     emulate_LD(_cpu_info *cpu);
void     emulate_STA(_cpu_info *cpu);
void     emulate_LXI(_cpu_info *cpu);
void     emulate_MVI(_cpu_info *cpu);
void     emulate_MOV(_cpu_info *cpu);

#endif /* INSTRUCTIONS_DATA_TRANSFER_H */

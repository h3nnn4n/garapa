/******************************************************************************
 * Copyright (C) 2016  Renan S. Silva                                         *
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
#ifndef INSTRUCTIONS_0XCB_H
#define INSTRUCTIONS_0XCB_H

#include <stdint.h>

#include "types.h"

void emulate_RR    ( _cpu_info *cpu, uint8_t target ) ;
void emulate_RLC   ( _cpu_info *cpu, uint8_t target ) ;
void emulate_RRC   ( _cpu_info *cpu, uint8_t target ) ;
void emulate_RL    ( _cpu_info *cpu, uint8_t target ) ;
void emulate_SLA   ( _cpu_info *cpu, uint8_t target ) ;
void emulate_SRA   ( _cpu_info *cpu, uint8_t target ) ;
void emulate_SWAP  ( _cpu_info *cpu, uint8_t target ) ;
void emulate_SRL   ( _cpu_info *cpu, uint8_t target ) ;
void decode_0xcb   ( _cpu_info *cpu                 ) ;

#endif /* INSTRUCTIONS_0XCB_H */

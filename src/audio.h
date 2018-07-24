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
#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

#include "audio_types.h"
#include "types.h"

void apu_sdl_init();

void apu_update ( _cpu_info *cpu );
void apu_update_on_div_change ( _cpu_info *cpu );

uint8_t apu_read_byte ( _cpu_info *cpu, uint16_t addr );
void apu_write_byte ( _cpu_info *cpu, uint16_t addr, uint8_t data );

void apu_clear ( _cpu_info *cpu );
void apu_reset ( _cpu_info *cpu );

void apu_ch1_reset ( _cpu_info *cpu );
void apu_ch2_reset ( _cpu_info *cpu );
void apu_ch3_reset ( _cpu_info *cpu );
void apu_ch4_reset ( _cpu_info *cpu );

void apu_ch1_clear ( _cpu_info *cpu );
void apu_ch2_clear ( _cpu_info *cpu );
void apu_ch3_clear ( _cpu_info *cpu );
void apu_ch4_clear ( _cpu_info *cpu );

void apu_ch1_step ( _cpu_info *cpu );
void apu_ch2_step ( _cpu_info *cpu );
void apu_ch3_step ( _cpu_info *cpu );
void apu_ch4_step ( _cpu_info *cpu );

uint16_t apu_ch1_sample ( _cpu_info *cpu );
uint16_t apu_ch2_sample ( _cpu_info *cpu );
uint16_t apu_ch3_sample ( _cpu_info *cpu );
uint16_t apu_ch4_sample ( _cpu_info *cpu );

uint8_t apu_is_ch1_enabled ( _cpu_info *cpu );
uint8_t apu_is_ch2_enabled ( _cpu_info *cpu );
uint8_t apu_is_ch3_enabled ( _cpu_info *cpu );
uint8_t apu_is_ch4_enabled ( _cpu_info *cpu );

void apu_ch1_trigger        ( _cpu_info *cpu );
void apu_ch1_step_length    ( _cpu_info *cpu );
void apu_ch1_step_volume    ( _cpu_info *cpu );
void apu_ch1_step_sweep     ( _cpu_info *cpu );
uint16_t apu_ch1_calc_sweep ( _cpu_info *cpu );

void apu_ch2_step_length( _cpu_info *cpu );
void apu_ch3_step_length( _cpu_info *cpu );
void apu_ch4_step_length( _cpu_info *cpu );
void apu_ch2_step_volume( _cpu_info *cpu );
void apu_ch4_step_volume( _cpu_info *cpu );


#endif /* AUDIO_H */

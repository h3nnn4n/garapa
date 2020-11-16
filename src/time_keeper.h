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
#ifndef TIME_KEEPER_H
#define TIME_KEEPER_H

#include <time.h>
#include <unistd.h>

struct timespec time_diff(struct timespec start, struct timespec end);
void            timekeeper_tic(struct timespec *t1);
void            timekeeper_wait(struct timespec *t1, struct timespec *t2);

void write_TAC(_cpu_info *cpu, uint8_t data);
void write_TMA(_cpu_info *cpu, uint16_t data);
void write_TIMA(_cpu_info *cpu, uint16_t data);
void write_DIV(_cpu_info *cpu, uint16_t data);

void timer_update(_cpu_info *cpu);
void timer_bump(_cpu_info *cpu);
void timer_tick_and_full_mcycle(_cpu_info *cpu);

uint8_t  read_TAC(_cpu_info *cpu);
uint16_t read_TMA(_cpu_info *cpu);
uint16_t read_TIMA(_cpu_info *cpu);
uint8_t  read_DIV(_cpu_info *cpu);
void     reset_TIMA(_cpu_info *cpu);

#endif /* TIME_KEEPER_H */

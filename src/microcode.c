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
#include "memory.h"
#include "types.h"

#include "time_keeper.h"

void write_byte_at_sp(_cpu_info *cpu, uint8_t data) {
    timer_tick_and_full_mcycle(cpu);

    cpu->sp -= 1;
    write_byte(cpu, cpu->sp, data);
}

uint8_t read_byte_at_sp(_cpu_info *cpu) {
    timer_tick_and_full_mcycle(cpu);

    uint8_t value = read_byte(cpu, cpu->sp);
    cpu->sp += 1;

    return value;
}

uint8_t read_byte_at_pc(_cpu_info *cpu) {
    timer_tick_and_full_mcycle(cpu);

    uint8_t value = read_byte(cpu, cpu->pc);
    cpu->pc += 1;

    return value;
}

uint8_t read_byte_with_tick(_cpu_info *cpu, uint16_t addr) {
    timer_tick_and_full_mcycle(cpu);

    uint8_t value = read_byte(cpu, addr);

    return value;
}

void write_byte_with_tick(_cpu_info *cpu, uint16_t addr, uint8_t data) {
    timer_tick_and_full_mcycle(cpu);

    write_byte(cpu, addr, data);
}

uint16_t read_bc(_cpu_info *cpu) { return (cpu->b << 8) | cpu->c; }

uint16_t read_de(_cpu_info *cpu) { return (cpu->d << 8) | cpu->e; }

uint16_t read_hl(_cpu_info *cpu) { return (cpu->h << 8) | cpu->l; }

void write_bc_16(_cpu_info *cpu, uint16_t data) {
    cpu->c = (data & 0x00ff) >> 0;
    cpu->b = (data & 0xff00) >> 8;
}

void write_de_16(_cpu_info *cpu, uint16_t data) {
    cpu->e = (data & 0x00ff) >> 0;
    cpu->d = (data & 0xff00) >> 8;
}

void write_hl_16(_cpu_info *cpu, uint16_t data) {
    cpu->l = (data & 0x00ff) >> 0;
    cpu->h = (data & 0xff00) >> 8;
}

void write_hl(_cpu_info *cpu, uint8_t lo, uint8_t hi) {
    cpu->l = lo;
    cpu->h = hi;
}

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
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "microcode.h"
#include "time_keeper.h"
#include "types.h"

#include "instructions_branch.h"

static void return_if(_cpu_info *cpu, uint8_t flag) {
    if (flag) {
        uint16_t addr;

        timer_tick_and_full_mcycle(cpu);

        addr = read_byte_at_sp(cpu);
        addr |= read_byte_at_sp(cpu) << 8;

        timer_tick_and_full_mcycle(cpu);
        cpu->pc = addr;
    } else {
        timer_tick_and_full_mcycle(cpu);
    }
}

static void call_if(_cpu_info *cpu, uint8_t flag) {
    uint16_t t  = 0;
    uint16_t t2 = 0;

    if (flag) {
        t  = cpu->pc + 2;
        t2 = read_byte_at_pc(cpu);
        t2 |= read_byte_at_pc(cpu) << 8;

        timer_tick_and_full_mcycle(cpu);

        write_byte_at_sp(cpu, (t >> 8) & 0xff);
        write_byte_at_sp(cpu, (t >> 0) & 0xff);
        cpu->pc = t2;
    } else {
        timer_tick_and_full_mcycle(cpu);
        timer_tick_and_full_mcycle(cpu);
        cpu->pc += 2;
    }
}

static void jmp_if(_cpu_info *cpu, uint8_t flag) {
    if (flag) {
        uint16_t addr;
        addr = read_byte_at_pc(cpu);
        addr |= read_byte_at_pc(cpu) << 8;
        timer_tick_and_full_mcycle(cpu);
        cpu->pc = addr;
    } else {
        timer_tick_and_full_mcycle(cpu);
        timer_tick_and_full_mcycle(cpu);
        cpu->pc += 2;
    }
}

void emulate_JMP(_cpu_info *cpu) {
    uint16_t addr = 0;

    addr = read_byte_at_pc(cpu);
    addr |= read_byte_at_pc(cpu) << 8;
    timer_tick_and_full_mcycle(cpu);
    cpu->pc = addr;
}

void emulate_JC(_cpu_info *cpu) { jmp_if(cpu, cpu->flags.c); }

void emulate_JNC(_cpu_info *cpu) { jmp_if(cpu, !cpu->flags.c); }

void emulate_JM(_cpu_info *cpu) { jmp_if(cpu, cpu->flags.n); }

void emulate_JP(_cpu_info *cpu) { jmp_if(cpu, !cpu->flags.n); }

void emulate_JZ(_cpu_info *cpu) { jmp_if(cpu, cpu->flags.z); }

void emulate_JNZ(_cpu_info *cpu) { jmp_if(cpu, !cpu->flags.z); }

void emulate_RETI(_cpu_info *cpu) {
    uint16_t addr;

    addr = read_byte_at_sp(cpu);
    addr |= read_byte_at_sp(cpu) << 8;

    timer_tick_and_full_mcycle(cpu); // A full cycle while the cpu does something

    cpu->pc = addr;

    cpu->enable_interrupts = 1;
}

void emulate_RET(_cpu_info *cpu) {
    uint16_t addr;

    addr = read_byte_at_sp(cpu);
    addr |= read_byte_at_sp(cpu) << 8;

    timer_tick_and_full_mcycle(cpu); // A full cycle while the cpu does something

    cpu->pc = addr;
}

void emulate_RZ(_cpu_info *cpu) { return_if(cpu, cpu->flags.z); }

void emulate_RNZ(_cpu_info *cpu) { return_if(cpu, !cpu->flags.z); }

void emulate_RP(_cpu_info *cpu) { return_if(cpu, !cpu->flags.n); }

void emulate_RM(_cpu_info *cpu) { return_if(cpu, cpu->flags.n); }

void emulate_RNC(_cpu_info *cpu) { return_if(cpu, !cpu->flags.c); }

void emulate_RC(_cpu_info *cpu) { return_if(cpu, cpu->flags.c); }

void emulate_RST(_cpu_info *cpu) {
    uint16_t ret = cpu->pc + 0;

    timer_tick_and_full_mcycle(cpu);

    write_byte_at_sp(cpu, (ret >> 8) & 0xff);
    write_byte_at_sp(cpu, (ret >> 0) & 0xff);
    switch (cpu->opcode) {
        case 0xc7: cpu->pc = 0x00; break;
        case 0xcf: cpu->pc = 0x08; break;
        case 0xd7: cpu->pc = 0x10; break;
        case 0xdf: cpu->pc = 0x18; break;
        case 0xe7: cpu->pc = 0x20; break;
        case 0xef: cpu->pc = 0x28; break;
        case 0xf7: cpu->pc = 0x30; break;
        case 0xff: cpu->pc = 0x38; break;
        default: assert(0 && "Code should not get here\n");
    }
}

void emulate_PCHL(_cpu_info *cpu) {
    switch (cpu->opcode) {
        case 0xe9: // PCHL
            cpu->pc = read_hl(cpu);
            break;
        default: assert(0 && "Code should not get here\n");
    }
}

void emulate_CNC(_cpu_info *cpu) { call_if(cpu, !cpu->flags.c); }

void emulate_CC(_cpu_info *cpu) { call_if(cpu, cpu->flags.c); }

void emulate_CNZ(_cpu_info *cpu) { call_if(cpu, !cpu->flags.z); }

void emulate_CZ(_cpu_info *cpu) { call_if(cpu, cpu->flags.z); }

void emulate_CALL(_cpu_info *cpu) {
    uint16_t t  = 0;
    uint16_t t2 = 0;

    switch (cpu->opcode) {
        case 0xcd: {
            t  = cpu->pc + 2;
            t2 = read_byte_at_pc(cpu); // Read Jump Addr
            t2 |= read_byte_at_pc(cpu) << 8;

            timer_tick_and_full_mcycle(cpu);

            write_byte_at_sp(cpu, (t >> 8) & 0xff); // Push ret addr
            write_byte_at_sp(cpu, (t >> 0) & 0xff);
            cpu->pc = t2;
        } break;
        default: assert(0 && "Code should not get here\n");
    }
}

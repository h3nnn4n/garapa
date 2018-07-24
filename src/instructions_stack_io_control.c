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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "memory.h"
#include "types.h"
#include "microcode.h"
#include "time_keeper.h"

#include "instructions_stack_io_control.h"

void emulate_STOP ( _cpu_info *cpu ) {
    switch ( cpu->opcode ) {
        case 0x10:
            /*cpu->stoped = 1;*/
            break;
        default:
            assert( 0 && "Code should not get here\n" ); }
}

void emulate_HALT ( _cpu_info *cpu ) {
    uint8_t intn = read_byte(cpu, 0xff0f) & (read_byte(cpu, 0xffff));

    switch ( cpu->opcode ) {
        case 0x76:
            if ( cpu->enable_interrupts == 0 && intn )
                cpu->halt_bug = 1;

            cpu->halted = 1;
            break;
        default:
            assert( 0 && "Code should not get here\n" ); }
}

void emulate_POP ( _cpu_info *cpu ) {
    uint8_t t = 0;
    switch ( cpu->opcode ) {
        case 0xc1: // POP B
            cpu->c = read_byte_at_sp ( cpu );
            cpu->b = read_byte_at_sp ( cpu );
            break;
        case 0xd1: // POP D
            cpu->e = read_byte_at_sp ( cpu );
            cpu->d = read_byte_at_sp ( cpu );
            break;
        case 0xe1: // POP H
            cpu->l = read_byte_at_sp ( cpu );
            cpu->h = read_byte_at_sp ( cpu );
            break;
        case 0xf1: // POP PSW
            t      = read_byte_at_sp ( cpu ); // This must be read first
            cpu->a = read_byte_at_sp ( cpu );

            cpu->flags.z  = ((t & 0x80));
            cpu->flags.n  = ((t & 0x40));
            cpu->flags.h  = ((t & 0x20));
            cpu->flags.c  = ((t & 0x10));
            break;
        default:
            assert( 0 && "Code should not get here\n" ); }
}

void emulate_PUSH ( _cpu_info *cpu ) {
    timer_tick_and_full_mcycle ( cpu );
    switch ( cpu->opcode ) {
        case 0xc5: // PUSH B
            write_byte_at_sp ( cpu, cpu->b );
            write_byte_at_sp ( cpu, cpu->c );
            break;
        case 0xd5: // PUSH D
            write_byte_at_sp ( cpu, cpu->d );
            write_byte_at_sp ( cpu, cpu->e );
            break;
        case 0xe5: // PUSH H
            write_byte_at_sp ( cpu, cpu->h );
            write_byte_at_sp ( cpu, cpu->l );
            break;
        case 0xf5: // PUSH PSW
            write_byte_at_sp ( cpu, cpu->a );
            write_byte_at_sp ( cpu,
                                     ( cpu->flags.z  ? 0x80 : 0x0 ) |
                                     ( cpu->flags.n  ? 0x40 : 0x0 ) |
                                     ( cpu->flags.h  ? 0x20 : 0x0 ) |
                                     ( cpu->flags.c  ? 0x10 : 0x0 ) );
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

void emulate_EI ( _cpu_info *cpu ) {
    switch ( cpu->opcode ) {
            case 0xfb: // EI
            cpu->enable_interrupts = 1;
            cpu->pending_interrupts = 2; // Sets the interrupt delay
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

void emulate_DI ( _cpu_info *cpu ) {
    switch ( cpu->opcode ) {
            case 0xf3: // EI
            cpu->enable_interrupts = 0;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

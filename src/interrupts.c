/******************************************************************************
 * Copyright (C) 2016-2020  Renan S. Silva                                    *
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
#include <stdio.h>

#include "debug.h"
#include "memory.h"
#include "microcode.h"
#include "time_keeper.h"
#include "types.h"
#include "utils.h"
#include "python_api.h"

// Bit 0: V-Blank  Interrupt Request (INT 40h)  (1=Request)
// Bit 1: LCD STAT Interrupt Request (INT 48h)  (1=Request)
// Bit 2: Timer    Interrupt Request (INT 50h)  (1=Request)
// Bit 3: Serial   Interrupt Request (INT 58h)  (1=Request)
// Bit 4: Joypad   Interrupt Request (INT 60h)  (1=Request)
void emulate_INTERRUPT(_cpu_info *cpu) {
    if (cpu->pending_interrupts == 2) {
        cpu->pending_interrupts--;
        return;
    } else {
        cpu->pending_interrupts = 0;
    }

    /*printf("%4x %4x %2d\n",*/
    /*read_byte(cpu, 0xff0f),*/
    /*read_byte(cpu, 0xffff),*/
    /*cpu->enable_interrupts);*/

    uint8_t intn = _read_byte(cpu, 0xff0f) & (_read_byte(cpu, 0xffff));

    if (cpu->enable_interrupts == 0 && (read_byte(cpu, 0xff0f) & read_byte(cpu, 0xffff))) {
        cpu->halted = 0;
        return;
    }

    uint8_t  doit = 0;
    uint16_t ret  = cpu->pc;
    if (cpu->pending_interrupts == 0 && cpu->enable_interrupts == 1 && read_byte(cpu, 0xff0f) & read_byte(cpu, 0xffff))
        doit = 1;

    if (doit) {
        if (debug_interrupts)
            printf("Interrupt: ");
        timer_tick_and_full_mcycle(cpu);
        timer_tick_and_full_mcycle(cpu);
        write_byte_at_sp(cpu, (ret >> 8) & 0xff);
        write_byte_at_sp(cpu, (ret >> 0) & 0xff);

        if (intn & 0x01) { // vblank
            if (debug_interrupts)
                printf("VLANK\n");
            cpu->pc                        = 0x0040;
            cpu->interrupts.pending_vblank = 0;
            doit                           = 0;

            trigger_vblank_callback(); // For the python api
        } else if (intn & 0x02) { // lcdstat
            if (debug_interrupts)
                printf("STAT\n");
            cpu->pc                         = 0x0048;
            cpu->interrupts.pending_lcdstat = 0;
            doit                            = 0;
        } else if (intn & 0x04) { // timer
            if (debug_interrupts)
                printf("TIMER\n");
            cpu->pc                       = 0x0050;
            cpu->interrupts.pending_timer = 0;
            doit                          = 0;
        } else if (intn & 0x08) { // serial
            if (debug_interrupts)
                printf("SERIAL\n");
            cpu->pc                        = 0x0058;
            cpu->interrupts.pending_serial = 0;
            doit                           = 0;
        } else if (intn & 0x10) { // joypad
            if (debug_interrupts)
                printf("JOYPAD\n");
            cpu->pc                        = 0x0060;
            cpu->interrupts.pending_joypad = 0;
            doit                           = 0;
        }

        timer_tick_and_full_mcycle(cpu);

        if (doit)
            assert(0 && "It should never happened: Interrupt requested but nothing was serviced");

        cpu->enable_interrupts = 0;
        cpu->halted            = 0;
        if (debug_interrupts)
            printf("Interrupt Serviced\n");
    }
}

uint8_t interrupt_read_IF(_cpu_info *cpu) {
    uint8_t mask = 0xE0;

    mask |= (cpu->interrupts.pending_vblank << 0);
    mask |= (cpu->interrupts.pending_lcdstat << 1);
    mask |= (cpu->interrupts.pending_timer << 2);
    mask |= (cpu->interrupts.pending_serial << 3);
    mask |= (cpu->interrupts.pending_joypad << 4);

    return mask;
}

void interrupt_write_IF(_cpu_info *cpu, uint8_t mask) {
    cpu->interrupts.pending_vblank  = !!(mask & 0x01);
    cpu->interrupts.pending_lcdstat = !!(mask & 0x02);
    cpu->interrupts.pending_timer   = !!(mask & 0x04);
    cpu->interrupts.pending_serial  = !!(mask & 0x08);
    cpu->interrupts.pending_joypad  = !!(mask & 0x10);
}

uint8_t interrupt_read_mask(_cpu_info *cpu) {
    unsigned char mask = cpu->interrupts.ununsed_bits;

    mask |= (!cpu->interrupts.masked_vblank << 0);
    mask |= (!cpu->interrupts.masked_lcdstat << 1);
    mask |= (!cpu->interrupts.masked_timer << 2);
    mask |= (!cpu->interrupts.masked_serial << 3);
    mask |= (!cpu->interrupts.masked_joypad << 4);

    return mask;
}

void interrupt_write_mask(_cpu_info *cpu, uint8_t mask) {
    cpu->interrupts.masked_vblank  = !(mask & 0x01);
    cpu->interrupts.masked_lcdstat = !(mask & 0x02);
    cpu->interrupts.masked_timer   = !(mask & 0x04);
    cpu->interrupts.masked_serial  = !(mask & 0x08);
    cpu->interrupts.masked_joypad  = !(mask & 0x10);

    cpu->interrupts.ununsed_bits = mask & 0xe0;
}

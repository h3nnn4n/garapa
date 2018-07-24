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
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "debug.h"

#include "types.h"
#include "audio.h"
#include "time_keeper.h"
#include "memory.h"
#include "display.h"
#include "graphics.h"

void print_timer_state ( _cpu_info *cpu ) {
    /*printf(" CYCLES: %8llu /%8llu  TIMER: %4x  DIV: %2x  TIMA: %2x  TMA: %2x  TIMA_delay: %2d  Enable: %c  Speed: %2x\n",*/
    printf("  Timer: DIV: %2x  TIMA: %2x  TMA: %2x  TIMA_reload_delay: %2d  TIMA_delay: %2d  Enable: %c  Speed: %2x\n",
            /*cpu->cycles_clock,*/
            /*cpu->cycles_machine,*/
            cpu->timer._timer,
            /*cpu->timer.DIV,*/
            cpu->timer.TIMA,
            cpu->timer.TMA,
            /*cpu->timer.TIMA_timer ? 'y':'n',*/
            /*cpu->timer.TAC & 0x04 ? 'y':'n',*/
            cpu->timer.TIMA_reload_timer,
            cpu->timer.TIMA_timer,
            cpu->timer.TAC & 0x04 ? '1':'0',
            cpu->timer.TAC & 0x03
            );
}

// 0xff07
void write_TAC ( _cpu_info *cpu, uint8_t data ) {
    cpu->timer.running = data & 0x04;

    if ( (cpu->timer.TAC & 0x04) && !(data & 0x04) )
    switch ( cpu->timer.TAC & 0x03 ) {
        case 0x00: // 00: 4096   Hz ( Increase every 1024 clocks )
            if ( cpu->timer._timer & 0x0200 ) {
                cpu->timer.TIMA ++;
                if ( debug_timer ) printf("TIMA triggered on TAC write\n");
            }
            break;
        case 0x01: // 01: 262144 Hz ( 16 clocks  )
            if ( cpu->timer._timer & 0x0008 ) {
                cpu->timer.TIMA ++;
                if ( debug_timer ) printf("TIMA triggered on TAC write\n");
            }
            break;
        case 0x02: // 10: 65536  Hz ( 64 clocks  )
            if ( cpu->timer._timer & 0x0020 ) {
                cpu->timer.TIMA ++;
                if ( debug_timer ) printf("TIMA triggered on TAC write\n");
            }
            break;
        case 0x03: // 11: 16386  Hz ( 256 clocks )
            if ( cpu->timer._timer & 0x0080 ) {
                cpu->timer.TIMA ++;
                if ( debug_timer ) printf("TIMA triggered on TAC write\n");
            }
            break;
        default:
            assert ( 0 && "Invalid value for TAC when writing to DIV\n");
            break;
    }

    /*if ( cpu->timer.TIMA > 0xff ) {*/
        /*cpu->interrupts.pending_timer = 1;*/
        /*reset_TIMA ( cpu );*/
    /*}*/
    if ( cpu->timer.TIMA > 0xff ) {
        cpu->timer.TIMA_reload_timer = 4;
        cpu->interrupts.pending_timer = 1;
        cpu->timer.TIMA = 0x00;

        if ( debug_timer ) printf("TIMA RESET: ");
        if ( debug_timer ) print_timer_state ( cpu );
    }

    if ( debug_timer ) printf("TAC changed: %2x ->: Enable: %c  Speed: %2x\n",
            data,
            data & 0x04 ? 'y':'n',
            data & 0x03);

    cpu->timer.TAC     = data;
}

// 0xff07
uint8_t read_TAC ( _cpu_info *cpu ) {
    if ( debug_timer ) printf("TAC was read\n");
    return cpu->timer.TAC;
}

// 0xff06
void write_TMA ( _cpu_info *cpu, uint16_t data ) {
    if ( debug_timer ) printf("TMA changed %2x to %2x\n", cpu->timer.TMA, data);

    cpu->timer.TMA = data;

    if ( cpu->timer.TIMA_timer > 0 )
        cpu->timer.TIMA = cpu->timer.TMA;
}

// 0xff06
uint16_t read_TMA ( _cpu_info *cpu ) {
    if ( debug_timer ) printf("TMA was read\n");
    return cpu->timer.TMA;
}

// 0xff05
void write_TIMA ( _cpu_info *cpu, uint16_t data ) {
    if ( debug_timer ) printf("TIMA changed %2x to %2x, was: " , cpu->timer.TIMA, data);
    if ( debug_timer ) print_timer_state ( cpu );

    if ( cpu->timer.TIMA_timer == 0 ) {
        cpu->timer.TIMA = data;

        cpu->timer.TIMA_reload_timer = 0;
    }
}

// 0xff05
uint16_t read_TIMA ( _cpu_info *cpu ) {
    if ( debug_timer ) printf("TIMA was read: ");
    if ( debug_timer ) print_timer_state ( cpu );
    return cpu->timer.TIMA;
}

void reset_TIMA ( _cpu_info *cpu ) {
    cpu->timer.TIMA = cpu->timer.TMA;
    /*write_TIMA(cpu, read_TMA(cpu));*/
}

// This is the glitch documented on AntonioND's that
// makes the TIMA increase when writing to DIV
// if the bit associated to TAC was 1. This happens
// due to the falling edge detector
void write_DIV( _cpu_info *cpu, uint16_t data ) {
    if ( cpu->timer.TAC & 0x04 )
    switch ( cpu->timer.TAC & 0x03 ) {
        case 0x00: // 00: 4096   Hz ( Increase every 1024 clocks )
            if ( cpu->timer._timer & 0x0200 ) {
                cpu->timer.TIMA ++;
            }
            break;
        case 0x01: // 01: 262144 Hz ( 16 clocks  )
            if ( cpu->timer._timer & 0x0008 ) {
                cpu->timer.TIMA ++;
            }
            break;
        case 0x02: // 10: 65536  Hz ( 64 clocks  )
            if ( cpu->timer._timer & 0x0020 ) {
                cpu->timer.TIMA ++;
            }
            break;
        case 0x03: // 11: 16386  Hz ( 256 clocks )
            if ( cpu->timer._timer & 0x0080 ) {
                cpu->timer.TIMA ++;
            }
            break;
        default:
            assert ( 0 && "Invalid value for TAC when writing to DIV\n");
            break;
    }

    /*if ( cpu->timer.TIMA > 0xff ) {*/
        /*cpu->timer.TIMA_timer = 1;*/
        /*cpu->interrupts.pending_timer = 1; // Maybe it is dealyed because the cpu cant poll it before*/
        /*[>cpu->timer.TIMA_reload_timer = 1;<]*/
        /*cpu->timer.TIMA = 0x00;*/
    /*}*/

    if ( cpu->timer.TIMA > 0xff ) {
        cpu->timer.TIMA_reload_timer = 4;
        cpu->interrupts.pending_timer = 1;
        cpu->timer.TIMA = 0x00;

        if ( debug_timer ) printf("TIMA RESET: ");
        if ( debug_timer ) print_timer_state ( cpu );
    }

    /*if ( cpu->timer.TIMA_reload_timer > 0 ) {*/
        /*cpu->timer.TIMA_reload_timer--;*/
        /*if ( cpu->timer.TIMA_reload_timer == 0 ) {*/
            /*reset_TIMA ( cpu );*/
            /*cpu->timer.TIMA_timer = 1;*/
        /*}*/
    /*}*/

    if ( debug_timer ) printf("DIV was reset");

    cpu->timer._timer_old = cpu->timer._timer;
    cpu->timer._timer     = data - data; // Just to use the parameter and remove and warning
    cpu->timer.DIV        = cpu->timer._timer >> 8 & 0xff;
}

uint8_t read_DIV ( _cpu_info *cpu ) {
    if ( debug_timer ) printf("DIV was read: ");
    // Making sure it has the correcrt value
    assert ( cpu->timer.DIV == ((cpu->timer._timer >> 8) & 0xff ));
    return cpu->timer.DIV;
}

void timer_tick_and_full_mcycle ( _cpu_info *cpu ) {
    dma_step ( cpu );

    for (int i = 0; i < 4; ++i) {
        timer_update   ( cpu );
        display_update ( cpu );
        apu_update     ( cpu );
        apu_update_on_div_change ( cpu );
    }

    input_update   ( cpu );
}

void timer_update( _cpu_info *cpu ) {
    // Timer Increases each T-CYCLE ( 4 per M-CYCLE );
    // DIV is the upper 8 (MSB) bits, and it increases every 256 t-cycles
    // or 64 NOPs

    if ( debug_timer ) print_timer_state ( cpu );

    static int8_t mcycle_bump = 0;
    cpu->cycles_clock    += 1;
    cpu->timer._timer_old = cpu->timer._timer;
    cpu->timer._timer    += 1;

    mcycle_bump += 1;

    if ( mcycle_bump == 4 ) {
        cpu->cycles_machine += 1;
        mcycle_bump = 0;
        /*print_timer_state ( cpu );*/
    }

    if ( cpu->timer.TIMA_timer > 0 ) {
        cpu->timer.TIMA_timer --;
    }

    if ( cpu->timer.TIMA_reload_timer > 0 ) {
        cpu->timer.TIMA_reload_timer--;
        if ( cpu->timer.TIMA_reload_timer == 0 ) {
            reset_TIMA ( cpu );
            cpu->timer.TIMA_timer = 1;
        }
    }

    cpu->timer.DIV = cpu->timer._timer >> 8 & 0xff;

    if ( cpu->timer.TAC & 0x04 ) // If timer is running
    switch ( cpu->timer.TAC & 0x03 ) {
        case 0x00: // 00: 4096   Hz ( Increase every 1024 clocks )
            if (!(cpu->timer._timer     & 0x0200 ) &&
                  cpu->timer._timer_old & 0x0200 ) {
                cpu->timer.TIMA ++;
            }
            break;
        case 0x01: // 01: 262144 Hz ( 16 clocks  )
            if (!(cpu->timer._timer     & 0x0008 ) &&
                  cpu->timer._timer_old & 0x0008 ) {
                cpu->timer.TIMA ++;
            }
            break;
        case 0x02: // 10: 65536  Hz ( 64 clocks  )
            if (!(cpu->timer._timer     & 0x0020 ) &&
                  cpu->timer._timer_old & 0x0020 ) {
                cpu->timer.TIMA ++;
            }
            break;
        case 0x03: // 11: 16386  Hz ( 256 clocks )
            if (!(cpu->timer._timer     & 0x0080 ) &&
                  cpu->timer._timer_old & 0x0080 ) {
                cpu->timer.TIMA ++;
            }
            break;
        default:
            assert ( 0 && "Invalid value for TAC\n");
            break;
    }

    if ( cpu->timer.TIMA > 0xff ) {
        cpu->timer.TIMA_reload_timer = 4;
        cpu->interrupts.pending_timer = 1;
        cpu->timer.TIMA = 0x00;

        if ( debug_timer ) printf("TIMA RESET: ");
        if ( debug_timer ) print_timer_state ( cpu );
    }
}

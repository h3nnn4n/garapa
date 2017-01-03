#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "types.h"
#include "time_keeper.h"
#include "memory.h"
#include "display.h"
#include "graphics.h"

// 0xff07
void write_TAC ( _cpu_info *cpu, uint8_t data ) {
    cpu->timer.running = data & 0x04;

    if ( (cpu->timer.TAC & 0x04) && !(data & 0x04) )
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
        /*cpu->interrupts.pending_timer = 1;*/
        /*reset_TIMA ( cpu );*/
    /*}*/
    if ( cpu->timer.TIMA > 0xff ) {
        cpu->timer.TIMA_reset_delay = 1;
        cpu->interrupts.pending_timer = 1; // Maybe it is dealyed because the cpu cant poll it before
        /*cpu->timer.TIMA_write_block = 1;*/
        cpu->timer.TIMA = 0x00;
    }

    cpu->timer.TAC     = data;
}

// 0xff07
uint8_t read_TAC ( _cpu_info *cpu ) {
    return cpu->timer.TAC;
}

// 0xff06
void write_TMA ( _cpu_info *cpu, uint16_t data ) {
    cpu->timer.TMA = data;
}

// 0xff06
uint16_t read_TMA ( _cpu_info *cpu ) {
    return cpu->timer.TMA;
}

// 0xff05
void write_TIMA ( _cpu_info *cpu, uint16_t data ) {
    /*if ( cpu->timer.TIMA_write_block == 1 ) {*/
        /*[>reset_TIMA ( cpu );<]*/
        /*cpu->timer.TIMA = cpu->timer.TMA;*/
        /*return;*/
    /*}*/
    cpu->timer.TIMA = data;
}

// 0xff05
uint16_t read_TIMA ( _cpu_info *cpu ) {
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

    if ( cpu->timer.TIMA > 0xff ) {
        cpu->interrupts.pending_timer = 1;
        reset_TIMA ( cpu );
    }

    cpu->timer._timer = 0;
    cpu->timer.DIV    = cpu->timer._timer >> 8 & 0xff;
}

uint8_t read_DIV ( _cpu_info *cpu ) {
    // Making sure it has the correcrt value
    assert ( cpu->timer.DIV == ((cpu->timer._timer >> 8) & 0xff ));
    return cpu->timer.DIV;
}

void timer_tick_and_full_mcycle ( _cpu_info *cpu ) {
    timer_bump     ( cpu );
    timer_update   ( cpu );
    display_update ( cpu );

    input_update   ( cpu );
}

void timer_bump ( _cpu_info *cpu ) {
    cpu->cycles_machine  += 1;
    cpu->cycles_clock    += 4;
    cpu->timer._timer_old = cpu->timer._timer;
    cpu->timer._timer    += 4;
}

void timer_update( _cpu_info *cpu ) {
    // Timer Increases each 4 T-CYCLES ( 1 M-CYCLE );
    // DIV is 8 MSB of Timer, so it increases each 256 tcycles;
    // DIV increases each 64 M-CYCLES


    /*if ( cpu->timer.TIMA_write_block == 1 ) {*/
        /*cpu->timer.TIMA_write_block = 0;*/
    /*}*/

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

    if ( cpu->timer.TIMA_reset_delay == 1 ) {
        cpu->timer.TIMA_reset_delay = 0;
        /*cpu->interrupts.pending_timer = 1; // The Interrupt is also delayed one m-cycle*/
        reset_TIMA ( cpu );
    }

    if ( cpu->timer.TIMA > 0xff ) {
        cpu->timer.TIMA_reset_delay = 1;
        cpu->interrupts.pending_timer = 1; // Maybe it is dealyed because the cpu cant poll it before
        /*cpu->timer.TIMA_write_block = 1;*/
        cpu->timer.TIMA = 0x00;
    }
}

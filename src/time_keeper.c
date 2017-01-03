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

void write_TAC ( _cpu_info *cpu, uint8_t data ) {
    cpu->timer.running = data & 0x03;
    cpu->timer.TAC     = data;

    switch ( data & 0x03 ) {
        case 0x00:
            cpu->timer.speed = 64;
            break;
        case 0x01:
            cpu->timer.speed = 1 ;
            break;
        case 0x02:
            cpu->timer.speed = 4 ;
            break;
        case 0x03:
            cpu->timer.speed = 16 ;
            break;
        default:
            assert ( 0 && "TAC WRITE, invalid data" );
    }
}

uint8_t read_TAC ( _cpu_info *cpu ) {
    return cpu->timer.TAC;
}

void write_TMA ( _cpu_info *cpu, uint16_t data ) {
    cpu->timer.TMA = data;
}

uint16_t read_TMA ( _cpu_info *cpu ) {
    return cpu->timer.TMA;
}

void write_TIMA ( _cpu_info *cpu, uint16_t data ) {
    cpu->timer.TIMA = data;
}

uint16_t read_TIMA ( _cpu_info *cpu ) {
    return cpu->timer.TIMA;
}

void reset_TIMA ( _cpu_info *cpu ) {
    write_TIMA(cpu, read_TMA(cpu));
}

void write_DIV( _cpu_info *cpu, uint16_t data ) {
    cpu->timer._timer = 0;
}

uint8_t read_DIV ( _cpu_info *cpu ) {
    // Making sure it has the correcrt value
    assert ( cpu->timer.DIV == ((cpu->timer._timer >> 8) && 0xff ));
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

    cpu->timer.DIV = cpu->timer._timer >> 8 & 0xff;

    if ( cpu->timer.TAC & 0x04 )
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
        cpu->interrupts.pending_timer = 1;
        reset_TIMA ( cpu );
    }
}

/*void timer_update( _cpu_info *cpu ) {*/
    /*cpu->timer._delta = cpu->cycles_machine - cpu->timer._old_clock;*/
    /*cpu->timer._old_clock = cpu->cycles_machine;*/

    /*[>if ( cpu->timer._delta == 1 ) print_timer_status ( cpu );<]*/

    /*[>assert ( cpu->timer._delta == 1 && 1 );//"Timer should only be updated once per mcycle!");<]*/

    /*[>if ( read_TAC(cpu) %0x4)<]*/

    /*cpu->timer._elapsed += cpu->timer._delta * 4;*/

    /*if ( cpu->timer._elapsed >= 16 ) {*/
        /*cpu->timer._ticks     += 1;*/

        /*if ( cpu->timer._ticks == 16 ) {*/
            /*cpu->timer._ticks = 0;*/
            /*cpu->timer.DIV += 1;*/
            /*[>printf("MEU Divider tick %4x\n", cpu->timer.DIV);<]*/
        /*}*/

        /*if ( read_TAC(cpu) & 0x4 ) {*/
            /*if ( cpu->timer._ticks == cpu->timer.speed ) {*/
                /*cpu->timer._ticks = 0;*/
                /*write_TIMA(cpu, read_TIMA(cpu) + 1);*/
            /*}*/

            /*if ( read_TIMA(cpu) == 0x100 ) {*/
                /*[>printf("MEU: Threw timer int\n");<]*/
                /*cpu->interrupts.pending_timer = 1;*/
                /*[>emulate_INTERRUPT( cpu );<]*/
                /*reset_TIMA(cpu);*/
                /*[>cpu->halted     = 0;<]*/
            /*}*/
        /*}*/

        /*cpu->timer._elapsed -= 16;*/
    /*}*/
/*}*/

/*void print_timer_status ( _cpu_info *cpu ) {*/
    /*printf("running: %c speed: %2d elapsed: %3d delta: %3d ticks: %2d TIMA: %4d %4x\n",*/
            /*(read_TAC(cpu) & 0x04) ? 'y':'n',*/
            /*read_TAC(cpu) & 0x03,*/
            /*cpu->timer._elapsed,*/
            /*cpu->timer._delta,*/
            /*cpu->timer._ticks,*/
            /*read_TIMA(cpu),*/
            /*read_TIMA(cpu)*/
          /*);*/
/*}*/

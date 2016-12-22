#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "types.h"
#include "time_keeper.h"

// timekeeper is the emulator breaks, to keep it to running WAY TOO FAST

struct timespec time_diff(struct timespec start, struct timespec end) {
    struct timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}

void timekeeper_tic  (struct timespec *t1) {
    clock_gettime(CLOCK_MONOTONIC, t1);
}

void timekeeper_wait (struct timespec *t1, struct timespec *t2) {
    clock_gettime(CLOCK_MONOTONIC, t2);
    struct timespec t3;
    t3.tv_nsec = 16667000 - (time_diff(*t1, *t2).tv_nsec);
    t3.tv_sec  = 0;
    nanosleep( &t3, NULL );
    timekeeper_tic(t1);
}

// Here is the actual CPU timer routines

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
            cpu->timer.speed = 2 ;
            break;
        case 0x03:
            cpu->timer.speed = 4 ;
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
    cpu->timer.DIV = data ^ data;   // Every write sets DIV to zero;
}

uint8_t read_DIV ( _cpu_info *cpu ) {
    return cpu->timer.DIV;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void timer_tick ( _cpu_info *cpu ) {
    /*cpu->cycles_machine++;*/
    /*cpu->cycles_left--;*/
}

void timer_update( _cpu_info *cpu ) {
    static uint16_t old_clock = 0;
    static uint16_t elapsed   = 0;
    static uint16_t ticks     = 0;

    uint16_t delta = cpu->cycles_machine - old_clock;
    old_clock = cpu->cycles_machine;

    elapsed += delta * 4;

    /*if ( read_TAC(cpu) %0x4)*/
    /*printf("running: %c speed: %2d elapsed: %3d delta: %3d ticks: %2d TIMA: %4d %4x\n",*/
            /*(read_TAC(cpu) & 0x04) ? 'y':'n',*/
            /*read_TAC(cpu) & 0x03,*/
            /*elapsed,*/
            /*delta,*/
            /*ticks,*/
            /*read_TIMA(cpu),*/
            /*read_TIMA(cpu)*/
          /*);*/

    if ( elapsed >= 16 ) {
        ticks     += 1;

        if ( ticks == 16 ) {
            ticks = 0;
            cpu->timer.DIV += 1;
            /*printf("MEU Divider tick %4x\n", cpu->timer.DIV);*/
        }

        if ( read_TAC(cpu) & 0x4 ) {
            if ( ticks == cpu->timer.speed ) {
                ticks = 0;
                write_TIMA(cpu, read_TIMA(cpu) + 1);
            }

            if ( read_TIMA(cpu) == 0x100 ) {
                /*printf("MEU: Threw timer int\n");*/
                cpu->interrupts.pending_timer = 1;
                /*emulate_INTERRUPT( cpu );*/
                reset_TIMA(cpu);
                /*cpu->halted     = 0;*/
            }
        }

        elapsed -= 16;
    }
}

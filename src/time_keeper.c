#include <time.h>
#include <stdio.h>
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
            printf(" Timer speed changed to 64\n");
            cpu->timer.speed = 64;
            break;
        case 0x01:
            printf(" Timer speed changed to 1\n");
            cpu->timer.speed = 1 ;
            break;
        case 0x02:
            printf(" Timer speed changed to 2\n");
            cpu->timer.speed = 2 ;
            break;
        case 0x03:
            printf(" Timer speed changed to 4\n");
            cpu->timer.speed = 4 ;
            break;
        default:
            assert ( 0 && "TAC WRITE, invalid data" );
    }
}

uint8_t read_TAC ( _cpu_info *cpu ) {
    return cpu->timer.TAC;
}

void write_TMA ( _cpu_info *cpu, uint8_t data ) {
    cpu->timer.TMA = data;
}

uint8_t read_TMA ( _cpu_info *cpu ) {
    return cpu->timer.TMA;
}

void write_TIMA ( _cpu_info *cpu, uint8_t data ) {
    cpu->timer.TIMA = data;
}

uint8_t read_TIMA ( _cpu_info *cpu ) {
    return cpu->timer.TIMA;
}

void write_DIV( _cpu_info *cpu, uint8_t data ) {
    cpu->timer.DIV = data ^ data;   // Every write sets DIV to zero;
}

uint8_t read_DIV ( _cpu_info *cpu ) {
    return cpu->timer.DIV;
}

void timer_tick ( _cpu_info *cpu ) {
    cpu->cycles_machine++;
    cpu->cycles_left--;
}

void timer_update( _cpu_info *cpu ) {
    static uint16_t old_clock = 0;
    static uint16_t elapsed   = 0;
    static uint16_t ticks     = 0;

    uint16_t delta = cpu->cycles_machine - old_clock;
    old_clock = cpu->cycles_machine;

    elapsed += delta * 4;

    if ( elapsed >= 16 ) {
        ticks     += 1;

        if ( ticks == 16 ) {
            ticks = 0;
            cpu->timer.DIV += 1;
        }

        if ( cpu->timer.running ) {
            if ( ticks == cpu->timer.speed ) {
                ticks = 0;
                cpu->timer.TIMA += 1;
            }

            if ( cpu->timer.TIMA == 0x100 ) {
                cpu->interrupts.pending_timer = 1;
                cpu->timer.TIMA = cpu->timer.TMA;
                cpu->halted     = 0;
            }
        }

        elapsed -= 16;
    }
}

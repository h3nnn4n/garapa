#ifndef TIME_KEEPER_H
#define TIME_KEEPER_H

#include <time.h>
#include <unistd.h>

struct timespec time_diff( struct timespec start, struct timespec end );
void     timekeeper_tic  ( struct timespec *t1                        );
void     timekeeper_wait ( struct timespec *t1  , struct timespec *t2 );

void write_TAC  ( _cpu_info *cpu, uint8_t  data ) ;
void write_TMA  ( _cpu_info *cpu, uint16_t data ) ;
void write_TIMA ( _cpu_info *cpu, uint16_t data ) ;
void write_DIV  ( _cpu_info *cpu, uint16_t data ) ;

void timer_update ( _cpu_info *cpu ) ;
void timer_bump   ( _cpu_info *cpu ) ;
void timer_tick_and_full_mcycle ( _cpu_info *cpu ) ;

uint8_t   read_TAC  ( _cpu_info *cpu ) ;
uint16_t  read_TMA  ( _cpu_info *cpu ) ;
uint16_t  read_TIMA ( _cpu_info *cpu ) ;
uint8_t   read_DIV  ( _cpu_info *cpu ) ;
void     reset_TIMA ( _cpu_info *cpu ) ;

#endif /* TIME_KEEPER_H */

#ifndef EMULATOR_H
#define EMULATOR_H

#include "types.h"

void emulator          ( _cpu_info *cpu ) ;
void emulate_INTERRUPT ( _cpu_info *cpu ) ;
void print_registers   ( _cpu_info *cpu ) ;

#endif /* EMULATOR_H */

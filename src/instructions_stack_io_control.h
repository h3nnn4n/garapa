#ifndef INSTRUCTIONS_STACK_IO_CONTROL_H
#define INSTRUCTIONS_STACK_IO_CONTROL_H

#include "types.h"

void emulate_PUSH      ( _cpu_info *cpu );
void emulate_POP       ( _cpu_info *cpu );
void emulate_XTHL      ( _cpu_info *cpu );
void emulate_SPHL      ( _cpu_info *cpu );
void emulate_IN        ( _cpu_info *cpu );
void emulate_OUT       ( _cpu_info *cpu );
void emulate_EI        ( _cpu_info *cpu );
void emulate_DI        ( _cpu_info *cpu );
void emulate_NOP       ( _cpu_info *cpu );
void emulate_INTERRUPT ( _cpu_info *cpu );

#endif /* INSTRUCTIONS_STACK_IO_CONTROL_H */

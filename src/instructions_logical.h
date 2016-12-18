#ifndef INSTRUCTIONS_LOGICAL_H
#define INSTRUCTIONS_LOGICAL_H

#include "types.h"

void emulate_ANA ( _cpu_info *cpu );
void emulate_ANI ( _cpu_info *cpu );
void emulate_XRA ( _cpu_info *cpu );
void emulate_XRI ( _cpu_info *cpu );
void emulate_ORA ( _cpu_info *cpu );
void emulate_ORI ( _cpu_info *cpu );
void emulate_CMP ( _cpu_info *cpu );
void emulate_CPI ( _cpu_info *cpu );
void emulate_RAL ( _cpu_info *cpu );
void emulate_RAR ( _cpu_info *cpu );
void emulate_CMA ( _cpu_info *cpu );
void emulate_CMC ( _cpu_info *cpu );
void emulate_STC ( _cpu_info *cpu );

#endif /* INSTRUCTIONS_LOGICAL_H */

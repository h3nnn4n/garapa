#ifndef INSTRUCTIONS_ARITHMETIC_H
#define INSTRUCTIONS_ARITHMETIC_H

#include "types.h"

void emulate_ADD ( _cpu_info *cpu );
void emulate_ADI ( _cpu_info *cpu );
void emulate_ADC ( _cpu_info *cpu );
void emulate_ACI ( _cpu_info *cpu );
void emulate_SUB ( _cpu_info *cpu );
void emulate_SUI ( _cpu_info *cpu );
void emulate_SBB ( _cpu_info *cpu );
void emulate_SBI ( _cpu_info *cpu );
void emulate_INR ( _cpu_info *cpu );
void emulate_DCR ( _cpu_info *cpu );
void emulate_INC ( _cpu_info *cpu );
void emulate_DCX ( _cpu_info *cpu );
void emulate_DAD ( _cpu_info *cpu );
void emulate_DAA ( _cpu_info *cpu );

#endif /* INSTRUCTIONS_ARITHMETIC_H */

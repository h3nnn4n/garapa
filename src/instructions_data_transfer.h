#ifndef INSTRUCTIONS_DATA_TRANSFER_H
#define INSTRUCTIONS_DATA_TRANSFER_H

#include <stdint.h>
#include <assert.h>

#include "types.h"

uint8_t *get_reg_ref  ( _cpu_info *cpu, uint8_t opcode );
void     emulate_XCHG ( _cpu_info *cpu );
void     emulate_LDAX ( _cpu_info *cpu );
void     emulate_STAX ( _cpu_info *cpu );
void     emulate_SHLD ( _cpu_info *cpu );
void     emulate_LHLD ( _cpu_info *cpu );
void     emulate_LDA  ( _cpu_info *cpu );
void     emulate_STA  ( _cpu_info *cpu );
void     emulate_LXI  ( _cpu_info *cpu );
void     emulate_MVI  ( _cpu_info *cpu );
void     emulate_MOV  ( _cpu_info *cpu );

#endif /* INSTRUCTIONS_DATA_TRANSFER_H */

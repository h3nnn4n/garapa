#ifndef MICROCODE_H
#define MICROCODE_H

#include "types.h"

uint8_t read_byte_at_pc      ( _cpu_info *cpu                ) ;
uint8_t read_byte_with_tick  ( _cpu_info *cpu, uint16_t addr ) ;
void    write_byte_with_tick ( _cpu_info *cpu, uint16_t addr ) ;

#endif /* MICROCODE_H */

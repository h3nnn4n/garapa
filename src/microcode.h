#ifndef MICROCODE_H
#define MICROCODE_H

#include "types.h"

uint16_t read_bc              ( _cpu_info *cpu                              ) ;
uint16_t read_de              ( _cpu_info *cpu                              ) ;
uint16_t read_hl              ( _cpu_info *cpu                              ) ;
uint8_t  read_byte_at_sp      ( _cpu_info *cpu                              ) ;
uint8_t  read_byte_at_pc      ( _cpu_info *cpu                              ) ;
uint8_t  read_byte_with_tick  ( _cpu_info *cpu, uint16_t addr               ) ;
void     write_byte_at_sp     ( _cpu_info *cpu, uint8_t data                ) ;
void     write_byte_with_tick ( _cpu_info *cpu, uint16_t addr, uint8_t data ) ;

void write_hl ( _cpu_info *cpu, uint8_t lo, uint8_t hi ) ;
void write_hl_16 ( _cpu_info *cpu, uint16_t data ) ;

#endif /* MICROCODE_H */

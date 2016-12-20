#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"

uint8_t interrupt_read_IF    ( _cpu_info* cpu               ) ;
void    interrupt_write_IF   ( _cpu_info *cpu, uint8_t mask ) ;
uint8_t interrupt_read_mask  ( _cpu_info *cpu               ) ;
void    interrupt_write_mask ( _cpu_info *cpu, uint8_t mask ) ;

#endif /* INTERRUPTS_H */

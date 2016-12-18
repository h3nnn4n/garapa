#ifndef INSTRUCTIONS_0XCB_H
#define INSTRUCTIONS_0XCB_H

#include <stdint.h>

#include "types.h"

void emulate_RR    ( _cpu_info *cpu, uint8_t target ) ;
void emulate_RLC   ( _cpu_info *cpu, uint8_t target ) ;
void emulate_RRC   ( _cpu_info *cpu, uint8_t target ) ;
void emulate_RL    ( _cpu_info *cpu, uint8_t target ) ;
void emulate_SLA   ( _cpu_info *cpu, uint8_t target ) ;
void emulate_SRA   ( _cpu_info *cpu, uint8_t target ) ;
void emulate_SWAP  ( _cpu_info *cpu, uint8_t target ) ;
void emulate_SRL   ( _cpu_info *cpu, uint8_t target ) ;
void decode_0xcb   ( _cpu_info *cpu                 ) ;

#endif /* INSTRUCTIONS_0XCB_H */

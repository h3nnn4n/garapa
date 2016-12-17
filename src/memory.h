#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

void     load_rom       ( _cpu_info *cpu     , const char* fname , uint16_t offset ) ;
void     write_byte     ( _cpu_info *cpu     , uint16_t    addr  , uint8_t  data   ) ;
uint16_t read_next_word ( _cpu_info *cpu                                           ) ;
uint16_t read_word      ( _cpu_info *cpu     , uint16_t    addr                    ) ;
uint8_t  read_byte      ( _cpu_info *cpu     , uint16_t    addr                    ) ;

#endif /* MEMORY_H */

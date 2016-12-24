#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "types.h"

void     print_rom_info ( _cpu_info *cpu ) ;
uint32_t get_rom_size   ( uint8_t *addr  ) ;
uint32_t get_ram_size   ( uint8_t *addr  ) ;
uint8_t  get_ram_code   ( uint8_t *addr  ) ;
uint8_t  get_rom_code   ( uint8_t *addr  ) ;

#endif /* CARTRIDGE_H */

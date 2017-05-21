#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

#include "audio_types.h"
#include "types.h"

uint8_t apu_read_byte ( _cpu_info *cpu, uint16_t addr );
void apu_write_byte ( _cpu_info *cpu, uint16_t addr, uint8_t data );

#endif /* AUDIO_H */

#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

#include "audio_types.h"
#include "types.h"

uint8_t apu_read_byte ( _cpu_info *cpu, uint16_t addr );
void apu_write_byte ( _cpu_info *cpu, uint16_t addr, uint8_t data );

void apu_clear ( _cpu_info *cpu );
void apu_reset ( _cpu_info *cpu );

void apu_ch1_reset ( _cpu_info *cpu );
void apu_ch2_reset ( _cpu_info *cpu );
void apu_ch3_reset ( _cpu_info *cpu );
void apu_ch4_reset ( _cpu_info *cpu );

void apu_ch1_clear ( _cpu_info *cpu );
void apu_ch2_clear ( _cpu_info *cpu );
void apu_ch3_clear ( _cpu_info *cpu );
void apu_ch4_clear ( _cpu_info *cpu );

#endif /* AUDIO_H */
#ifndef CH2_H
#define CH2_H

#include "types.h"

void apu_ch2_reset         ( _cpu_info *cpu );
void apu_ch2_clear         ( _cpu_info *cpu );
void apu_ch2_step          ( _cpu_info *cpu );
uint16_t apu_ch2_sample    ( _cpu_info *cpu );
uint8_t apu_is_ch2_enabled ( _cpu_info *cpu );
void apu_ch2_step_length   ( _cpu_info *cpu );
void apu_ch2_step_volume   ( _cpu_info *cpu );
void apu_ch2_trigger       ( _cpu_info *cpu );

#endif /* CH2_H */

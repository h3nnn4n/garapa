#ifndef CH3_H
#define CH3_H

#include <stdint.h>

#include "types.h"

void apu_ch3_reset         ( _cpu_info *cpu );
void apu_ch3_clear         ( _cpu_info *cpu );
void apu_ch3_step          ( _cpu_info *cpu );
uint16_t apu_ch3_sample    ( _cpu_info *cpu );
uint8_t apu_is_ch3_enabled ( _cpu_info *cpu );
void apu_ch3_step_length   ( _cpu_info *cpu );

#endif /* CH3_H */

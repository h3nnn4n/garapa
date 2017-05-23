#ifndef CH4_H
#define CH4_H

#include <stdint.h>
#include "types.h"

void apu_ch4_reset         ( _cpu_info *cpu );
void apu_ch4_clear         ( _cpu_info *cpu );
void apu_ch4_step          ( _cpu_info *cpu );
uint16_t apu_ch4_sample    ( _cpu_info *cpu );
uint8_t apu_is_ch4_enabled ( _cpu_info *cpu );
void apu_ch4_step_length   ( _cpu_info *cpu );
void apu_ch4_step_volume   ( _cpu_info *cpu );
void apu_ch4_trigger       ( _cpu_info *cpu );
uint16_t get_divisor       ( uint8_t index  );

#endif /* CH4_H */

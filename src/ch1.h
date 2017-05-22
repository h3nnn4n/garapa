#ifndef CH1_H
#define CH1_H

#include <stdint.h>

#include "types.h"

void apu_ch1_reset          ( _cpu_info *cpu );
void apu_ch1_clear          ( _cpu_info *cpu );
void apu_ch1_step           ( _cpu_info *cpu );
uint16_t apu_ch1_sample     ( _cpu_info *cpu );
uint8_t apu_is_ch1_enabled  ( _cpu_info *cpu );
void apu_ch1_trigger        ( _cpu_info *cpu );
void apu_ch1_step_length    ( _cpu_info *cpu );
void apu_ch1_step_volume    ( _cpu_info *cpu );
void apu_ch1_step_sweep     ( _cpu_info *cpu );
uint16_t apu_ch1_calc_sweep ( _cpu_info *cpu );

#endif /* CH1_H */

#include <stdint.h>
#include "ch4.h"
#include "types.h"

void apu_ch4_reset ( _cpu_info *cpu ){
    apu_ch4_clear( cpu );
    cpu->apu.ch4.length = 0;
}

void apu_ch4_clear ( _cpu_info *cpu ){
    cpu->apu.ch4.enable                = 0;
    cpu->apu.ch4.length_enable         = 0;

    cpu->apu.ch4.volume                = 0;
    cpu->apu.ch4.volume_envl_initial   = 0;
    cpu->apu.ch4.volume_envl_direction = 0;
    cpu->apu.ch4.volume_envl_period    = 0;
    cpu->apu.ch4.volume_envl_timer     = 0;

    cpu->apu.ch4.shift                 = 0;
    cpu->apu.ch4.width                 = 0;
    cpu->apu.ch4.divisor               = 0;
    cpu->apu.ch4.lfsr                  = 0;
}

void apu_ch4_step( _cpu_info *cpu ) {

}

uint16_t apu_ch4_sample( _cpu_info *cpu ) {
    return 0;
}

uint8_t apu_is_ch4_enabled ( _cpu_info *cpu ) {
    return cpu->apu.ch4.enable;
}

void apu_ch4_step_length( _cpu_info *cpu ) {

}

void apu_ch4_step_volume( _cpu_info *cpu ) {

}


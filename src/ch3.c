#include <stdint.h>

#include "ch3.h"
#include "types.h"

void apu_ch3_reset ( _cpu_info *cpu ){
    cpu->apu.ch3.wave_ram[0]  = 0x84;
    cpu->apu.ch3.wave_ram[1]  = 0x40;
    cpu->apu.ch3.wave_ram[2]  = 0x43;
    cpu->apu.ch3.wave_ram[3]  = 0xaa;
    cpu->apu.ch3.wave_ram[4]  = 0x2d;
    cpu->apu.ch3.wave_ram[5]  = 0x78;
    cpu->apu.ch3.wave_ram[6]  = 0x92;
    cpu->apu.ch3.wave_ram[7]  = 0x3c;
    cpu->apu.ch3.wave_ram[8]  = 0x60;
    cpu->apu.ch3.wave_ram[9]  = 0x59;
    cpu->apu.ch3.wave_ram[10] = 0x59;
    cpu->apu.ch3.wave_ram[11] = 0xb0;
    cpu->apu.ch3.wave_ram[12] = 0x34;
    cpu->apu.ch3.wave_ram[13] = 0xb8;
    cpu->apu.ch3.wave_ram[14] = 0x2e;
    cpu->apu.ch3.wave_ram[15] = 0xda;

    cpu->apu.ch3.length = 0;
    apu_ch3_clear( cpu );
}

void apu_ch3_clear ( _cpu_info *cpu ){
    cpu->apu.ch3.enable                = 0;
    cpu->apu.ch3.dac_enable            = 0;

    cpu->apu.ch3.length_enable         = 0;

    cpu->apu.ch3.volume                = 0;
    cpu->apu.ch3.frequency             = 0;
    cpu->apu.ch3.timer                 = 0;

    cpu->apu.ch3.wave_ram_buffer       = 0;
    cpu->apu.ch3.wave_ram_buffer_position = 0;
}

void apu_ch3_step( _cpu_info *cpu ) {

}

uint16_t apu_ch3_sample( _cpu_info *cpu ) {
    return 0;
}

uint8_t apu_is_ch3_enabled ( _cpu_info *cpu ) {
    return cpu->apu.ch3.enable;
}

void apu_ch3_step_length( _cpu_info *cpu ) {

}

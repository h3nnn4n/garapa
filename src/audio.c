#include <stdint.h>

#include "audio_types.h"
#include "audio.h"
#include "types.h"

uint8_t apu_read_byte ( _cpu_info *cpu, uint16_t addr ) {
    switch ( addr ) {
        // Ch1 sweep [-PPP NSSS] = sweep Period, Negate, Shift
        case 0xff10:
            return ( ( cpu->apu.ch1.sweep_period << 4 ) | ( ((cpu->apu.ch1.sweep_direction ? 1 : 0) << 3) ) | ( cpu->apu.ch1.sweep_shift ) | 0x80 );

        // Ch1 sound lenght and wave patter duty
        // [DDLL LLLL] Duty, Lenght
        case 0xff11:
            return ( cpu->apu.ch1.wave_pattern_duty << 6 ) | 0x3f;

        // Ch1 Volume envelope
        // [VVVV APPP] starting Volume, Add mode, Period
        case 0xff12:
            return ( ( cpu->apu.ch1.volume_envl_initial << 4 ) | ( ((cpu->apu.ch1.volume_envl_direction ? 1 : 0) << 3) ) | ( cpu->apu.ch1.volume_envl_period ) );

        // Ch1 Frequency LSB, W only
        case 0xff13:
            return 0xff;

        // Ch1 misc
        // [TL-- -FFF] Trigger, Length enable, Frequensy MSB
        case 0xff14:
            return ( ((cpu->apu.ch1.length_enable ? 1 : 0) << 6) ) | 0xbf;

        default:
            break;
    }

    return 0;
}

void apu_write_byte ( _cpu_info *cpu, uint16_t addr, uint8_t data ){
    switch ( addr ) {
        default:
            break;
    }
}

void apu_reset ( _cpu_info *cpu ) {
    cpu->apu.buffer_index     = 0;
    cpu->apu.sample_timer     = 0;

    apu_ch1_reset( cpu );
    apu_ch2_reset( cpu );
    apu_ch3_reset( cpu );
    apu_ch4_reset( cpu );

    apu_clear( cpu );

    cpu->apu.enable           = 1;

    cpu->apu.left_volume      = 0x07;
    cpu->apu.right_volume     = 0x07;

    cpu->apu.ch1_left_enable  = 1;
    cpu->apu.ch2_left_enable  = 1;
    cpu->apu.ch3_left_enable  = 1;
    cpu->apu.ch4_left_enable  = 1;

    cpu->apu.ch1_right_enable = 1;
    cpu->apu.ch2_right_enable = 1;
    cpu->apu.ch3_right_enable = 0;
    cpu->apu.ch4_right_enable = 0;
}

void apu_clear ( _cpu_info *cpu ) {
    cpu->apu.enable           = 0;
    cpu->apu.frame_seq_step   = 0;

    cpu->apu.left_volume      = 0x00;
    cpu->apu.right_volume     = 0x00;

    cpu->apu.right_vin_enable = 0;
    cpu->apu.left_vin_enable  = 0;

    cpu->apu.ch1_left_enable  = 0;
    cpu->apu.ch2_left_enable  = 0;
    cpu->apu.ch3_left_enable  = 0;
    cpu->apu.ch4_left_enable  = 0;

    cpu->apu.ch1_right_enable = 0;
    cpu->apu.ch2_right_enable = 0;
    cpu->apu.ch3_right_enable = 0;
    cpu->apu.ch4_right_enable = 0;

}

// CH reset functions

void apu_ch1_reset ( _cpu_info *cpu ){
    apu_ch1_clear( cpu );
    cpu->apu.ch1.enable = 1;

    cpu->apu.ch1.length_enable = 0;

    cpu->apu.ch1.wave_pattern_duty = 0x10;

    cpu->apu.ch1.volume_envl_period = 0x11;
    cpu->apu.ch1.volume_envl_initial = 0x0f;

    cpu->apu.ch1.timer = 0x00;
}

void apu_ch2_reset ( _cpu_info *cpu ){
    apu_ch2_clear( cpu );
    cpu->apu.ch2.length = 0;
    cpu->apu.ch2.timer = 0x00;
}

void apu_ch3_reset ( _cpu_info *cpu ){

}

void apu_ch4_reset ( _cpu_info *cpu ){

}

// CH clear functions

void apu_ch1_clear ( _cpu_info *cpu ){
    cpu->apu.ch1.enable                = 0;
    cpu->apu.ch1.sweep_enable          = 0;
    cpu->apu.ch1.sweep_timer           = 0;
    cpu->apu.ch1.frequency_sh          = 0;
    cpu->apu.ch1.sweep_direction       = 0;
    cpu->apu.ch1.sweep_shift           = 0;
    cpu->apu.ch1.sweep_negate_calcd    = 0;

    cpu->apu.ch1.wave_pattern_duty     = 0;
    cpu->apu.ch1.wave_pattern_index    = 0;

    cpu->apu.ch1.length_enable         = 0;

    cpu->apu.ch1.volume                = 0;
    cpu->apu.ch1.volume_envl_initial   = 0;
    cpu->apu.ch1.volume_envl_direction = 0;
    cpu->apu.ch1.volume_envl_period    = 0;
    cpu->apu.ch1.volume_envl_timer     = 0;

    cpu->apu.ch1.timer                &= 0x03; // when reset, the lower 2 bits are not changed
}

void apu_ch2_clear ( _cpu_info *cpu ){
    cpu->apu.ch2.enable                = 0;

    cpu->apu.ch2.wave_pattern_duty     = 0;
    cpu->apu.ch2.wave_pattern_index    = 0;

    cpu->apu.ch2.length_enable         = 0;

    cpu->apu.ch2.volume                = 0;
    cpu->apu.ch2.volume_envl_initial   = 0;
    cpu->apu.ch2.volume_envl_direction = 0;
    cpu->apu.ch2.volume_envl_period    = 0;
    cpu->apu.ch2.volume_envl_timer     = 0;

    cpu->apu.ch2.timer                &= 0x03; // when reset, the lower 2 bits are not changed
}

void apu_ch3_clear ( _cpu_info *cpu ){

}

void apu_ch4_clear ( _cpu_info *cpu ){

}

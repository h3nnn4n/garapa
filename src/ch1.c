#include <stdint.h>
#include <stdio.h>

#include "types.h"
#include "audio_types.h"
#include "audio.h"
#include "ch1.h"

void apu_ch1_reset ( _cpu_info *cpu ){
    apu_ch1_clear( cpu );
    cpu->apu.ch1.enable = 1;

    cpu->apu.ch1.length_enable = 0;

    cpu->apu.ch1.wave_pattern_duty = 0x02;

    cpu->apu.ch1.volume_envl_period = 0x3;
    cpu->apu.ch1.volume_envl_initial = 0x0f;

    cpu->apu.ch1.timer = 0x00;
}

void apu_ch1_clear ( _cpu_info *cpu ){
    cpu->apu.ch1.enable                = 0;
    cpu->apu.ch1.sweep_enable          = 0;
    cpu->apu.ch1.sweep_timer           = 0;
    cpu->apu.ch1.frequency_sh          = 0;
    cpu->apu.ch1.frequency             = 0;
    cpu->apu.ch1.sweep_direction       = 0;
    cpu->apu.ch1.sweep_period          = 0;
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

void apu_ch1_step( _cpu_info *cpu ) {
    if ( cpu->apu.ch1.timer > 0 ) {
        cpu->apu.ch1.timer--;
    }

    if ( cpu->apu.ch1.timer == 0 ) {
        cpu->apu.ch1.wave_pattern_index ++;
        if ( cpu->apu.ch1.wave_pattern_index >= 8 ) {
            cpu->apu.ch1.wave_pattern_index = 0;
        }

        cpu->apu.ch1.timer = ( 2048 - cpu->apu.ch1.frequency ) * 4;
    }
}

// Get channel sample
uint16_t apu_ch1_sample( _cpu_info *cpu ) {
    if ( cpu->apu.ch1.enable ) {

        uint8_t pat = 0;

        switch (cpu->apu.ch1.wave_pattern_duty) {
            case 0x0:
                pat = 0x01 ; // 0b00000001
                break;
            case 0x1:
                pat = 0x81 ; // 0b10000001
                break;
            case 0x2:
                pat = 0x87 ; // 0b10000111
                break;
            case 0x3:
                pat = 0x7e ; // 0b01111110
                break;
        }

        uint8_t bit = (pat & (1 << (7 - cpu->apu.ch1.wave_pattern_index)));

        if ( bit ) {
            return cpu->apu.ch1.volume;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

uint8_t apu_is_ch1_enabled ( _cpu_info *cpu ) {
    return cpu->apu.ch1.enable && (cpu->apu.ch1.volume_envl_initial > 0 || cpu->apu.ch1.volume_envl_direction);
}

void apu_ch1_trigger ( _cpu_info *cpu ) {
    cpu->apu.ch1.enable = 1;

    if ( cpu->apu.ch1.length == 0 ) {
        if ( cpu->apu.ch1.length_enable && ( cpu->apu.frame_seq_step % 2 == 1 ) ) {
            cpu->apu.ch1.length = 63;
        } else {
            cpu->apu.ch1.length = 64;
        }
    }

    cpu->apu.ch1.timer = ( 2048 - cpu->apu.ch1.frequency ) * 4;

    cpu->apu.ch1.volume = cpu->apu.ch1.volume_envl_initial;
    if ( cpu->apu.ch1.volume_envl_period == 0 ) {
        cpu->apu.ch1.volume_envl_timer = 8;
    } else {
        cpu->apu.ch1.volume_envl_timer = cpu->apu.ch1.volume_envl_period;
    }

    if ( cpu->apu.frame_seq_step == 7 ) {
        cpu->apu.ch1.volume_envl_timer++;
    }

    cpu->apu.ch1.frequency_sh = cpu->apu.ch1.frequency;

    if ( cpu->apu.ch1.sweep_period == 0 ) {
        cpu->apu.ch1.sweep_timer = 8;
    } else {
        cpu->apu.ch1.sweep_timer = cpu->apu.ch1.sweep_period;
    }

    cpu->apu.ch1.sweep_enable = cpu->apu.ch1.sweep_period > 0 || cpu->apu.ch1.sweep_shift > 0;
    cpu->apu.ch1.sweep_negate_calcd = 0;

    if ( cpu->apu.ch1.sweep_enable && cpu->apu.ch1.sweep_shift > 0 ) {
        apu_ch1_calc_sweep( cpu );
    }
}

void apu_ch1_step_length ( _cpu_info *cpu ) {
    if ( cpu->apu.ch1.length_enable && cpu->apu.ch1.length > 0 ) {
        cpu->apu.ch1.length -= 1;
        if ( cpu->apu.ch1.length == 0 ) {
            cpu->apu.ch1.enable = 0;
        }
    }
}

void apu_ch1_step_volume ( _cpu_info *cpu ) {
    if ( cpu->apu.ch1.volume_envl_timer > 0 ) {
        cpu->apu.ch1.volume_envl_timer -= 1;
    }

    if ( cpu->apu.ch1.volume_envl_period > 0 && cpu->apu.ch1.volume_envl_timer == 0 ) {
        if ( cpu->apu.ch1.volume_envl_direction ) {
            if ( cpu->apu.ch1.volume < 0xF ) {
                cpu->apu.ch1.volume += 1;
            }
        } else if ( cpu->apu.ch1.volume > 0 ) {
            cpu->apu.ch1.volume -= 1;
        }
    }

    if ( cpu->apu.ch1.volume_envl_timer == 0 ) {
        if ( cpu->apu.ch1.volume_envl_period == 0 ) {
            cpu->apu.ch1.volume_envl_timer = 8;
        } else {
            cpu->apu.ch1.volume_envl_timer = cpu->apu.ch1.volume_envl_period;
        }
    }
}

void apu_ch1_step_sweep ( _cpu_info *cpu ) {
    if ( cpu->apu.ch1.sweep_timer > 0 ) {
        cpu->apu.ch1.sweep_timer -= 1;
    }

    if ( cpu->apu.ch1.sweep_period > 0 && cpu->apu.ch1.sweep_enable && cpu->apu.ch1.sweep_timer == 0 ) {
        uint16_t freq = apu_ch1_calc_sweep( cpu );
        if ( freq <= 2047 && cpu->apu.ch1.sweep_shift > 0 ) {
            cpu->apu.ch1.frequency_sh = freq;
            cpu->apu.ch1.frequency = freq;

            apu_ch1_calc_sweep( cpu );
        }
    }

    if ( cpu->apu.ch1.sweep_timer == 0 ) {
        if ( cpu->apu.ch1.sweep_period == 0 ) {
            cpu->apu.ch1.sweep_timer = 8;
        } else {
            cpu->apu.ch1.sweep_timer = cpu->apu.ch1.sweep_period;
        }
    }
}

uint16_t apu_ch1_calc_sweep ( _cpu_info *cpu ) {
    uint16_t freq = cpu->apu.ch1.frequency_sh;
    uint16_t r    = cpu->apu.ch1.frequency_sh >> cpu->apu.ch1.sweep_shift;

    if ( cpu->apu.ch1.sweep_direction ) {
        freq -= r;
    } else {
        freq += r;
    }

    if ( freq > 2047 ) {
        cpu->apu.ch1.enable = 0;
        cpu->apu.ch1.sweep_enable = 0;
    }

    if ( cpu->apu.ch1.sweep_direction ) {
        cpu->apu.ch1.sweep_negate_calcd = 1;
    }

    return freq;
}

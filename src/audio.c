#include <stdint.h>

#include "SDL.h"

#include "audio_types.h"
#include "audio.h"
#include "types.h"

void apu_sdl_init( _cpu_info *cpu ) {
    SDL_InitSubSystem(SDL_INIT_AUDIO);

    SDL_memset(&cpu->apu.want, 0, sizeof(cpu->apu.want));
    cpu->apu.want.freq = 48000;          // Sample rate
    cpu->apu.want.format = AUDIO_S8;
    cpu->apu.want.channels = 2;          // Number of sound channels
    cpu->apu.want.samples = 1024;        // Buffer size
    cpu->apu.want.callback = NULL;

    cpu->apu.dev = SDL_OpenAudioDevice(NULL, 0, &cpu->apu.want, &cpu->apu.have, 0);

    if (cpu->apu.dev == 0) {
        SDL_Log("Failed to open audio: %s", SDL_GetError());
    } else {
        if (cpu->apu.have.format != cpu->apu.want.format) {
            SDL_Log("We didn't get U16 audio format.");
        }
        if (cpu->apu.have.freq != cpu->apu.want.freq) {
            SDL_Log("We didn't get the freq.");
        }
        if (cpu->apu.have.samples != cpu->apu.want.samples) {
            SDL_Log("We didn't get samples.");
        }
        SDL_PauseAudioDevice(cpu->apu.dev, 0);
        SDL_PauseAudioDevice(cpu->apu.dev, 1);
        SDL_PauseAudioDevice(cpu->apu.dev, 0);
    }
}

void apu_update ( _cpu_info *cpu ) {
    // Step: Channels
    apu_ch1_step( cpu );
    apu_ch2_step( cpu );
    apu_ch3_step( cpu );
    apu_ch4_step( cpu );

    // Collect sample from channels (if ready)
    if ( cpu->apu.sample_timer > 0 ) {
        cpu->apu.sample_timer -= 1;
    }

    if ( cpu->apu.sample_timer == 0 ) {
        uint16_t sample_l = 0;
        uint16_t sample_r = 0;

        if ( cpu->apu.enable ) {
            uint16_t ch1 = apu_ch1_sample( cpu );
            uint16_t ch2 = apu_ch2_sample( cpu );
            uint16_t ch3 = apu_ch3_sample( cpu );
            uint16_t ch4 = apu_ch4_sample( cpu );

            printf("enabled: ");

            if ( cpu->apu.ch1_left_enable ) {
                sample_l += ch1;
                printf("ch1l ");
            }
            if ( cpu->apu.ch2_left_enable ) {
                sample_l += ch2;
                printf("ch2l ");
            }
            if ( cpu->apu.ch3_left_enable ) {
                sample_l += ch3;
                printf("ch3l ");
            }
            if ( cpu->apu.ch4_left_enable ) {
                sample_l += ch4;
                printf("ch4l ");
            }

            if ( cpu->apu.ch1_right_enable ) {
                sample_r += ch1;
                printf("ch1r ");
            }
            if ( cpu->apu.ch2_right_enable ) {
                sample_r += ch2;
                printf("ch2r ");
            }
            if ( cpu->apu.ch3_right_enable ) {
                sample_r += ch3;
                printf("ch3r ");
            }
            if ( cpu->apu.ch4_right_enable ) {
                sample_r += ch4;
                printf("ch4r ");
            }

            printf(" left: %6d right:%6d ch1: %4d ch2: %4d ch3: %4d ch4: %4d \n", sample_l, sample_r, ch1, ch2, ch3, ch4);
        }

        sample_l *= (uint16_t) cpu->apu.left_volume;
        sample_r *= (uint16_t) cpu->apu.right_volume;

        sample_l *= 12;
        sample_r *= 12;

        cpu->apu.buffer[cpu->apu.buffer_index] = sample_l;
        cpu->apu.buffer[cpu->apu.buffer_index + 1] = sample_r;
        cpu->apu.buffer_index += 2;

        /*printf("%4d %4d %4d\n", cpu->apu.buffer_index, sample_l, sample_r);*/

        if ( cpu->apu.buffer_index >= (BUFFER_SIZE) * 2 ) {
            printf("BOOM! audio write\n");

            if ( SDL_QueueAudio( cpu->apu.dev, cpu->apu.buffer, cpu->apu.buffer_index )) {
                printf("This happened on SDL_QueueAudio: %s", SDL_GetError());
            }
            cpu->apu.buffer_index = 0;
        }

        // Reload sample timer
        cpu->apu.sample_timer = 4194304 / SAMPLE_RATE;
    }
}

uint8_t apu_read_byte ( _cpu_info *cpu, uint16_t addr ) {
    switch ( addr ) {
        // Ch1 sweep [-PPP NSSS] = sweep Period, Negate, Shift
        case 0xff10:
            return ( ( cpu->apu.ch1.sweep_period << 4 ) | ( ((cpu->apu.ch1.sweep_direction ? 1 : 0) << 3) ) | ( cpu->apu.ch1.sweep_shift ) | 0x80 );

        // Ch1 sound length and wave patter duty
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

        case 0xff24:
            return ((cpu->apu.left_vin_enable  << 7) |
                    (cpu->apu.right_vin_enable << 3) |
                    (cpu->apu.left_volume      << 4) |
                    (cpu->apu.right_volume     << 0) );

        case 0xff25:
            return ((cpu->apu.ch4_left_enable  << 7) |
                    (cpu->apu.ch3_left_enable  << 6) |
                    (cpu->apu.ch2_left_enable  << 5) |
                    (cpu->apu.ch1_left_enable  << 4) |
                    (cpu->apu.ch4_right_enable << 3) |
                    (cpu->apu.ch3_right_enable << 2) |
                    (cpu->apu.ch2_right_enable << 1) |
                    (cpu->apu.ch1_right_enable << 0));

        case 0xff26:
            return ((cpu->apu.enable           << 7) |
                    (apu_is_ch1_enabled( cpu ) << 3) |
                    (apu_is_ch2_enabled( cpu ) << 2) |
                    (apu_is_ch3_enabled( cpu ) << 1) |
                    (apu_is_ch4_enabled( cpu ) << 0) | 0x70);

        default:
            break;
    }

    return 0;
}

void apu_write_byte ( _cpu_info *cpu, uint16_t addr, uint8_t data ){
    switch ( addr ) {
        case 0xff10:
            if ( cpu->apu.enable ) {
                cpu->apu.ch1.sweep_period = (data >> 4) & 7;
                cpu->apu.ch1.sweep_shift = data & 7;

                // Clearing the sweep negate mode bit in NR10 after at least one
                // sweep calculation has been made using the negate mode since
                // the last trigger causes the channel to be immediately disabled.
                if (cpu->apu.ch1.sweep_direction && !((data & (1 << 3)) != 0) && cpu->apu.ch1.sweep_negate_calcd) {
                    cpu->apu.ch1.enable = 0;
                    cpu->apu.ch1.sweep_enable = 0;
                }
            }
            break;

        case 0xff11:
            if ( cpu->apu.enable ) {
                cpu->apu.ch1.wave_pattern_duty = (data >> 6) & 0x03;
            }

            cpu->apu.ch1.length = 64 - (data & 63);
            break;

        case 0xff12:
            if ( cpu->apu.enable ) {
                // If the old envelope period was zero and the envelope is
                // still doing automatic updates, volume is incremented by 1,
                // otherwise if the envelope was in subtract mode, volume is
                // incremented by 2.
                if ( cpu->apu.ch1.volume_envl_period == 0 && (cpu->apu.ch1.volume > 0 || cpu->apu.ch1.volume < 0x0f)) {
                    cpu->apu.ch1.volume += 1;
                    if (cpu->apu.ch1.volume_envl_direction) {
                        cpu->apu.ch1.volume += 1;
                    }

                    cpu->apu.ch1.volume &= 0xF;
                }

                cpu->apu.ch1.volume_envl_initial = (data >> 4) & 15;

                // If the mode was changed (add to subtract or subtract to add),
                // volume is set to 16-volume.
                if (cpu->apu.ch1.volume_envl_direction != ((data & (1 << 3)) != 0)) {
                    cpu->apu.ch1.volume = 16 - cpu->apu.ch1.volume;
                    cpu->apu.ch1.volume &= 0xF;
                }

                cpu->apu.ch1.volume_envl_direction = ((data & (1 << 3)) != 0);
                cpu->apu.ch1.volume_envl_period = data & 7;

                // Setting the volume envelope to 0 with a decrease direction will disable
                // the channel
                if (cpu->apu.ch1.volume_envl_initial == 0 && !cpu->apu.ch1.volume_envl_direction) {
                    cpu->apu.ch1.enable = 0;
                }
            }
            break;

        case 0xff13:
            if ( cpu->apu.enable ) {
                cpu->apu.ch1.frequency &= !0xff;
                cpu->apu.ch1.frequency |= data;
            }
            break;

        case 0xff14:
            if ( cpu->apu.enable ) {
                cpu->apu.ch1.frequency &= !0x700;
                cpu->apu.ch1.frequency |= ((data & 7)) << 8;

                uint8_t prev_length_enable = cpu->apu.ch1.length_enable;
                cpu->apu.ch1.length_enable = ((data & (1 << 6)) != 0);

                // Enabling the length counter when the next step of the frame sequencer
                // would not clock the length counter; should clock the length counter
                if (!prev_length_enable && cpu->apu.ch1.length_enable && (cpu->apu.frame_seq_step % 2 == 1) && cpu->apu.ch1.length > 0) {
                    cpu->apu.ch1.length -= 1;
                }

                if ((data & (1 << 7)) != 0) {
                    apu_ch1_trigger( cpu );
                } else if ( cpu->apu.ch1.length == 0 ) {
                    // If the extra length clock brought our length to 0 and we weren't triggered;
                    // disable
                    cpu->apu.ch1.enable = 0;
                }
            }
            break;

        case 0xff24:
            if ( cpu->apu.enable ) {
                cpu->apu.left_vin_enable  = data & (1 << 7);
                cpu->apu.right_vin_enable = data & (1 << 3);
                cpu->apu.left_volume      = (data >> 4) & 0x07;
                cpu->apu.left_volume      = (data >> 0) & 0x07;
            }
            break;

        case 0xff25:
            if ( cpu->apu.enable ) {
                cpu->apu.ch4_left_enable  = data & (1 << 7);
                cpu->apu.ch3_left_enable  = data & (1 << 6);
                cpu->apu.ch2_left_enable  = data & (1 << 5);
                cpu->apu.ch1_left_enable  = data & (1 << 4);

                cpu->apu.ch4_right_enable = data & (1 << 3);
                cpu->apu.ch3_right_enable = data & (1 << 2);
                cpu->apu.ch2_right_enable = data & (1 << 1);
                cpu->apu.ch1_right_enable = data & (1 << 0);
            }
            break;

        case 0xff26:
            cpu->apu.enable = data & (1 << 7);
            if ( !cpu->apu.enable ) {
                apu_clear( cpu );
                apu_ch1_clear( cpu );
                apu_ch2_clear( cpu );
                apu_ch3_clear( cpu );
                apu_ch4_clear( cpu );
            }
            break;

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

void apu_ch4_reset ( _cpu_info *cpu ){
    apu_ch4_clear( cpu );
    cpu->apu.ch4.length = 0;
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
    cpu->apu.ch3.enable                = 0;
    cpu->apu.ch3.dac_enable            = 0;

    cpu->apu.ch3.length_enable         = 0;

    cpu->apu.ch3.volume                = 0;
    cpu->apu.ch3.frequency             = 0;
    cpu->apu.ch3.timer                 = 0;

    cpu->apu.ch3.wave_ram_buffer       = 0;
    cpu->apu.ch3.wave_ram_buffer_position = 0;
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

// Channels Step

void apu_ch1_step( _cpu_info *cpu ) {
    if ( cpu->apu.ch1.timer > 0 ) {
        cpu->apu.ch1.timer--;
    }

    if ( cpu->apu.ch1.timer-- ) {
        cpu->apu.ch1.wave_pattern_index ++;
        if ( cpu->apu.ch1.wave_pattern_index >= 8 ) {
            cpu->apu.ch1.wave_pattern_index = 0;
        }

        cpu->apu.ch1.timer = ( 2048 - cpu->apu.ch1.frequency ) * 4;
    }
}

void apu_ch2_step( _cpu_info *cpu ) {

}

void apu_ch3_step( _cpu_info *cpu ) {

}

void apu_ch4_step( _cpu_info *cpu ) {

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

uint16_t apu_ch2_sample( _cpu_info *cpu ) {
    return 0;
}

uint16_t apu_ch3_sample( _cpu_info *cpu ) {
    return 0;
}

uint16_t apu_ch4_sample( _cpu_info *cpu ) {
    return 0;
}


uint8_t apu_is_ch1_enabled ( _cpu_info *cpu ) {
    return cpu->apu.ch1.enable && (cpu->apu.ch1.volume_envl_initial > 0 || cpu->apu.ch1.volume_envl_direction);
}

uint8_t apu_is_ch2_enabled ( _cpu_info *cpu ) {
    return cpu->apu.ch2.enable;
}

uint8_t apu_is_ch3_enabled ( _cpu_info *cpu ) {
    return cpu->apu.ch3.enable;
}

uint8_t apu_is_ch4_enabled ( _cpu_info *cpu ) {
    return cpu->apu.ch4.enable;
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
        cpu->apu.ch1.volume_envl_period;
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

}

void apu_ch1_step_volume ( _cpu_info *cpu ) {

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
        };
    }
}

uint16_t apu_ch1_calc_sweep ( _cpu_info *cpu ) {
    int16_t freq = cpu->apu.ch1.frequency_sh;
    int16_t r    = cpu->apu.ch1.frequency_sh >> cpu->apu.ch1.sweep_shift;

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


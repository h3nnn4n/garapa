/******************************************************************************
 * Copyright (C) 2016-2018  Renan S. Silva                                    *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgment in the product documentation would be   *
 *    appreciated but is not required.                                        *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 ******************************************************************************/
#include <stdint.h>

#include <SDL2/SDL.h>

#include "audio.h"
#include "audio_types.h"
#include "ch1.h"
#include "ch2.h"
#include "ch3.h"
#include "ch4.h"
#include "types.h"

#if defined(USE_SDL_AUDIO)
void apu_sdl_init(_cpu_info *cpu) {
    SDL_InitSubSystem(SDL_INIT_AUDIO);

    SDL_memset(&cpu->apu.want, 0, sizeof(cpu->apu.want));
    cpu->apu.want.freq     = SAMPLE_RATE; // Sample rate
    cpu->apu.want.format   = AUDIO_U16;
    cpu->apu.want.format   = AUDIO_S16;
    cpu->apu.want.channels = 2;           // Number of sound channels
    cpu->apu.want.samples  = BUFFER_SIZE; // Buffer size
    cpu->apu.want.callback = NULL;

    cpu->apu.dev = SDL_OpenAudioDevice(NULL, 0, &cpu->apu.want, &cpu->apu.have, 0);

    if (cpu->apu.dev == 0) {
        SDL_Log("Failed to open audio: %s", SDL_GetError());
    } else {
        if (cpu->apu.have.format != cpu->apu.want.format) {
            SDL_Log("We didn't get requested audio format.");
        }
        if (cpu->apu.have.freq != cpu->apu.want.freq) {
            SDL_Log("Asked for %d frequency but got %d", cpu->apu.want.freq, cpu->apu.have.freq);
        }
        if (cpu->apu.have.samples != cpu->apu.want.samples) {
            SDL_Log("Asked for %d samples but got %d", cpu->apu.want.samples, cpu->apu.have.samples);
        }
        SDL_PauseAudioDevice(cpu->apu.dev, 0);
        SDL_PauseAudioDevice(cpu->apu.dev, 1);
        SDL_PauseAudioDevice(cpu->apu.dev, 0);
    }
}

#else
#pragma GCC diagnostic ignored "-Wunused-parameter"
void apu_sdl_init(_cpu_info *cpu) { SDL_memset(&cpu->apu.want, 0, sizeof(cpu->apu.want)); }
#endif

void apu_update_on_div_change(_cpu_info *cpu) {
    if (cpu->apu.enable) {
        if ((((cpu->timer._timer_old >> 8) & (1 << 4)) != 0) && !(((cpu->timer._timer >> 8) & (1 << 4)) != 0)) {
            if (cpu->apu.frame_seq_step % 2 == 0) {
                apu_ch1_step_length(cpu);
                apu_ch2_step_length(cpu);
                apu_ch3_step_length(cpu);
                apu_ch4_step_length(cpu);
            }

            if (cpu->apu.frame_seq_step == 7) {
                apu_ch1_step_volume(cpu);
                apu_ch2_step_volume(cpu);
                apu_ch4_step_volume(cpu);
            }

            if (cpu->apu.frame_seq_step == 6 || cpu->apu.frame_seq_step == 2) {
                apu_ch1_step_sweep(cpu);
            }

            cpu->apu.frame_seq_step += 1;
            cpu->apu.frame_seq_step &= 7;
        }
    } else {
        return;
    }
}

void apu_update(_cpu_info *cpu) {
    // Step: Channels
    apu_ch1_step(cpu);
    apu_ch2_step(cpu);
    apu_ch3_step(cpu);
    apu_ch4_step(cpu);

    // Collect sample from channels (if ready)
    if (cpu->apu.sample_timer > 0) {
        cpu->apu.sample_timer -= 1;
    }

    if (cpu->apu.sample_timer == 0) {
        uint16_t sample_l = 0;
        uint16_t sample_r = 0;

        uint16_t ch1 = 0;
        uint16_t ch2 = 0;
        uint16_t ch3 = 0;
        uint16_t ch4 = 0;

        if (cpu->apu.enable) {
            ch1 = apu_ch1_sample(cpu);
            ch2 = apu_ch2_sample(cpu);
            ch3 = apu_ch3_sample(cpu);
            ch4 = apu_ch4_sample(cpu);

            if (cpu->apu.ch1_left_enable) {
                sample_l += ch1;
            }
            if (cpu->apu.ch2_left_enable) {
                sample_l += ch2;
            }
            if (cpu->apu.ch3_left_enable) {
                sample_l += ch3;
            }
            if (cpu->apu.ch4_left_enable) {
                sample_l += ch4;
            }

            if (cpu->apu.ch1_right_enable) {
                sample_r += ch1;
            }
            if (cpu->apu.ch2_right_enable) {
                sample_r += ch2;
            }
            if (cpu->apu.ch3_right_enable) {
                sample_r += ch3;
            }
            if (cpu->apu.ch4_right_enable) {
                sample_r += ch4;
            }
        }

        sample_l *= (uint16_t)cpu->apu.left_volume;
        sample_r *= (uint16_t)cpu->apu.right_volume;

        sample_l *= 12;
        sample_r *= 12;

        sample_l *= 9;
        sample_r *= 9;

        cpu->apu.buffer[cpu->apu.buffer_index]     = sample_l;
        cpu->apu.buffer[cpu->apu.buffer_index + 1] = sample_r;
        cpu->apu.buffer_index += 2;

        if (cpu->apu.buffer_index >= (cpu->apu.have.samples) * 2) {
            cpu->apu.buffer_index = 0;

#ifdef __use_audio
            while (SDL_GetQueuedAudioSize(cpu->apu.dev) > cpu->apu.have.samples * sizeof(uint16_t) * 2) {
                SDL_Delay(1);
            }

            if (SDL_QueueAudio(cpu->apu.dev, cpu->apu.buffer, cpu->apu.have.samples * sizeof(uint16_t) * 2)) {
                SDL_Log("This happened on SDL_QueueAudio: %s", SDL_GetError());
            }
#endif
        }

        // Reload sample timer
        cpu->apu.sample_timer = 4194304 / SAMPLE_RATE;
    }
}

uint8_t _apu_read_byte(_cpu_info *cpu, uint16_t addr) {
    if ((addr >= 0xff30) && (addr <= 0xff3f)) {
        return cpu->apu.ch3.wave_ram[addr - 0xff30];
    }

    switch (addr) {
        /////////////////////////////////////////////
        //  CH1 session
        /////////////////////////////////////////////
        // Ch1 sweep [-PPP NSSS] = sweep Period, Negate, Shift
        case 0xff10:
            return ((cpu->apu.ch1.sweep_period << 4) | (((cpu->apu.ch1.sweep_direction ? 1 : 0) << 3)) |
                    (cpu->apu.ch1.sweep_shift) | (0x80));

        // Ch1 sound length and wave patter duty
        // [DDLL LLLL] Duty, Lenght
        case 0xff11: return (cpu->apu.ch1.wave_pattern_duty << 6) | 0x3f;

        // Ch1 Volume envelope
        // [VVVV APPP] starting Volume, Add mode, Period
        case 0xff12:
            return ((cpu->apu.ch1.volume_envl_initial << 4) | (((cpu->apu.ch1.volume_envl_direction ? 1 : 0) << 3)) |
                    (cpu->apu.ch1.volume_envl_period));

        // Ch1 Frequency LSB, W only
        case 0xff13: return 0xff;

        // Ch1 misc
        // [TL-- -FFF] Trigger, Length enable, Frequensy MSB
        case 0xff14: return (((cpu->apu.ch1.length_enable ? 1 : 0) << 6)) | 0xbf;

        /////////////////////////////////////////////

        /////////////////////////////////////////////
        //  CH2 session
        /////////////////////////////////////////////
        case 0xff16: return (cpu->apu.ch2.wave_pattern_duty << 6) | 0x3f;

        case 0xff17:
            return ((cpu->apu.ch2.volume_envl_initial << 4) | (((cpu->apu.ch2.volume_envl_direction ? 1 : 0) << 3)) |
                    (cpu->apu.ch2.volume_envl_period));

        case 0xff18: return 0xff;

        case 0xff19: return (((cpu->apu.ch2.length_enable ? 1 : 0) << 6)) | 0xbf;

        /////////////////////////////////////////////

        /////////////////////////////////////////////
        //  CH3 session
        /////////////////////////////////////////////
        // Channel 3 Sound On/Off
        // [E--- ----] DAC Power
        case 0xff1a: return (cpu->apu.ch3.dac_enable << 7) | 0x7f;

        case 0xff1b: return 0xff;

        // Channel 3 Volume
        // [-VV- ----] Volume
        case 0xff1c: return (cpu->apu.ch3.volume << 5) | 0x9f;

        case 0xff1d: return 0xff;

        // Channel 3 Misc.
        // [TL-- -FFF] Trigger, Length enable, Frequency MSB
        case 0xff1e: return (cpu->apu.ch3.length_enable << 6) | 0xbf;

        // Wave RAM
        // Treated elsewhere

        /////////////////////////////////////////////

        /////////////////////////////////////////////
        //  CH4 session
        /////////////////////////////////////////////
        case 0xff20: return 0xff;

        // Channel 4 Volume Envelope
        // [VVVV APPP] Starting volume, Envelope add mode, period
        case 0xff21:
            return ((cpu->apu.ch4.volume_envl_initial << 4) | (((cpu->apu.ch4.volume_envl_direction ? 1 : 0) << 3)) |
                    (cpu->apu.ch4.volume_envl_period));

        // Channel 4 Polynomial Counter
        // [SSSS WDDD] Clock shift, Width mode of LFSR, Divisor code
        /*0xFF22 => (self.shift << 4) | bits::bit(self.width, 3) | self.divisor,*/
        case 0xff22: return ((cpu->apu.ch4.shift << 4) | (cpu->apu.ch4.width << 3) | (cpu->apu.ch4.divisor));

        // Channel 2 Misc.
        // [TL-- ----] Trigger, Length enable
        /*0xFF23 => bits::bit(self.length_enable, 6) | 0xBF,*/
        case 0xff23: return (cpu->apu.ch4.length_enable << 6) | 0xbf;
        /////////////////////////////////////////////

        /////////////////////////////////////////////
        //  APU
        /////////////////////////////////////////////
        case 0xff24:
            return ((cpu->apu.left_vin_enable << 7) | (cpu->apu.right_vin_enable << 3) | (cpu->apu.left_volume << 4) |
                    (cpu->apu.right_volume << 0));

        case 0xff25:
            return ((cpu->apu.ch4_left_enable << 7) | (cpu->apu.ch3_left_enable << 6) |
                    (cpu->apu.ch2_left_enable << 5) | (cpu->apu.ch1_left_enable << 4) |
                    (cpu->apu.ch4_right_enable << 3) | (cpu->apu.ch3_right_enable << 2) |
                    (cpu->apu.ch2_right_enable << 1) | (cpu->apu.ch1_right_enable << 0));

        case 0xff26:
            return (((!!cpu->apu.enable) << 7) | (apu_is_ch4_enabled(cpu) << 3) | (apu_is_ch3_enabled(cpu) << 2) |
                    (apu_is_ch2_enabled(cpu) << 1) | (apu_is_ch1_enabled(cpu) << 0) | 0x70);
            /////////////////////////////////////////////
    }

    return 0xff;
}

uint8_t apu_read_byte(_cpu_info *cpu, uint16_t addr) {
    uint8_t ret = _apu_read_byte(cpu, addr);

    /*printf("apu_read %04x: %02x\n", addr, ret);*/

    return ret;
}

void apu_write_byte(_cpu_info *cpu, uint16_t addr, uint8_t data) {
    /*printf("apu_write %04x: %02x\n", addr, data);*/
    /////////////////////////////////////////////
    //  CH3 - Wave ram
    /////////////////////////////////////////////
    if (addr >= 0xff30 && addr <= 0xffe3) {
        cpu->apu.ch3.wave_ram[(addr - 0xff30)] = data;
        return;
    }
    /////////////////////////////////////////////

    switch (addr) {
        /////////////////////////////////////////////
        //  CH1
        /////////////////////////////////////////////
        case 0xff10:
            if (cpu->apu.enable) {
                cpu->apu.ch1.sweep_period = (data >> 4) & 7;
                cpu->apu.ch1.sweep_shift  = data & 7;

                // Clearing the sweep negate mode bit in NR10 after at least one
                // sweep calculation has been made using the negate mode since
                // the last trigger causes the channel to be immediately disabled.
                if (cpu->apu.ch1.sweep_direction && !((data & (1 << 3)) != 0) && cpu->apu.ch1.sweep_negate_calcd) {
                    cpu->apu.ch1.enable       = 0;
                    cpu->apu.ch1.sweep_enable = 0;
                }

                cpu->apu.ch1.sweep_direction = (data & (1 << 3)) != 0;
            }
            break;

        case 0xff11:
            if (cpu->apu.enable) {
                cpu->apu.ch1.wave_pattern_duty = (data >> 6) & 0x03;
            }

            cpu->apu.ch1.length = 64 - (data & 63);
            break;

        case 0xff12:
            if (cpu->apu.enable) {
                // if the old envelope period was zero and the envelope is
                // still doing automatic updates, volume is incremented by 1,
                // otherwise if the envelope was in subtract mode, volume is
                // incremented by 2.
                if (cpu->apu.ch1.volume_envl_period == 0 && (cpu->apu.ch1.volume > 0 && cpu->apu.ch1.volume < 0x0f)) {
                    cpu->apu.ch1.volume += 1;
                    if (cpu->apu.ch1.volume_envl_direction) {
                        cpu->apu.ch1.volume += 1;
                    }

                    cpu->apu.ch1.volume &= 0xf;
                }

                cpu->apu.ch1.volume_envl_initial = (data >> 4) & 15;

                // if the mode was changed (add to subtract or subtract to add),
                // volume is set to 16-volume.
                if (cpu->apu.ch1.volume_envl_direction != ((data & (1 << 3)) != 0)) {
                    cpu->apu.ch1.volume = 16 - cpu->apu.ch1.volume;
                    cpu->apu.ch1.volume &= 0xf;
                }

                cpu->apu.ch1.volume_envl_direction = ((data & (1 << 3)) != 0);
                cpu->apu.ch1.volume_envl_period    = data & 7;

                // setting the volume envelope to 0 with a decrease direction will disable
                // the channel
                if (cpu->apu.ch1.volume_envl_initial == 0 && !cpu->apu.ch1.volume_envl_direction) {
                    cpu->apu.ch1.enable = 0;
                }
            }
            break;

        case 0xff13:
            if (cpu->apu.enable) {
                cpu->apu.ch1.frequency &= ~0xff;
                cpu->apu.ch1.frequency |= data;
            }
            break;

        case 0xff14:
            if (cpu->apu.enable) {
                cpu->apu.ch1.frequency &= ~0x700;
                cpu->apu.ch1.frequency |= ((data & 7)) << 8;

                uint8_t prev_length_enable = cpu->apu.ch1.length_enable;
                cpu->apu.ch1.length_enable = ((data & (1 << 6)) != 0);

                // Enabling the length counter when the next step of the frame sequencer
                // would not clock the length counter; should clock the length counter
                if (!prev_length_enable && cpu->apu.ch1.length_enable && (cpu->apu.frame_seq_step % 2 == 1) &&
                    cpu->apu.ch1.length > 0) {
                    cpu->apu.ch1.length -= 1;
                }

                if ((data & (1 << 7)) != 0) {
                    apu_ch1_trigger(cpu);
                } else if (cpu->apu.ch1.length == 0) {
                    // If the extra length clock brought our length to 0 and we weren't triggered;
                    // disable
                    cpu->apu.ch1.enable = 0;
                }
            }
            break;
            /////////////////////////////////////////////

        case 0xff15:
            break;

            /////////////////////////////////////////////
            //  CH2
            /////////////////////////////////////////////

        case 0xff16:
            if (cpu->apu.enable) {
                cpu->apu.ch2.wave_pattern_duty = (data >> 6) & 0x03;
            }

            cpu->apu.ch2.length = 64 - (data & 63);
            break;

        case 0xff17:
            if (cpu->apu.enable) {
                // If the old envelope period was zero and the envelope is
                // still doing automatic updates, volume is incremented by 1,
                // otherwise if the envelope was in subtract mode, volume is
                // incremented by 2.
                if (cpu->apu.ch2.volume_envl_period == 0 && (cpu->apu.ch2.volume > 0 && cpu->apu.ch2.volume < 0x0f)) {
                    cpu->apu.ch2.volume += 1;
                    if (cpu->apu.ch2.volume_envl_direction) {
                        cpu->apu.ch2.volume += 1;
                    }

                    cpu->apu.ch2.volume &= 0xF;
                }

                cpu->apu.ch2.volume_envl_initial = (data >> 4) & 15;

                // If the mode was changed (add to subtract or subtract to add),
                // volume is set to 16-volume.
                if (cpu->apu.ch2.volume_envl_direction != ((data & (1 << 3)) != 0)) {
                    cpu->apu.ch2.volume = 16 - cpu->apu.ch2.volume;
                    cpu->apu.ch2.volume &= 0xF;
                }

                cpu->apu.ch2.volume_envl_direction = ((data & (1 << 3)) != 0);
                cpu->apu.ch2.volume_envl_period    = data & 7;

                // Setting the volume envelope to 0 with a decrease direction will disable
                // the channel
                if (cpu->apu.ch2.volume_envl_initial == 0 && !cpu->apu.ch2.volume_envl_direction) {
                    cpu->apu.ch2.enable = 0;
                }
            }
            break;

        case 0xff18:
            if (cpu->apu.enable) {
                cpu->apu.ch2.frequency &= ~0xff;
                cpu->apu.ch2.frequency |= data;
            }
            break;

        case 0xff19:
            if (cpu->apu.enable) {
                cpu->apu.ch2.frequency &= ~0x700;
                cpu->apu.ch2.frequency |= ((data & 7)) << 8;

                uint8_t prev_length_enable = cpu->apu.ch2.length_enable;
                cpu->apu.ch2.length_enable = ((data & (1 << 6)) != 0);

                // Enabling the length counter when the next step of the frame sequencer
                // would not clock the length counter; should clock the length counter
                if (!prev_length_enable && cpu->apu.ch2.length_enable && (cpu->apu.frame_seq_step % 2 == 1) &&
                    cpu->apu.ch2.length > 0) {
                    cpu->apu.ch2.length -= 1;
                }

                if ((data & (1 << 7)) != 0) {
                    apu_ch2_trigger(cpu);
                } else if (cpu->apu.ch2.length == 0) {
                    // If the extra length clock brought our length to 0 and we weren't triggered;
                    // disable
                    cpu->apu.ch2.enable = 0;
                }
            }
            break;

        /////////////////////////////////////////////

        /////////////////////////////////////////////
        //  CH4
        /////////////////////////////////////////////

        // Channel 4 Sound Length
        // [--LL LLLL] Length load (64-L)
        /*0xFF20 => { self.length = 64 - (value & 0b11_1111); }*/
        case 0xff20: cpu->apu.ch4.length = 64 - (data & 0x3f); break;

        // Channel 4 Volume Envelope
        // [VVVV APPP] Starting volume, Envelope add mode, period
        case 0xff21:
            if (cpu->apu.enable) {
                // if the old envelope period was zero and the envelope is
                // still doing automatic updates, volume is incremented by 1,
                // otherwise if the envelope was in subtract mode, volume is
                // incremented by 2.
                if (cpu->apu.ch4.volume_envl_period == 0 && (cpu->apu.ch4.volume > 0 && cpu->apu.ch4.volume < 0x0f)) {
                    cpu->apu.ch4.volume += 1;
                    if (cpu->apu.ch4.volume_envl_direction) {
                        cpu->apu.ch4.volume += 1;
                    }

                    cpu->apu.ch4.volume &= 0xf;
                }

                cpu->apu.ch4.volume_envl_initial = (data >> 4) & 15;

                // if the mode was ch4nged (add to subtract or subtract to add),
                // volume is set to 16-volume.
                if (cpu->apu.ch4.volume_envl_direction != ((data & (1 << 3)) != 0)) {
                    cpu->apu.ch4.volume = 16 - cpu->apu.ch4.volume;
                    cpu->apu.ch4.volume &= 0xf;
                }

                cpu->apu.ch4.volume_envl_direction = ((data & (1 << 3)) != 0);
                cpu->apu.ch4.volume_envl_period    = data & 7;

                // setting the volume envelope to 0 with a decrease direction will disable
                // the ch4nnel
                if (cpu->apu.ch4.volume_envl_initial == 0 && !cpu->apu.ch4.volume_envl_direction) {
                    cpu->apu.ch4.enable = 0;
                }
            }
            break;

        // Channel 4 Polynomial Counter
        // [SSSS WDDD] Clock shift, Width mode of LFSR, Divisor code
        case 0xff22:
            if (cpu->apu.enable) {
                cpu->apu.ch4.shift   = (data >> 4) & 15;
                cpu->apu.ch4.width   = (data & (1 << 3)) != 0;
                cpu->apu.ch4.divisor = data & 7;
            }
            break;

        // Channel 4 Misc.
        // [TL-- ----] Trigger, Length enable
        case 0xff23:
            if (cpu->apu.enable) {
                uint8_t prev_length_enable = cpu->apu.ch4.length_enable;
                cpu->apu.ch4.length_enable = ((data & (1 << 6)) != 0);

                // Enabling the length counter when the next step of the frame sequencer
                // would not clock the length counter; should clock the length counter
                if (!prev_length_enable && cpu->apu.ch4.length_enable && (cpu->apu.frame_seq_step % 2 == 1) &&
                    cpu->apu.ch4.length > 0) {
                    cpu->apu.ch4.length -= 1;
                }

                if ((data & (1 << 7)) != 0) {
                    apu_ch4_trigger(cpu);
                } else if (cpu->apu.ch4.length == 0) {
                    // If the extra length clock brought our length to 0 and we weren't triggered;
                    // disable
                    cpu->apu.ch4.enable = 0;
                }
            }
            break;
        /////////////////////////////////////////////

        /////////////////////////////////////////////
        //  APU
        /////////////////////////////////////////////
        case 0xff24:
            if (cpu->apu.enable) {
                cpu->apu.left_vin_enable  = (data & (1 << 7)) != 0;
                cpu->apu.right_vin_enable = (data & (1 << 3)) != 0;
                cpu->apu.left_volume      = (data >> 4) & 0x07;
                cpu->apu.right_volume     = (data >> 0) & 0x07;
            }
            break;

        case 0xff25:
            if (cpu->apu.enable) {
                cpu->apu.ch4_left_enable = (data & (1 << 7)) != 0;
                cpu->apu.ch3_left_enable = (data & (1 << 6)) != 0;
                cpu->apu.ch2_left_enable = (data & (1 << 5)) != 0;
                cpu->apu.ch1_left_enable = (data & (1 << 4)) != 0;

                cpu->apu.ch4_right_enable = (data & (1 << 3)) != 0;
                cpu->apu.ch3_right_enable = (data & (1 << 2)) != 0;
                cpu->apu.ch2_right_enable = (data & (1 << 1)) != 0;
                cpu->apu.ch1_right_enable = (data & (1 << 0)) != 0;
            }
            break;

        case 0xff26:
            cpu->apu.enable = (data & (1 << 7)) != 0;
            if (!cpu->apu.enable) {
                apu_clear(cpu);
                apu_ch1_clear(cpu);
                apu_ch2_clear(cpu);
                apu_ch3_clear(cpu);
                apu_ch4_clear(cpu);
            }
            break;
        /////////////////////////////////////////////

        /////////////////////////////////////////////
        //  CH3
        /////////////////////////////////////////////
        // Ch3 on-off
        // [E--- ----]
        case 0xff1a:
            if (cpu->apu.enable) {
                cpu->apu.ch3.dac_enable = (data & (1 << 7)) != 0;

                if (!cpu->apu.ch3.dac_enable) {
                    cpu->apu.ch3.enable = 0;
                }
            }
            break;

        // ch3 sound len
        // [LLLL LLLL] Length load (256-L)
        case 0xff1b: cpu->apu.ch3.length = ((uint16_t)256) - data; break;

        // ch3 sound volume
        // [-VV- ----] Volume
        case 0xff1c:
            if (cpu->apu.enable) {
                cpu->apu.ch3.volume = (data >> 5) & 3;
            }
            break;

        // Channel 3 Frequency (lo)
        // [FFFF FFFF] Frequency LSB
        case 0xff1d:
            if (cpu->apu.enable) {
                cpu->apu.ch3.frequency &= ~0x700;
                cpu->apu.ch3.frequency |= ((data & 7) << 8);
            }
            break;

        // Channel 3 Misc.
        // [TL-- -FFF] Trigger, Length enable, Frequency MSB
        case 0xff1e:
            if (cpu->apu.enable) {
                cpu->apu.ch3.frequency &= ~0x700;
                cpu->apu.ch3.frequency |= ((data & 7) << 8);

                uint8_t prev_length_enable = cpu->apu.ch3.length_enable;
                cpu->apu.ch3.length_enable = ((data & (1 << 6)) != 0);

                if (!prev_length_enable && cpu->apu.ch3.length_enable && (cpu->apu.frame_seq_step % 2 == 1) &&
                    cpu->apu.ch3.length > 0) {
                    cpu->apu.ch3.length -= 1;
                }

                if ((data & (1 << 7)) != 0) {
                    apu_ch3_trigger(cpu);
                } else if (cpu->apu.ch3.length == 0) {
                    cpu->apu.ch3.enable = 0;
                }
            }
            break;
            /////////////////////////////////////////////

        default: break;
    }
}

void apu_reset(_cpu_info *cpu) {
    cpu->apu.buffer_index = 0;
    cpu->apu.sample_timer = 0;

    apu_ch1_reset(cpu);
    apu_ch2_reset(cpu);
    apu_ch3_reset(cpu);
    apu_ch4_reset(cpu);

    apu_clear(cpu);

    cpu->apu.enable = 1;

    cpu->apu.left_volume  = 0x07;
    cpu->apu.right_volume = 0x07;

    cpu->apu.ch1_left_enable = 1;
    cpu->apu.ch2_left_enable = 1;
    cpu->apu.ch3_left_enable = 1;
    cpu->apu.ch4_left_enable = 1;

    cpu->apu.ch1_right_enable = 1;
    cpu->apu.ch2_right_enable = 1;
    cpu->apu.ch3_right_enable = 0;
    cpu->apu.ch4_right_enable = 0;
}

void apu_clear(_cpu_info *cpu) {
    cpu->apu.enable         = 0;
    cpu->apu.frame_seq_step = 0;

    cpu->apu.left_volume  = 0x00;
    cpu->apu.right_volume = 0x00;

    cpu->apu.right_vin_enable = 0;
    cpu->apu.left_vin_enable  = 0;

    cpu->apu.ch1_left_enable = 0;
    cpu->apu.ch2_left_enable = 0;
    cpu->apu.ch3_left_enable = 0;
    cpu->apu.ch4_left_enable = 0;

    cpu->apu.ch1_right_enable = 0;
    cpu->apu.ch2_right_enable = 0;
    cpu->apu.ch3_right_enable = 0;
    cpu->apu.ch4_right_enable = 0;
}

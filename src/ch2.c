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

#include "ch2.h"
#include "types.h"

void apu_ch2_reset(_cpu_info *cpu) {
    apu_ch2_clear(cpu);
    cpu->apu.ch2.length = 0;
    cpu->apu.ch2.timer  = 0x00;
}

void apu_ch2_clear(_cpu_info *cpu) {
    cpu->apu.ch2.enable = 0;

    cpu->apu.ch2.wave_pattern_duty  = 0;
    cpu->apu.ch2.wave_pattern_index = 0;

    cpu->apu.ch2.length_enable = 0;

    cpu->apu.ch2.volume                = 0;
    cpu->apu.ch2.volume_envl_initial   = 0;
    cpu->apu.ch2.volume_envl_direction = 0;
    cpu->apu.ch2.volume_envl_period    = 0;
    cpu->apu.ch2.volume_envl_timer     = 0;

    cpu->apu.ch2.timer &= 0x03; // when reset, the lower 2 bits are not changed
}

void apu_ch2_step(_cpu_info *cpu) {
    if (cpu->apu.ch2.timer > 0) {
        cpu->apu.ch2.timer--;
    }

    if (cpu->apu.ch2.timer == 0) {
        cpu->apu.ch2.wave_pattern_index++;
        if (cpu->apu.ch2.wave_pattern_index >= 8) {
            cpu->apu.ch2.wave_pattern_index = 0;
        }

        cpu->apu.ch2.timer = (2048 - cpu->apu.ch2.frequency) * 4;
    }
}

uint16_t apu_ch2_sample(_cpu_info *cpu) {
    if (cpu->apu.ch2.enable) {

        uint8_t pat = 0;

        switch (cpu->apu.ch2.wave_pattern_duty) {
            case 0x0:
                pat = 0x01; // 0b00000001
                break;
            case 0x1:
                pat = 0x81; // 0b10000001
                break;
            case 0x2:
                pat = 0x87; // 0b10000111
                break;
            case 0x3:
                pat = 0x7e; // 0b01111110
                break;
        }

        uint8_t bit = (pat & (1 << (7 - cpu->apu.ch2.wave_pattern_index)));

        if (bit) {
            return cpu->apu.ch2.volume;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

uint8_t apu_is_ch2_enabled(_cpu_info *cpu) {
    return cpu->apu.ch2.enable && (cpu->apu.ch2.volume_envl_initial > 0 || cpu->apu.ch2.volume_envl_direction);
}

void apu_ch2_trigger(_cpu_info *cpu) {
    cpu->apu.ch2.enable = 1;

    if (cpu->apu.ch2.length == 0) {
        if (cpu->apu.ch2.length_enable && (cpu->apu.frame_seq_step % 2 == 1)) {
            cpu->apu.ch2.length = 63;
        } else {
            cpu->apu.ch2.length = 64;
        }
    }

    cpu->apu.ch2.timer = (2048 - cpu->apu.ch2.frequency) * 4;

    cpu->apu.ch2.volume = cpu->apu.ch2.volume_envl_initial;
    if (cpu->apu.ch2.volume_envl_period == 0) {
        cpu->apu.ch2.volume_envl_timer = 8;
    } else {
        cpu->apu.ch2.volume_envl_timer = cpu->apu.ch2.volume_envl_period;
    }

    if (cpu->apu.frame_seq_step == 7) {
        cpu->apu.ch2.volume_envl_timer++;
    }
}

void apu_ch2_step_volume(_cpu_info *cpu) {
    if (cpu->apu.ch2.volume_envl_timer > 0) {
        cpu->apu.ch2.volume_envl_timer -= 1;
    }

    if (cpu->apu.ch2.volume_envl_period > 0 && cpu->apu.ch2.volume_envl_timer == 0) {
        if (cpu->apu.ch2.volume_envl_direction) {
            if (cpu->apu.ch2.volume < 0xF) {
                cpu->apu.ch2.volume += 1;
            }
        } else if (cpu->apu.ch2.volume > 0) {
            cpu->apu.ch2.volume -= 1;
        }
    }

    if (cpu->apu.ch2.volume_envl_timer == 0) {
        if (cpu->apu.ch2.volume_envl_period == 0) {
            cpu->apu.ch2.volume_envl_timer = 8;
        } else {
            cpu->apu.ch2.volume_envl_timer = cpu->apu.ch2.volume_envl_period;
        }
    }
}

void apu_ch2_step_length(_cpu_info *cpu) {
    if (cpu->apu.ch2.length_enable && cpu->apu.ch2.length > 0) {
        cpu->apu.ch2.length -= 1;
        if (cpu->apu.ch2.length == 0) {
            cpu->apu.ch2.enable = 0;
        }
    }
}

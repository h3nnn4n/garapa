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

#include "ch3.h"
#include "types.h"

void apu_ch3_reset(_cpu_info *cpu) {
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
    apu_ch3_clear(cpu);
}

void apu_ch3_clear(_cpu_info *cpu) {
    cpu->apu.ch3.enable     = 0;
    cpu->apu.ch3.dac_enable = 0;

    cpu->apu.ch3.length_enable = 0;

    cpu->apu.ch3.volume    = 0;
    cpu->apu.ch3.frequency = 0;
    cpu->apu.ch3.timer     = 0;

    cpu->apu.ch3.wave_ram_buffer          = 0;
    cpu->apu.ch3.wave_ram_buffer_position = 0;
}

void apu_ch3_trigger(_cpu_info *cpu) {
    cpu->apu.ch3.enable = 1;

    if (cpu->apu.ch3.length == 0) {
        if (cpu->apu.ch3.length_enable && (cpu->apu.frame_seq_step % 2 == 1)) {
            cpu->apu.ch3.length = 255;
        } else {
            cpu->apu.ch3.length = 256;
        }
    }

    cpu->apu.ch3.timer = (2048 - cpu->apu.ch3.frequency) * 4;

    cpu->apu.ch3.wave_ram_buffer_position = 0;
}

void apu_ch3_step(_cpu_info *cpu) {
    if (cpu->apu.ch3.timer > 0) {
        cpu->apu.ch3.timer--;
    }

    if (cpu->apu.ch3.timer == 0) {
        cpu->apu.ch3.wave_ram_buffer_position++;
        if (cpu->apu.ch3.wave_ram_buffer_position >= 32) {
            cpu->apu.ch3.wave_ram_buffer_position = 0;
        }

        cpu->apu.ch3.wave_ram_buffer = cpu->apu.ch3.wave_ram[(int)(cpu->apu.ch3.wave_ram_buffer_position / 2)];
        cpu->apu.ch3.wave_ram_buffer >>= (1 - (cpu->apu.ch3.wave_ram_buffer_position % 2)) * 4;
        cpu->apu.ch3.wave_ram_buffer &= 0x0F;

        cpu->apu.ch3.timer = (2048 - cpu->apu.ch3.frequency) * 4;
    }
}

uint16_t apu_ch3_sample(_cpu_info *cpu) {
    if (cpu->apu.ch3.enable) {
        return (uint16_t)(cpu->apu.ch3.wave_ram_buffer >>
                          ((cpu->apu.ch3.volume > 0) ? (cpu->apu.ch3.volume - 1) : (4)));
    } else {
        return 0;
    }
}

uint8_t apu_is_ch3_enabled(_cpu_info *cpu) { return cpu->apu.ch3.enable && cpu->apu.ch3.dac_enable; }

void apu_ch3_step_length(_cpu_info *cpu) {
    if (cpu->apu.ch3.length_enable && cpu->apu.ch3.length > 0) {
        cpu->apu.ch3.length -= 1;
        if (cpu->apu.ch3.length == 0) {
            cpu->apu.ch3.enable = 0;
        }
    }
}

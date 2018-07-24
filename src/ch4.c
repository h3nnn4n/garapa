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
    if ( cpu->apu.ch4.timer > 0 ) {
        cpu->apu.ch4.timer--;
    }

    if ( cpu->apu.ch4.timer == 0 ) {
        // Using a noise channel clock shift of 14 or 15 results in the
        // LFSR receiving no clocks.
        if (cpu->apu.ch4.shift < 14) {
            // When clocked by the frequency timer, the low two bits (0 and 1)
            // are XORed
            uint16_t b = (cpu->apu.ch4.lfsr & 0x1) ^ ((cpu->apu.ch4.lfsr & 0x2) >> 1);

            // All bits are shifted right by one
            cpu->apu.ch4.lfsr >>= 1;

            // And the result of the XOR is put into the now-empty high bit
            if (b != 0) {
                cpu->apu.ch4.lfsr |= 0x4000;
            }

            // If width mode is 1 (NR43), the XOR result is ALSO put into
            // bit 6 AFTER the shift, resulting in a 7-bit LFSR.
            if (cpu->apu.ch4.width) {
                cpu->apu.ch4.lfsr &= !0x40;
                if (b != 0) {
                    cpu->apu.ch4.lfsr |= 0x40;
                }
            }
        }

        // Reload timer
        cpu->apu.ch4.timer = get_divisor(cpu->apu.ch4.divisor) << cpu->apu.ch4.shift;
    }
}

uint16_t apu_ch4_sample( _cpu_info *cpu ) {
    if ( cpu->apu.ch4.enable ) {
        if (( cpu->apu.ch4.lfsr & 0x1 ) == 0) {
            return cpu->apu.ch4.volume;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

uint8_t apu_is_ch4_enabled ( _cpu_info *cpu ) {
    return cpu->apu.ch4.enable && (cpu->apu.ch4.volume_envl_initial > 0 || cpu->apu.ch4.volume_envl_direction);
}

void apu_ch4_step_length( _cpu_info *cpu ) {
    if ( cpu->apu.ch4.length_enable && cpu->apu.ch4.length > 0 ) {
        cpu->apu.ch4.length -= 1;
        if ( cpu->apu.ch4.length == 0 ) {
            cpu->apu.ch4.enable = 0;
        }
    }
}

void apu_ch4_step_volume( _cpu_info *cpu ) {
    if ( cpu->apu.ch4.volume_envl_timer > 0 ) {
        cpu->apu.ch4.volume_envl_timer -= 1;
    }

    if ( cpu->apu.ch4.volume_envl_period > 0 && cpu->apu.ch4.volume_envl_timer == 0 ) {
        if ( cpu->apu.ch4.volume_envl_direction ) {
            if ( cpu->apu.ch4.volume < 0xF ) {
                cpu->apu.ch4.volume += 1;
            }
        } else if ( cpu->apu.ch4.volume > 0 ) {
            cpu->apu.ch4.volume -= 1;
        }
    }

    if ( cpu->apu.ch4.volume_envl_timer == 0 ) {
        if ( cpu->apu.ch4.volume_envl_period == 0 ) {
            cpu->apu.ch4.volume_envl_timer = 8;
        } else {
            cpu->apu.ch4.volume_envl_timer = cpu->apu.ch4.volume_envl_period;
        }
    }
}

uint16_t get_divisor(uint8_t index) {
    if (index == 1) {
        return 16;
    } else if ( index == 2 ) {
        return 32;
    } else if ( index == 3 ) {
        return 48;
    } else if ( index == 4 ) {
        return 64;
    } else if ( index == 5 ) {
        return 80;
    } else if ( index == 6 ) {
        return 96;
    } else if ( index == 7 ) {
        return 112;
    } else {
        return 8;
    }
}

void apu_ch4_trigger ( _cpu_info *cpu ) {
    cpu->apu.ch4.enable = 1;

    if ( cpu->apu.ch4.length == 0 ) {
        if ( cpu->apu.ch4.length_enable && ( cpu->apu.frame_seq_step % 2 == 1 ) ) {
            cpu->apu.ch4.length = 63;
        } else {
            cpu->apu.ch4.length = 64;
        }
    }

    cpu->apu.ch4.timer = get_divisor(cpu->apu.ch4.divisor) << cpu->apu.ch4.shift;

    cpu->apu.ch4.volume = cpu->apu.ch4.volume_envl_initial;
    if ( cpu->apu.ch4.volume_envl_period == 0 ) {
        cpu->apu.ch4.volume_envl_timer = 8;
    } else {
        cpu->apu.ch4.volume_envl_timer = cpu->apu.ch4.volume_envl_period;
    }

    if ( cpu->apu.frame_seq_step == 7 ) {
        cpu->apu.ch4.volume_envl_timer++;
    }

    cpu->apu.ch4.lfsr = 0x7FFF;
}

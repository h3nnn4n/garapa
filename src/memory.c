/******************************************************************************
 * Copyright (C) 2016  Renan S. Silva                                         *
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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "memory.h"
#include "audio.h"
#include "interrupts.h"
#include "time_keeper.h"
#include "utils.h"
#include "types.h"
#include "graphics.h"
#include "display.h"
#include "cartridge.h"

void dma_step ( _cpu_info *cpu ) {
    if ( cpu->dma.oam_dma_timer > 0 ) {
        uint16_t src = cpu->dma.oam_dma_source + cpu->dma.oam_dma_index;
        uint8_t data = read_byte ( cpu, src );
        cpu->mem_controller.memory [ 0xfe00 + cpu->dma.oam_dma_index ] = data;

        /*printf("DMA: %3d %3d %4x %2x\n",*/
                /*cpu->dma.oam_dma_index,*/
                /*cpu->dma.oam_dma_timer,*/
                /*0xfe00 + cpu->dma.oam_dma_index,*/
                /*data);*/

        cpu->dma.oam_dma_index ++;
        cpu->dma.oam_dma_timer --;
    }

    if ( cpu->dma.oam_dma_delay_timer > 0 ) {
        cpu->dma.oam_dma_delay_timer --;
        if ( cpu->dma.oam_dma_delay_timer == 0 ) {
            cpu->dma.oam_dma_timer = 160;
            cpu->dma.oam_dma_index = 0;
            cpu->dma.oam_dma_source = cpu->dma.oam_dma_next_source;
        }
    }
}

uint8_t read_byte ( _cpu_info *cpu, uint16_t addr ) {
    if ( ( addr >= 0xFE00 ) && ( addr <= 0xFE9F ) ) {
        if ( ( cpu->dma.oam_dma_timer == 0 ) && ( ( cpu->lcd.power == 0 ) || ( cpu->lcd.mode  <  2 ))) {
            return cpu->mem_controller.memory [ addr ];
        } else {
            return 0xff;
        }
    }

    if ( ( addr >= 0x8000 ) && ( addr <= 0x9fff ) ) {
        if ( cpu->lcd.power == 0 || cpu->lcd.mode < 3 ) {
            return cpu->mem_controller.memory [ addr ];
        } else {
            return 0xff;
        }
    }

    /*if ( (addr >= 0xff10 && addr <= 0xff26) ||*/
         /*(addr >= 0xff30 && addr <= 0xff3f) ) {*/
    if ( (addr >= 0xff10 && addr <= 0xff19) ||
         (addr >= 0xff24 && addr <= 0xff26) ) {
        return apu_read_byte ( cpu, addr );
    }

    return _read_byte ( cpu, addr );
}

uint8_t _read_byte ( _cpu_info *cpu, uint16_t addr ) {
    if ( addr < 0x8000 ) {
        return cartridge_read ( cpu, addr );
    }

    switch ( addr ) {
        case 0xff00:
            {
                /*printf (" %d %d %d %d - %d %d %d %d\n",*/
                          /*cpu->joystick.button_start  ,*/
                          /*cpu->joystick.button_select ,*/
                          /*cpu->joystick.button_b      ,*/
                          /*cpu->joystick.button_a      ,*/
                          /*cpu->joystick.button_down   ,*/
                          /*cpu->joystick.button_up     ,*/
                          /*cpu->joystick.button_left   ,*/
                          /*cpu->joystick.button_right  );*/
                uint8_t input = 0;
                if ( cpu->joystick.select_button == 0 ) {
                    input =
                        ( cpu->joystick.button_start  << 3 ) |
                        ( cpu->joystick.button_select << 2 ) |
                        ( cpu->joystick.button_b      << 1 ) |
                        ( cpu->joystick.button_a      << 0 ) ;
                }

                if ( cpu->joystick.select_direction == 0 ) {
                    input =
                        ( cpu->joystick.button_down   << 3 ) |
                        ( cpu->joystick.button_up     << 2 ) |
                        ( cpu->joystick.button_left   << 1 ) |
                        ( cpu->joystick.button_right  << 0 ) ;
                }
                return 0xc0 | ( input );
            }
        case 0xff01:
            return 0x00;
        case 0xff02:
            return 0x7e;
        case 0xff0f:
            return interrupt_read_IF( cpu );
        case 0xff04: // DIV
            return read_DIV  ( cpu );
        case 0xff05: // TIMA
            return read_TIMA ( cpu );
        case 0xff06: // TMA
            return read_TMA  ( cpu );
        case 0xff07: // TAC
            return read_TAC  ( cpu ) | 0xf8;
        case 0xff41:
            return display_read_stat ( cpu );
        case 0xff42:
            return read_scroll_y ( cpu );
        case 0xff43:
            return read_scroll_x ( cpu );
        case 0xff44:
            return display_read_LY ( cpu );
        case 0xff45: // LCY
            return display_read_LYC ( cpu );
        case 0xff4a:
            return read_window_y ( cpu );
        case 0xff4b:
            return read_window_x ( cpu );

        case 0xff10:
            return 0 | 0x80;
        case 0xff1a:
            return 0 | 0x7f;
        case 0xff1c:
            return 0 | 0x9f;
        case 0xff20:
            return 0 | 0xc0;
        case 0xff23:
            return 0 | 0x3f;
        case 0xff26:
            return 0 | 0x70;

        case 0xff03:
        case 0xff08:
        case 0xff09:
        case 0xff0a:
        case 0xff0b:
        case 0xff0c:
        case 0xff0d:
        case 0xff0e:
        case 0xff15:
        case 0xff1f:
        case 0xff27:
        case 0xff28:
        case 0xff29:
            return 0 | 0xff;

        case 0xffff:
            return interrupt_read_mask( cpu );

        default:
            break;
    }

    if ( addr >= 0xff4c && addr <= 0xff7f ) {
        return 0xff;
    }

    // No need to check if the address is valid.
    // uint16_t can only hold enough to access the
    // 64kb memory space;
    return cpu->mem_controller.memory [ addr ];
}

uint16_t read_word ( _cpu_info *cpu, uint16_t addr ) {
    return read_byte ( cpu, addr + 1 ) << 8 |
           read_byte ( cpu, addr + 0 ) ;
}

uint16_t read_next_word ( _cpu_info *cpu ) {
    return read_word ( cpu, cpu->pc + 1 );
}

void check_passed ( char c ) {
    static int return_count = 0;
    static int state = 0;

    if ( c == 10 )
        return_count++;

    if ( return_count == 3 ) {
        switch ( c ) {
            case 80:  // P
                state++;
                break;
            case 97:  // a
                state++;
                break;
            case 115: // s
                state++;
                break;
            case 101: // e
                state++;
                break;
            case 100: // d
                state++;
                break;
        }
    }

    if ( state == 6 && c == 10) {
        exit(0);
    }
}

void write_byte ( _cpu_info *cpu, uint16_t addr, uint8_t data ) {
    if ( ( addr >= 0xFE00 ) && ( addr <= 0xFE9F ) ) {
        if (
            ( cpu->dma.oam_dma_timer == 0 )
            &&
            (
             ( cpu->lcd.power == 0 )
             ||
             ( cpu->lcd.mode  <  2 )
            )
        ) {
            cpu->mem_controller.memory [ addr ] = data;
        } else {
            return;
        }
    }

    if ( ( addr >= 0x8000 ) && ( addr <= 0x9fff ) ) {
        if ( cpu->lcd.power == 0 || cpu->lcd.mode < 3 ) {
            cpu->mem_controller.memory [ addr ] = data;
        } else {
            return;
        }
    }

    if ( addr < 0x8000 ) {
        cartridge_write ( cpu, addr, data );
    }

    /*if ( (addr >= 0xff10 && addr <= 0xff26) ||*/
         /*(addr >= 0xff30 && addr <= 0xff3f) ) {*/
    if ( (addr >= 0xff10 && addr <= 0xff19) ||
         (addr >= 0xff24 && addr <= 0xff26) ) {
        apu_write_byte ( cpu, addr, data );
    }

    switch ( addr ) {  // WRITE
        case 0xff00:
            cpu->joystick.select_button    = data & 0x20;
            cpu->joystick.select_direction = data & 0x10;
            break;
        case 0xff01: // Serial OUT
            /*fprintf(stderr, "%c", data);*/
            /*check_passed(data);*/
            /*fprintf(stderr, "%c %d\n", data, data);*/
            break;
        case 0xff04: // DIV
            write_DIV  ( cpu, data );
            break;
        case 0xff05: // TIMA
            write_TIMA ( cpu, data );
            break;
        case 0xff06: // TMA
            write_TMA  ( cpu, data );
            break;
        case 0xff07: // TAC
            write_TAC  ( cpu, data );
            break;
        case 0xff40: // lcd control
            write_lcd_control ( cpu, data );
            break;
        case 0xff41: // lcd stat
            display_write_stat ( cpu, data );
            break;
        case 0xff42:
            write_scroll_y ( cpu, data );
            break;
        case 0xff43:
            write_scroll_x ( cpu, data );
            break;
        case 0xff44:
            // LY is read only
            return;
        case 0xff45:
            // do nothing
            display_write_LYC ( cpu, data );
            break;
        case 0xff46:
            cpu->dma.oam_dma_next_source = (uint16_t) data << 8;
            cpu->dma.oam_dma_delay_timer = 2;
            /*abort();*/
            break;
        case 0xff47: // BG Palette
            write_bg_palette   ( cpu, data ) ;
            break;
        case 0xff48: // sprite Palette 1
            write_spr1_palette ( cpu, data ) ;
            break;
        case 0xff49: // sprite Palette 2
            write_spr2_palette ( cpu, data ) ;
            break;
        case 0xff4a:
            write_window_y ( cpu, data );
            break;
        case 0xff4b:
            write_window_x ( cpu, data );
            break;
        case 0xff0f: // Interrupt FLAG
            cpu->interrupt_flag = data;
            interrupt_write_IF(cpu, data);
            break;
        case 0xffff: // Interrupt ENABLE
            cpu->interrupt_mask = data;
            interrupt_write_mask(cpu, data);
            return;
    }

    cpu->mem_controller.memory [ addr ] = data;
}

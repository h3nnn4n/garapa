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

#include "SDL.h"

#include "automated_tests.h"

#include "debug.h"
#include "types.h"
#include "memory.h"
#include "display.h"
#include "graphics.h"
#include "julia_api.h"

#define dma_read(cpu,addr) (_read_byte(cpu, addr))

typedef struct {
    int16_t  posx;
    int16_t  posy;
    uint8_t  hflip;
    uint8_t  vflip;

    uint8_t  palette_number;

    uint8_t  priority; // 1 if above background
                       // PS: Actual bit in OAM is 0 for when above
    uint8_t  tile;
    uint8_t  tileaddr;

    uint8_t  color_bit1;
    uint8_t  color_bit2;

    uint16_t tile_addr;
} _sprite_info;

_sprite_info sprites[40];
uint8_t sprite_pivot;
uint16_t pixel_pipeline_cycles;

uint8_t priority_cache[144 * 160];
uint8_t sprite_x_cache[144 * 160];
uint8_t sprite_stall_buckets[(168 + 256 + 7) / 8]; // How big?

// 0xff47
void write_bg_palette ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.bg_palette[0] = ( data >> 0 ) & 0x03;
    cpu->lcd.bg_palette[1] = ( data >> 2 ) & 0x03;
    cpu->lcd.bg_palette[2] = ( data >> 4 ) & 0x03;
    cpu->lcd.bg_palette[3] = ( data >> 6 ) & 0x03;
}

// 0xff48
void write_spr1_palette ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.spr1_palette[0] = ( data >> 0 ) & 0x03;
    cpu->lcd.spr1_palette[1] = ( data >> 2 ) & 0x03;
    cpu->lcd.spr1_palette[2] = ( data >> 4 ) & 0x03;
    cpu->lcd.spr1_palette[3] = ( data >> 6 ) & 0x03;
}

// 0xff49
void write_spr2_palette ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.spr2_palette[0] = ( data >> 0 ) & 0x03;
    cpu->lcd.spr2_palette[1] = ( data >> 2 ) & 0x03;
    cpu->lcd.spr2_palette[2] = ( data >> 4 ) & 0x03;
    cpu->lcd.spr2_palette[3] = ( data >> 6 ) & 0x03;
}

void write_scroll_y ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.bg_scroll_y = data;
}

void write_scroll_x ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.bg_scroll_x = data;
}

void write_window_y ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.window_scroll_y = data;
}

void write_window_x ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.window_scroll_x = data;
}

// SCY
uint8_t read_scroll_y ( _cpu_info *cpu ) {
    return cpu->lcd.bg_scroll_y;
}

// SCX
uint8_t read_scroll_x ( _cpu_info *cpu ) {
    return cpu->lcd.bg_scroll_x;
}

uint8_t read_window_y ( _cpu_info *cpu ) {
    return cpu->lcd.window_scroll_y;
}

uint8_t read_window_x ( _cpu_info *cpu ) {
    return cpu->lcd.window_scroll_x;
}

uint8_t read_lcd_control ( _cpu_info *cpu ) {
    return dma_read ( cpu, 0xff40 );
}

// 0xff40
void write_lcd_control ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.power          = ( data & 0x80 ) != 0; // Bit 7
    cpu->lcd.window_tilemap = ( data & 0x40 ) != 0; // Bit 6
    cpu->lcd.window_enabled = ( data & 0x20 ) != 0; // Bit 5
    cpu->lcd.bg_and_tilemap = ( data & 0x10 ) != 0; // Bit 4
    cpu->lcd.bg_tileset     = ( data & 0x08 ) != 0; // Bit 3
    cpu->lcd.sprite_size    = ( data & 0x04 ) != 0; // Bit 2
    cpu->lcd.sprite_enable  = ( data & 0x02 ) != 0; // Bit 1
    cpu->lcd.bg_enable      = ( data & 0x01 ) != 0; // Bit 0

    if ( cpu->lcd.power == 0 ) {
        cpu->lcd.active_line  = 0;
        cpu->lcd.cycles_spent = 0;
        cpu->lcd.mode         = 0;
        /*printf("LCD is now off\n");*/
    } else {
        /*printf("LCD is now on\n");*/
    }

    /*if ( debug_display ) printf("ff40 lcd control write\n");*/
}

// 0 = Off
// 1 = On
// bit 7
uint8_t display_test_lcdpower ( _cpu_info *cpu ) {
    return cpu->lcd.power;
}

// 0 = 0x9800 - 0x9bff
// 1 = 0x9c00 - 0x9fff
// bit 6
uint8_t display_test_windowtilemap ( _cpu_info *cpu ) {
    return cpu->lcd.window_tilemap;
}

// 0 = Disabled
// 1 = Disabled
// bit 5
uint8_t display_test_windowenable ( _cpu_info *cpu ) {
    return cpu->lcd.window_enabled;
}

// 0 = 0x8800 - 0x97ff
// 1 = 0x8000 - 0xfff
// bit 4
uint8_t display_test_bg_tileset_select ( _cpu_info *cpu ) {
    return cpu->lcd.bg_and_tilemap;
}

// 0 = 0x9800 - 0x9bff
// 1 = 0x9c00 - 0x9fff
// bit 3
uint8_t display_test_tilemap_select ( _cpu_info *cpu ) {
    return cpu->lcd.bg_tileset;
}

// 0 = 8x8
// 1 = 8x16
// bit 2
uint8_t display_test_sprite_size ( _cpu_info *cpu ) {
    return cpu->lcd.sprite_size;
}

// 0 = Disabled
// 1 = Disabled
uint8_t display_test_sprite_enabled ( _cpu_info *cpu ) {
    return cpu->lcd.sprite_enable;
}

// 0 = Disabled
// 1 = Disabled
uint8_t display_test_bg_enabled ( _cpu_info *cpu ) {
    return cpu->lcd.bg_enable;
}

// Current horizontal line being draw
// mapped 0xff44
uint8_t display_read_LY ( _cpu_info *cpu ) {
    if ( display_test_lcdpower(cpu) )
        return cpu->lcd.active_line;

    return 0x00;
}

// The value on 0xff45 used to trigger the LYC interrupt
uint8_t display_read_LYC ( _cpu_info *cpu ) {
    if ( display_test_lcdpower(cpu) )
        return cpu->lcd.lyc_trigger;

    return 0x00;
}

void display_write_LYC ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.lyc_trigger = data;
}

uint8_t display_read_stat  ( _cpu_info *cpu ) {
    return 0x80 |
        ( cpu->lcd.lyc_enable    << 6 ) |
        ( cpu->lcd.mode2_oam     << 5 ) |
        ( cpu->lcd.mode1_vblank  << 4 ) |
        ( cpu->lcd.mode0_hblank  << 3 ) |
        ( cpu->lcd.lyc_delay     == 0  &&
          cpu->lcd.lyc_trigger   ==
          cpu->lcd.active_line ) << 2   |
        ( cpu->lcd.mode );
    if ( debug_display ) printf("ff41 lcd control write\n");
}

// 0xff41 lcd stat
void display_write_stat ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.lyc_enable   = (data & 0x40) != 0; // bit 6
    cpu->lcd.mode2_oam    = (data & 0x20) != 0; // bit 5
    cpu->lcd.mode1_vblank = (data & 0x10) != 0; // bit 4
    cpu->lcd.mode0_hblank = (data & 0x08) != 0; // bit 3
    if ( debug_display ) printf("ff41 lcd control write\n");
}

// LY = LYC check enable
uint8_t display_test_LYC_enable ( _cpu_info *cpu ) {
    return cpu->lcd.lyc_enable;
}

uint8_t display_test_mode2_oam ( _cpu_info *cpu ) {
    return cpu->lcd.mode2_oam;
}

uint8_t display_test_mode1_vblank ( _cpu_info *cpu ) {
    return cpu->lcd.mode1_vblank;
}

uint8_t display_test_mode0_hblank ( _cpu_info *cpu ) {
    return cpu->lcd.mode0_hblank;
}

uint8_t display_test_LYC_LY_trigger ( _cpu_info *cpu ) {
    return cpu->lcd.lyc_ly_triggered;
}

uint8_t display_test_screenmode ( _cpu_info *cpu ) {
    if ( display_test_lcdpower(cpu) )
        return cpu->lcd.mode;

    return 0x00;
}

/*
 *0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
 *4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
 *8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
 *A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
 *C000-CFFF   4KB Work RAM Bank 0 (WRAM)
 *D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
 *E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
 *FE00-FE9F   Sprite Attribute Table (OAM)
 *FEA0-FEFF   Not Usable
 *FF00-FF7F   I/O Ports
 *FF80-FFFE   High RAM (HRAM)
 *FFFF        Interrupt Enable Register
 */

/*
 * FF40 - LCDC - LCD Control (R/W)
 * Bit 7 - LCD Display Enable             (0=Off, 1=On)
 * Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
 * Bit 5 - Window Display Enable          (0=Off, 1=On)
 * Bit 4 - BG & Window Tile Data Select   (0=8800-97FF, 1=8000-8FFF)
 * Bit 3 - BG Tile Map Display Select     (0=9800-9BFF, 1=9C00-9FFF)
 * Bit 2 - OBJ (Sprite) Size              (0=8x8, 1=8x16)
 * Bit 1 - OBJ (Sprite) Display Enable    (0=Off, 1=On)
 * Bit 0 - BG Display (for CGB see below) (0=Off, 1=On)
 */

void draw_background_and_window( _cpu_info *cpu ) {
    uint32_t *buffer = get_frame_buffer();
    uint8_t  *memory = cpu->mem_controller.memory;
    if ( buffer == NULL ) return;

    uint16_t posx;
    uint16_t posy;
    uint8_t color;

    uint16_t bg_addr;
    uint16_t tile_addr;

    /*for (int i = 0; i < 160; i+=2) { // screen is 160 pixels wide*/
    for (int i = 0; i < 160; i+=1) { // screen is 160 pixels wide
        if ( display_test_windowenable ( cpu ) &&          // If windows is active
            cpu->lcd.active_line >= read_window_y ( cpu ) &&  // and on current scanline
            read_window_y ( cpu ) - cpu->lcd.active_line < 144 // and on current scanline
            ) {
            posy = cpu->lcd.active_line - read_window_y ( cpu ); // Calculate the y position
            posx = i - read_window_x ( cpu ) + 7;
            bg_addr = display_test_windowtilemap ( cpu ) ?       // Select address with the tilemap
                            0x9c00:
                            0x9800;
        } else {
            posy = cpu->lcd.active_line + read_scroll_y ( cpu );  // Calculate the y position
            bg_addr = display_test_tilemap_select ( cpu ) ?       // Select address with the tilemap
                            0x9c00:
                            0x9800;
            posx = read_scroll_x ( cpu ) + i;
            posy %= 256;
            posx %= 256;
        }

        uint8_t tile_number = memory[bg_addr + (posy/8) * 32 + posx/8]; // Read the tilenumber

        if (display_test_bg_tileset_select ( cpu ) ) // Reads from the correct position
            tile_addr = 0x8000 +  tile_number * 16;
        else
            tile_addr = 0x9000 +  ((int8_t) tile_number * 16); // Uses 0x9000 instead 0x8800 because the tilenumber here has a signal

        uint16_t offset = (posy % 8 ) * 2;
        uint8_t bit1 = dma_read(cpu, tile_addr + offset    );
        uint8_t bit2 = dma_read(cpu, tile_addr + offset + 1);

        color = (((bit2 & (0x01 << (((posx % 8) - 7) * -1))) != 0) << 1) |
                 ((bit1 & (0x01 << (((posx % 8) - 7) * -1))) != 0)       ;

        priority_cache[cpu->lcd.active_line * 160 + i] = color > 0 ? 1 : 0;

        buffer[cpu->lcd.active_line*160 + i] = cpu->lcd.colors[cpu->lcd.bg_palette[color]];

        test_write_to_buffer(&test_control,
                              cpu->lcd.active_line*160 + i,
                              cpu->lcd.colors[cpu->lcd.bg_palette[color]]);
    }
}

void fetch_sprites ( _cpu_info *cpu ) {
    sprite_pivot          = 0;

    // OAM = 0xfe00
    // bit0 = y
    // bit1 = x
    // bit2 = tile number
    // bit3 = atributes
    // TODO: Sprites Priority
    for (int i = 0; i < 40; ++i) { // Loops over the 40 sprites
        int16_t posy      = dma_read(cpu, 0xfe00 + (i*4    )) - 16; // Reads the y coordinate
        int16_t posx      = dma_read(cpu, 0xfe00 + (i*4 + 1)) - 8 ; // Reads the x coordinate
        uint16_t tileaddr = dma_read(cpu, 0xfe00 + (i*4 + 2)); // Tile index
        uint8_t flags     = dma_read(cpu, 0xfe00 + (i*4 + 3)); // Tile flags

        sprites[sprite_pivot].posx  = posx;
        sprites[sprite_pivot].posy  = posy;
        sprites[sprite_pivot].vflip = !!(flags & 0x40);
        sprites[sprite_pivot].hflip = !!(flags & 0x20);
        sprites[sprite_pivot].tile  = tileaddr;

        sprites[sprite_pivot].palette_number = !!(flags & 0x08);

        /*printf(" sprite %04x x: %4d  y: %4d\n", 0xfe00 + (i*4    ), posx, posy);*/

        if ( ( cpu->lcd.active_line >= posy ) && // Checks if the sprite overlaps the current line
             ( cpu->lcd.active_line < posy + ( cpu->lcd.sprite_size ? 16 : 8 ) ) ) {

            uint8_t line_offset = cpu->lcd.active_line - posy;

            if ( flags & 0x40 ) // VHLIP
                line_offset = (7 + display_test_sprite_size(cpu)*8) - line_offset;

            line_offset *= 2;

            uint16_t addr = 0x8000 + (tileaddr * 16) + line_offset;

            sprites[sprite_pivot].tileaddr   = addr;
            sprites[sprite_pivot].color_bit1 = dma_read(cpu, addr    );
            sprites[sprite_pivot].color_bit2 = dma_read(cpu, addr + 1);

            sprite_pivot ++;
        }
    }
}

void sort_sprites ( ) {
    // Dumb bubble sort
    // Should be fast enough

    int run;

    do {
        run = 0;
        for (int i = 0; i < sprite_pivot; ++i) {
            if ( (sprites[i].posx >  sprites[i+1].posy) ||
                ((sprites[i].posx == sprites[i+1].posy) && sprites[i].tile > sprites[i+1].tile)) {

                _sprite_info t = sprites[i  ];
                sprites[i  ]   = sprites[i+1];
                sprites[i+1]   = t;

                run = 1;
            }
        }
    } while ( run );
}

void draw_sprites ( _cpu_info *cpu ) {
    uint32_t *buffer = get_frame_buffer();
    if ( buffer == NULL ) return; // FIXME

    pixel_pipeline_cycles    = 0;

    uint8_t  rendered        = 0;
    uint16_t has_sprite_at_0 = 0;
    uint16_t sprites_draw    = 0;
    uint32_t cache_index     = 0;

    // OAM = 0xfe00
    // bit0 = y
    // bit1 = x
    // bit2 = tile number
    // bit3 = atributes
    // TODO: Sprites Priority

    int sprites_to_draw = sprite_pivot <= 10 ? sprite_pivot : 10;
    for (int i = 0; i < sprites_to_draw; ++i) {
        int16_t posy      = sprites[i].posy;
        int16_t posx      = sprites[i].posx;
        uint8_t pallete   = sprites[i].palette_number;

        if ( sprites_draw >= 10 ) break;

        if ( ( cpu->lcd.active_line >= posy ) &&
             ( cpu->lcd.active_line < posy + ( cpu->lcd.sprite_size ? 16 : 8 ) ) ) {

            uint8_t bit1 = sprites[i].color_bit1;
            uint8_t bit2 = sprites[i].color_bit2;

            for (int j = 0; j < 8; ++j) {
                uint8_t color;
                uint8_t j_flip;
                cache_index = cpu->lcd.active_line * 160 + posx + j;
                uint8_t pcache = priority_cache[cache_index];

                if ( (pcache & 0x02 ) &&
                     (sprite_x_cache[cache_index] <= posx +8 ) )
                    continue;

                if ( pcache & 0x04 )
                    continue;

                if ( (pcache & 0x01) &&
                     (sprites[i].priority) )
                    continue;

                if ( sprites[i].hflip )
                    j_flip = 7 - j;
                else
                    j_flip = j;

                if ( posx + j_flip >= 160 )
                    continue;

                color = (((bit2 & (0x80 >> j_flip)) != 0) << 1) |
                         ((bit1 & (0x80 >> j_flip)) != 0)       ;

                priority_cache[cache_index] |= color > 0 ? 0x02 : 0x00;
                sprite_x_cache[cache_index]  = posx + j;

                rendered = 1;

                if ( !color ) continue;

                buffer[cpu->lcd.active_line*160 + posx + j] = pallete ? cpu->lcd.colors[cpu->lcd.spr2_palette[color]] :
                                                                        cpu->lcd.colors[cpu->lcd.spr1_palette[color]];
                test_write_to_buffer(&test_control,
                                      cpu->lcd.active_line*160 + posx + j,
                                      pallete ? cpu->lcd.colors[cpu->lcd.spr2_palette[color]] :
                                                cpu->lcd.colors[cpu->lcd.spr1_palette[color]]);
            }

            if ( rendered ) {
                int16_t posx2 = posx + 8;
                if ( posx2 < 168 ) {
                    pixel_pipeline_cycles += 6;

                    int16_t x = posx2 + (int16_t) cpu->lcd.bg_scroll_x;

                    if ( x < 0 ) {
                        x = 0;
                    }

                    if ( x <= 0 ) {
                        has_sprite_at_0 = 1;
                    }

                    uint32_t bucket_index = (x >> 3);
                    int32_t stall = 5 - (x & 0x07);

                    if ( stall < 0) {
                        stall = 0;
                    }

                    sprite_stall_buckets[bucket_index] =
                        stall > sprite_stall_buckets[bucket_index] ?
                        stall :
                        sprite_stall_buckets[bucket_index];
                }

                sprites_draw++;
            }
        }
    }

    for (int i = 0; i < ((168 + cpu->lcd.bg_scroll_x + 7) / 8); ++i) {
        pixel_pipeline_cycles += sprite_stall_buckets[i];
    }

    if ( has_sprite_at_0 ) {
        pixel_pipeline_cycles += cpu->lcd.bg_scroll_x & 0x07;
    }

    pixel_pipeline_cycles &= 0xfc;
}

// Mode 0 lasts 201-207
// Mode 2 lasts 77-83
// Mode 3 lasts 169-174
//
// Modes 0 + 2 + 3 takes 456 clocks
//
// VBLANK lasts 4560 clocks
//
// Vertical refresh every 70224 clocks
//
// Scanline: Mode 2 -> 3 -> 0
// Lines 144-153: Mode 1
//
// Mode 0: Access VRAM and OAM
// Mode 1: Access VRAM and OAM
// Mode 2: Access VRAM but no OAM
// Mode 3: No access to VRAM and OAM

void display_update( _cpu_info *cpu ) {
    uint8_t         irq             = 0;

    if ( !display_test_lcdpower(cpu) ) return;

    if ( debug_display ) printf(" PPU: Power: %2d  Spent: %3d  mode: %2d  ly: %3d  lyc: %3d  ",
            cpu->lcd.power,
            cpu->lcd.cycles_spent, cpu->lcd.mode, cpu->lcd.active_line, cpu->lcd.lyc_trigger);

    cpu->lcd.mode_cmp = 255;

    if ( cpu->lcd.lyc_delay > 0 ) {
        cpu->lcd.lyc_delay --;
    }

    cpu->lcd.cycles_spent++;

    if ( cpu->lcd.mode == 0 && cpu->lcd.cycles_spent == 5 ) {
        if ( cpu->lcd.active_line < 144 ) {
            cpu->lcd.mode = 2;
        } else {
            cpu->lcd.mode = 1;
            /*cpu->lcd.active_line = 0;*/

            /*printf("VBLANK Interrupt\n");*/
            cpu->interrupts.pending_vblank = 1;

            garapa_jl_display_update();
            flip_screen( cpu );
            cpu->die = test_step ( &test_control );
        }
    } else if ( cpu->lcd.mode == 0 && cpu->lcd.cycles_spent >= 1 && cpu->lcd.cycles_spent < 5 && cpu->lcd.active_line >= 1 &&
                cpu->lcd.active_line <= 143 ) {
        cpu->lcd.mode_cmp = 2;
    } else if ( cpu->lcd.mode == 2 && cpu->lcd.cycles_spent == 85 ) {
        cpu->lcd.mode = 3;

        cpu->lcd.m3_cycles = 175;

        if ( ( cpu->lcd.bg_scroll_x % 8 ) == 0 ) {
            // Do nothing
        } else if ( ( cpu->lcd.bg_scroll_x % 8 ) <= 4 ) {
            cpu->lcd.m3_cycles += 4;
        } else {
            cpu->lcd.m3_cycles += 8;
        }

        for (int i = 0; i < 160 * 144; ++i) {
            priority_cache[i] = 0;
            sprite_x_cache[i] = 0;
        }

        for (int i = 0; i < (160 + 256 + 7)/8; ++i) {
            sprite_stall_buckets[i] = 0;
        }

        draw_background_and_window(cpu);

        if ( cpu->lcd.power && cpu->lcd.sprite_enable ) {
            fetch_sprites(cpu);
            /*sort_sprites(cpu);*/
            draw_sprites(cpu);
            cpu->lcd.m3_cycles += pixel_pipeline_cycles;
        }

    } else if ( cpu->lcd.mode == 3 && cpu->lcd.cycles_spent >= ((85 + cpu->lcd.m3_cycles) -7 ) &&
            cpu->lcd.cycles_spent < (85 + cpu->lcd.m3_cycles ) ) {
        cpu->lcd.mode_cmp = 0;
    } else if ( cpu->lcd.mode == 3 && cpu->lcd.cycles_spent == (85 + cpu->lcd.m3_cycles) ) {
        cpu->lcd.mode = 0;
    } else if ( cpu->lcd.mode == 0 && cpu->lcd.cycles_spent == 457 ) {
        cpu->lcd.active_line += 1;
        cpu->lcd.lyc_delay = 3;
        cpu->lcd.mode = 0;
        cpu->lcd.cycles_spent = 1;
    } else if ( cpu->lcd.mode == 1 ) {
        if ( cpu->lcd.cycles_spent == 457 ) {
            cpu->lcd.cycles_spent = 1;

            if ( cpu->lcd.active_line == 0 ) {
                cpu->lcd.mode = 0;
            } else {
                cpu->lcd.active_line += 1;
                cpu->lcd.lyc_delay = 3;
            }
        } else if ( cpu->lcd.active_line == 153 && cpu->lcd.cycles_spent == 5 ) {
            cpu->lcd.active_line = 0;
            cpu->lcd.lyc_delay = 3;
        }
    }

    if ( cpu->lcd.mode_cmp == 255 ) {
        cpu->lcd.mode_cmp = cpu->lcd.mode;
    }

    if (((cpu->lcd.lyc_delay == 0) && (cpu->lcd.active_line == cpu->lcd.lyc_trigger) && (cpu->lcd.lyc_enable == 1)) ||
         (cpu->lcd.mode_cmp == 0 && cpu->lcd.mode0_hblank ) ||
         (cpu->lcd.mode_cmp == 2 && cpu->lcd.mode2_oam    ) ||
         (cpu->lcd.mode_cmp == 1 && (cpu->lcd.mode2_oam || cpu->lcd.mode1_vblank))) {
        irq = 1;
    }

    if ( debug_display ) printf("m3_cycles: %3d  irq: %2d %2d\n", cpu->lcd.m3_cycles, cpu->lcd.stat_irq, irq);

    if ( !cpu->lcd.stat_irq && irq ) {
        /*printf("STAT Interrupt\n");*/
        if ( debug_display ) {
            printf("STAT Interrupt: ");
            if((cpu->lcd.lyc_delay == 0) && (cpu->lcd.active_line == cpu->lcd.lyc_trigger) && (cpu->lcd.lyc_enable == 1)) printf("LYC\n");
            if (cpu->lcd.mode_cmp == 0 && cpu->lcd.mode0_hblank ) printf("MODE0\n");
            if (cpu->lcd.mode_cmp == 2 && cpu->lcd.mode2_oam    ) printf("MODE2\n");
            if (cpu->lcd.mode_cmp == 1 && (cpu->lcd.mode2_oam || cpu->lcd.mode1_vblank)) printf("MODE1\n");
        }
        cpu->interrupts.pending_lcdstat = 1;
    }

    cpu->lcd.stat_irq = irq;
}

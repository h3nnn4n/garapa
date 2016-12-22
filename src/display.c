#include <SDL/SDL.h>
#include <stdio.h>

#include "types.h"
#include "memory.h"
#include "display.h"
#include "graphics.h"

static int bgpalette[]          = {3, 2, 1, 0};
static int sprpalette1[]        = {0, 1, 2, 3};
/*static int sprpalette2[]        = {0, 1, 2, 3};*/
static unsigned long colours[4] = {0xFFFFFF, 0xC0C0C0, 0x808080, 0x000000};

void write_scroll_y ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.scy = data;
}

void write_scroll_x ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.scx = data;
}

void write_window_y ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.bgy = data;
}

void write_window_x ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.bgx = data;
}

// SCY
uint8_t read_scroll_y ( _cpu_info *cpu ) {
    return cpu->lcd.scy;
}

// SCX
uint8_t read_scroll_x ( _cpu_info *cpu ) {
    return cpu->lcd.scx;
}

uint8_t read_window_y ( _cpu_info *cpu ) {
    return cpu->lcd.bgy;
}

uint8_t read_window_x ( _cpu_info *cpu ) {
    return cpu->lcd.bgx;
}

uint8_t read_active_line( _cpu_info *cpu ) {
    return cpu->lcd.active_line;
}

uint8_t read_lcd_control ( _cpu_info *cpu ) {
    return read_byte ( cpu, 0xff40 );
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
uint8_t display_read_LY ( _cpu_info *cpu ) {
    if ( display_test_lcdpower(cpu) )
        return cpu->lcd.active_line;
    return 0x00;
}

uint8_t display_read_stat  ( _cpu_info *cpu ) {
    return ( display_test_LYC_enable( cpu )) << 6 | cpu->lcd.mode;
}

void display_write_stat ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.lyc_enable   = (data & 0x40) != 0; // bit 6
    cpu->lcd.mode2_oam    = (data & 0x20) != 0; // bit 5
    cpu->lcd.mode1_vblank = (data & 0x10) != 0; // bit 4
    cpu->lcd.mode0_hblank = (data & 0x08) != 0; // bit 3
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

void draw_background_and_window( _cpu_info *cpu ) {
    uint8_t *buffer = screen->pixels;

    for (int x = 0; x < 160; ++x) {
        unsigned int map_select, map_offset, tile_num, tile_addr, xm, ym;
        unsigned char b1, b2, mask, colour;

        if( read_active_line(cpu) >= read_window_y(cpu) &&
            display_test_windowenable(cpu) &&
            read_active_line(cpu) - read_window_y(cpu) < 144) {
            xm = x;
            ym = read_active_line(cpu) - read_window_y(cpu);
            map_select = display_test_windowtilemap(cpu); } else { if(!display_test_bg_enabled(cpu) ) {
                buffer[read_active_line(cpu)*160 + x] = 0;
                return;
            }
            xm = (                    x + read_scroll_x(cpu))%256;
            ym = (read_active_line(cpu) + read_scroll_y(cpu))%256;
            map_select = display_test_tilemap_select(cpu);
        }

        map_offset = (ym/8)*32 + xm/8;

        tile_num = cpu->memory[(0x9800 + map_select*0x400 + map_offset)];
        if(display_test_bg_tileset_select(cpu)) {
            tile_addr = 0x8000 + tile_num*16;
        } else {
            tile_addr = 0x9000 + ((signed char)tile_num)*16;
        }

        b1 = cpu->memory[(tile_addr+(ym%8)*2)];
        b2 = cpu->memory[(tile_addr+(ym%8)*2+1)];
        mask = 128>>(xm%8);
        colour = (!!(b2&mask)<<1) | !!(b1&mask);
        buffer[read_active_line(cpu)*160 + x] = colours[bgpalette[colour]];
    }
}

void draw_sprites ( _cpu_info *cpu ) {
    uint8_t *buffer = screen->pixels;

    // OAM = 0xfe00
    // bit0 = y
    // bit1 = x
    // bit2 = tile number
    // bit3 = atributes
    // TODO: HFLIP and VFLIP
    for (int i = 0; i < 40; ++i) { // Loops over the 40 sprites
        uint8_t posy      = read_byte(cpu, 0xfe00 + (i*4    )) - 16; // Reads the y coordinate
        uint8_t posx      = read_byte(cpu, 0xfe00 + (i*4 + 1)) - 8 ; // Reads the x coordinate
        uint16_t tileaddr = read_byte(cpu, 0xfe00 + (i*4 + 2)); // Tile index

        /*if ( cpu->cycles_machine > 46022942 ) printf("Sprite %2d: x: %3d y: %3d\n", i, posx, posy);*/

        if ( ( cpu->lcd.active_line >= posy ) && // Checks if the sprite overlaps the current line
             ( cpu->lcd.active_line < posy + ( cpu->lcd.sprite_size ? 16 : 8 ) ) ) {

            /*if ( cpu->cycles_machine > 46022942 )*/
                /*printf("Sprite %2d: x: %3d y: %3d\n", i, posx, posy);*/

            uint8_t line_offset = (cpu->lcd.active_line - posy) * 2; // 2 bytes per line

            uint16_t addr = 0x8000 + (tileaddr * 16) + line_offset;
            uint8_t bit1 = read_byte(cpu, addr    );
            uint8_t bit2 = read_byte(cpu, addr + 1);

            for (int j = 0; j < 8; ++j) { // Loops over the pixels
                uint8_t color;

                if ( posx + j >= 160 ) // Out of screen
                    continue;

                color = (((bit2 & (0x80 >> j)) != 0) << 1) |
                         ((bit1 & (0x80 >> j)) != 0)       ;

                if ( !color ) continue; // Transparent

                buffer[cpu->lcd.active_line*160 + posx + j] = colours[sprpalette1[color]];
            }
        }
    }
}

void display_update( _cpu_info *cpu ) {
    uint32_t cycles = cpu->cycles_machine;
    uint32_t frame;
    static uint16_t last_line;

    if ( !display_test_lcdpower(cpu) ) return;

    frame    = cycles % (70224/4);
    cpu->lcd.active_line = frame / (456/4);

           if (frame < 204/4 ) {
        cpu->lcd.mode = 2;
    } else if (frame < 284/4) {
        cpu->lcd.mode = 3;
    } else if (frame < 456/4) {
        cpu->lcd.mode = 0;
    }

    if ( cpu->lcd.active_line >= 144 ) {
        cpu->lcd.mode = 1;
    }

    if ( cpu->lcd.active_line != last_line &&
         cpu->lcd.active_line < 144 ) {
        draw_background_and_window(cpu); // Draw one line of the background
        draw_sprites(cpu);
    }

    if ( display_read_LY(cpu) == read_byte(cpu, 0xff45) &&
         display_test_LYC_enable(cpu) ){
        cpu->interrupts.pending_lcdstat = 1; abort();
    }

    if ( cpu->lcd.mode == 2 && cpu->lcd.mode2_oam ) {
        cpu->interrupts.pending_lcdstat = 1;
    }

    if ( cpu->lcd.mode == 1 && cpu->lcd.mode1_vblank ) {
        cpu->interrupts.pending_lcdstat = 1;
    }

    if ( cpu->lcd.mode == 0 && cpu->lcd.mode0_hblank ) {
        cpu->interrupts.pending_lcdstat = 1;
    }

    /*printf("MEU: mode: %x line: %3d frame: %6d\n", cpu->lcd.mode, cpu->lcd.active_line, frame);*/

    if ( last_line == 143 && cpu->lcd.active_line == 144 ) {
        /*printf("MEU: LCD requested vblank int\n");*/
        cpu->interrupts.pending_vblank = 1;
        flip_screen();
    }

    last_line = cpu->lcd.active_line;
}

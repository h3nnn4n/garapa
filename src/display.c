#include <stdio.h>

#include "SDL.h"

#include "types.h"
#include "memory.h"
#include "display.h"
#include "graphics.h"

void write_spr1_palette ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.bg_palette[0] = ( data >> 0 ) & 0x03;
    cpu->lcd.bg_palette[1] = ( data >> 2 ) & 0x03;
    cpu->lcd.bg_palette[2] = ( data >> 4 ) & 0x03;
    cpu->lcd.bg_palette[3] = ( data >> 6 ) & 0x03;
}

void write_spr2_palette ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.spr1_palette[0] = ( data >> 0 ) & 0x03;
    cpu->lcd.spr1_palette[1] = ( data >> 2 ) & 0x03;
    cpu->lcd.spr1_palette[2] = ( data >> 4 ) & 0x03;
    cpu->lcd.spr1_palette[3] = ( data >> 6 ) & 0x03;
}

void write_bg_palette ( _cpu_info *cpu, uint8_t data ) {
    cpu->lcd.spr1_palette[0] = ( data >> 0 ) & 0x03;
    cpu->lcd.spr1_palette[1] = ( data >> 2 ) & 0x03;
    cpu->lcd.spr1_palette[2] = ( data >> 4 ) & 0x03;
    cpu->lcd.spr1_palette[3] = ( data >> 6 ) & 0x03;

}

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

    uint8_t posx;
    uint8_t posy;
    uint8_t color;

    uint16_t bg_addr;
    uint16_t tile_addr;

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
        uint8_t bit1 = read_byte(cpu, tile_addr + offset    );
        uint8_t bit2 = read_byte(cpu, tile_addr + offset + 1);

        color = (((bit2 & (0x01 << (((posx % 8) - 7) * -1))) != 0) << 1) |
                 ((bit1 & (0x01 << (((posx % 8) - 7) * -1))) != 0)       ;

        buffer[cpu->lcd.active_line*160 + i] = cpu->lcd.colors[cpu->lcd.bg_palette[color]];
    }
}

void draw_sprites ( _cpu_info *cpu ) {
    uint32_t *buffer = get_frame_buffer();

    // OAM = 0xfe00
    // bit0 = y
    // bit1 = x
    // bit2 = tile number
    // bit3 = atributes
    // TODO: Sprites Priority
    for (int i = 0; i < 40; ++i) { // Loops over the 40 sprites
        uint8_t posy      = read_byte(cpu, 0xfe00 + (i*4    )) - 16; // Reads the y coordinate
        uint8_t posx      = read_byte(cpu, 0xfe00 + (i*4 + 1)) - 8 ; // Reads the x coordinate
        uint16_t tileaddr = read_byte(cpu, 0xfe00 + (i*4 + 2)); // Tile index
        uint8_t flags     = read_byte(cpu, 0xfe00 + (i*4 + 3)); // Tile flags

        /*if ( cpu->cycles_machine > 46022942 ) printf("Sprite %2d: x: %3d y: %3d\n", i, posx, posy);*/

        if ( ( cpu->lcd.active_line >= posy ) && // Checks if the sprite overlaps the current line
             ( cpu->lcd.active_line < posy + ( cpu->lcd.sprite_size ? 16 : 8 ) ) ) {

            /*if ( cpu->cycles_machine > 46022942 )*/
                /*printf("Sprite %2d: x: %3d y: %3d\n", i, posx, posy);*/

            uint8_t line_offset = cpu->lcd.active_line - posy;

            if ( flags & 0x40 ) // VHLIP
                line_offset = (7 + display_test_sprite_size(cpu)*8) - line_offset;

            line_offset *= 2;

            uint16_t addr = 0x8000 + (tileaddr * 16) + line_offset;
            uint8_t bit1 = read_byte(cpu, addr    );
            uint8_t bit2 = read_byte(cpu, addr + 1);

            for (int j = 0; j < 8; ++j) { // Loops over the pixels
                uint8_t color;
                uint8_t j_flip;

                if ( flags & 0x20 ) // HFLIP
                    j_flip = 7 - j;
                else
                    j_flip = j;

                if ( posx + j_flip >= 160 ) // Out of screen
                    continue;

                color = (((bit2 & (0x80 >> j_flip)) != 0) << 1) |
                         ((bit1 & (0x80 >> j_flip)) != 0)       ;

                if ( !color ) continue; // Transparent

                buffer[cpu->lcd.active_line*160 + posx + j] = cpu->lcd.colors[cpu->lcd.bg_palette[color]];
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
        draw_background_and_window(cpu);
        draw_sprites(cpu);
    }

    if ( display_read_LY(cpu) == read_byte(cpu, 0xff45) &&
         display_test_LYC_enable(cpu) ){
        cpu->interrupts.pending_lcdstat = 1;
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

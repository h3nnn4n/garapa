#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "interrupts.h"
#include "memory.h"
#include "time_keeper.h"
#include "utils.h"
#include "types.h"
#include "graphics.h"
#include "display.h"

void load_rom ( _cpu_info *cpu, const char* fname, uint16_t offset ) {
    FILE *f = NULL;
    off_t buffer_size = -1;

    f = fopen(fname, "rb");

    buffer_size = fsize( fname );

    assert( offset + buffer_size <= 512 * 1024 && "ROM too big for the 64k bytes RAM");
    if ( fread(cpu->rom + offset, buffer_size, 1, f) != 1 ) {
        printf("Something went weird while reading into buffer\n");
    }

    memcpy(&cpu->memory[0x0000], &cpu->rom[0x0000], 0x4000);
    memcpy(&cpu->memory[0x4000], &cpu->rom[0x4000], 0x4000);

    fclose(f);

    /*fprintf(stderr, "Loaded %s into %04x to %04x\n", fname, offset, offset +(uint16_t) buffer_size);*/
}

uint8_t read_byte ( _cpu_info *cpu, uint16_t addr ) {

    if ( cpu->DMA_in_progress && addr < 0xff80 ) {
        uint8_t dx = cpu->cycles_machine - cpu->DMA_in_progress;
        if ( dx >= 160 ) {
            cpu->DMA_in_progress = 0;
        } else {
            return cpu->memory[0xff80 + dx];
        }
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
        case 0xffff:
            return interrupt_read_mask( cpu );
        case 0xff0f:
            return interrupt_read_IF( cpu );
        case 0xff04: // DIV
            return read_DIV  ( cpu );
        case 0xff05: // TIMA
            return read_TIMA ( cpu );
        case 0xff06: // TMA
            return read_TMA  ( cpu );
        case 0xff07: // TAC
            return read_TAC  ( cpu );
        case 0xff41:
            return display_read_stat ( cpu );
        case 0xff42:
            return read_scroll_y ( cpu );
        case 0xff43:
            return read_scroll_x ( cpu );
        case 0xff44:
            return display_read_LY ( cpu );
        case 0xff45: // LCY
            break;
        case 0xff4a:
            return read_window_y ( cpu );
        case 0xff4b:
            return read_window_x ( cpu );
    }

    // No need to check if the address is valid.
    // uint16_t can only hold enough to access the
    // 64kb memory space;
    return cpu->memory [ addr ];
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
    /*uint8_t */
    uint8_t mapper = cpu->memory[0x0147];
    static uint8_t upper = 0;
    static uint8_t ram_select = 0;

    switch (mapper) {
        case 0x00:
            if ( addr < 0x8000 ) {
                /*printf("Tried to write to 0x%04x\n", addr);*/
                return;
            }
            break;
        case 0x01:
            if ( addr < 0x2000 ) {
                /*printf("Tried to write to 0x%04x\n", addr);*/
                return;
            } else if ( addr >= 0x2000 && addr < 0x4000 ) {
                cpu->active_bank = data & 0x1f;
                if ( !ram_select )
                    cpu->active_bank |= upper;

                if ( cpu->active_bank == 0x00 ||
                     cpu->active_bank == 0x20 ||
                     cpu->active_bank == 0x40 ||
                     cpu->active_bank == 0x60 ) {
                    cpu->active_bank++;
                }

                /*printf("Tried to write to 0x%04x\n", addr);*/

                memcpy(&cpu->memory[0x4000], &cpu->rom[cpu->active_bank * 0x4000], 0x4000);
                return;
            } else if ( addr >= 0x4000 && addr < 0x6000 ) {
                /*printf("Tried to write to 0x%04x\n", addr);*/
                upper = (data & 0x03 ) << 5;
                return;
            } else if ( addr >= 0x6000 && addr < 0x8000 ) {
                /*printf("Tried to write to 0x%04x\n", addr);*/
                ram_select = data & 0x01;
                return;
            }
            break;
        default:
            break;
    }

    switch ( addr ) {
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
            /*printf("MEU: Write lcd control %2x\n", data);*/
            write_lcd_control ( cpu, data );
            break;
        case 0xff41: // lcd stat
            /*printf("MEU: Write stat %2x\n", data);*/
            display_write_stat ( cpu, data );
            break;
        case 0xff42:
            write_scroll_y ( cpu, data );
            break;
        case 0xff43:
            write_scroll_x ( cpu, data );
            break;
        case 0xff44:
            // do nothing
            break;
        case 0xff45:
            // do nothing
            break;
        case 0xff46:
            /*printf("Requested OAM DMA\n");*/
            memcpy(&cpu->memory[0xfe00], &cpu->memory[data*0x100], 0xa0);
            cpu->DMA_in_progress = cpu->cycles_machine;
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
            break;
    }

    cpu->memory [ addr ] = data;
}

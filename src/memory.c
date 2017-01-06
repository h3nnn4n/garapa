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

void dma_step ( _cpu_info *cpu ) {
    if ( cpu->dma.oam_dma_timer > 0 ) {
        uint16_t src = cpu->dma.oam_dma_source + cpu->dma.oam_dma_index;
        uint8_t data = read_byte ( cpu, src );
        // info!("oam/transfer [{:X}] {:X} -> [{:X}]",
        //       src,
        //       r,
        //       0xFE00 + self.oam_dma_index);
        /*self.gpu.oam[self.oam_dma_index as usize] = r;*/
        cpu->mem_controller.memory [ 0xfe00 + cpu->dma.oam_dma_index ] = data;

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
    if ( addr >= 0xa000 && addr < 0xbfff ) {
        if ( cpu->mem_controller.ram_enable && cpu->mem_controller.ram_size ) {
            uint16_t offset = addr - 0xa000;
            if ( cpu->mem_controller.ram_size == 0x01 ) {
                if ( addr > 0xa7ff ) return 0xff;
            } else {
                return cpu->mem_controller.cartridge_ram[ offset ];
            }

            if ( cpu->mem_controller.ram_size == 0x02 ) {
                /*printf("Reading from %04x\n", addr);*/
                return cpu->mem_controller.cartridge_ram[ offset ];
            }

            return cpu->mem_controller.cartridge_ram[ offset + cpu->mem_controller.ram_bank_number * 0x2000 ];
        } else {
            return 0xff;
        }
    }

    if ( ( ( addr >= 0xFE00 ) && ( addr <= 0xFE9F ) ) &&
         ( cpu->dma.oam_dma_timer > 0 || cpu->lcd.mode == 3 )) {
        return 0xff;
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
            return display_read_LYC ( cpu );
        case 0xff4a:
            return read_window_y ( cpu );
        case 0xff4b:
            return read_window_x ( cpu );
        default:
            break;
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
    uint8_t mapper = cpu->mem_controller.memory[0x0147];

    switch (mapper) {
        case 0x00:
            if ( addr < 0x8000 ) {
                /*printf("Tried to write to 0x%04x\n", addr);*/
                return;
            }
            break;
        case 0x03:
        case 0x02:
        case 0x01:
            if ( addr < 0x2000 ) { // Ram Enable
                cpu->mem_controller.ram_enable = data & 0x0a ? 1 : 0;
                /*printf(" RAM write : %c\n", cpu->mem_controller.ram_enable ? 'y' : 'n' );*/
                return;
            } else if ( addr >= 0x2000 && addr < 0x4000 ) { // Rom bank switch
                cpu->mem_controller.rom_bank_number = data & 0x1f;
                if ( !cpu->mem_controller.ram_mode )
                    cpu->mem_controller.rom_bank_number |= cpu->mem_controller.ram_bank_number;

                if ( cpu->mem_controller.rom_bank_number == 0x00 ||
                     cpu->mem_controller.rom_bank_number == 0x20 ||
                     cpu->mem_controller.rom_bank_number == 0x40 ||
                     cpu->mem_controller.rom_bank_number == 0x60 ) {
                    cpu->mem_controller.rom_bank_number++;
                }

                /*printf(" ROM bank: %2x\n", cpu->mem_controller.rom_bank_number );*/

                memcpy(&cpu->mem_controller.memory[0x4000],
                       &cpu->mem_controller.rom[cpu->mem_controller.rom_bank_number * 0x4000],
                       0x4000);
                return;
            } else if ( addr >= 0x4000 && addr < 0x6000 ) {
                cpu->mem_controller.ram_bank_number = ( data & 0x03 ) << 5;
                /*printf(" RAM bank: %c\n", cpu->mem_controller.ram_bank_number );*/
                return;
            } else if ( addr >= 0x6000 && addr < 0x8000 ) { // Ram Ena
                cpu->mem_controller.ram_mode = data & 0x01;
                /*printf(" RAM enable: %c\n", cpu->mem_controller.ram_mode ? '0' : '1' );*/
                return;
            }
            break;
        default:
            assert ( 0 && "Unsupported MBC " );
            break;
    }

    if ( addr >= 0xa000 && addr < 0xbfff ) {
        if ( cpu->mem_controller.ram_enable && cpu->mem_controller.ram_size ) {
            uint16_t offset = addr - 0xa000;
            if ( cpu->mem_controller.ram_size == 0x01 ) {
                if ( addr > 0xa7ff ) return;
            } else {
               cpu->mem_controller.cartridge_ram[ offset ] = data;
               return;
            }

            if ( cpu->mem_controller.ram_size == 0x02 ) {
                /*printf("Writing to %04x\n", addr);*/
                cpu->mem_controller.cartridge_ram[ offset ] = data;
                return;
            }

            cpu->mem_controller.cartridge_ram[ offset + cpu->mem_controller.ram_bank_number * 0x2000 ] = data;
        } else {
            return;
        }
    }

    if ( ( ( addr >= 0xFE00 ) && ( addr <= 0xFE9F ) ) &&
         ( cpu->dma.oam_dma_timer > 0 )) {
         /*( cpu->dma.oam_dma_timer > 0 || cpu->lcd.mode >= 2 )) {*/
        /*printf("Write blocked: DMA_timer: %3d  LCD_mode: %2d\n", cpu->dma.oam_dma_timer, cpu->lcd.mode);*/
        return;
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

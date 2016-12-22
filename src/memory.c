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

    assert( offset + buffer_size <= 64 * 1024 && "ROM too big for the 64k bytes RAM");
    if ( fread(cpu->memory + offset, buffer_size, 1, f) != 1 ) {
        printf("Something went weird while reding into buffer\n");
    }

    fclose(f);

    /*fprintf(stderr, "Loaded %s into %04x to %04x\n", fname, offset, offset +(uint16_t) buffer_size);*/
}

uint8_t read_byte ( _cpu_info *cpu, uint16_t addr ) {
    switch ( addr ) {
        case 0xff00:
            return 0xcf;
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
    switch ( addr ) {
        case 0xff00:
            break;
        case 0xff01: // Serial OUT
            /*fprintf(stderr, "%c", data);*/
            check_passed(data);
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
        case 0xff44: // LYC
            // do nothing
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

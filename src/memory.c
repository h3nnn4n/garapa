#include <stdio.h>
#include <assert.h>

#include "memory.h"
#include "utils.h"
#include "types.h"

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

    fprintf(stderr, "Loaded %s into %04x to %04x\n", fname, offset, offset +(uint16_t) buffer_size);
}

uint8_t read_byte ( _cpu_info *cpu, uint16_t addr ) {
    // No need to check if the address is valid.
    // uint16_t can only hold enough to access the
    // 64kb memory space;
    return cpu->memory [ addr ];
}

uint16_t read_word ( _cpu_info *cpu, uint16_t addr ) {
    return read_byte ( cpu, addr + 0 ) << 8 |
           read_byte ( cpu, addr + 1 ) ;
}

uint16_t read_next_word ( _cpu_info *cpu ) {
    return read_word ( cpu, cpu->pc + 1 );
}

void write_byte ( _cpu_info *cpu, uint16_t addr, uint8_t data ) {
    if ( addr == 0xff01 ) {
        fprintf(stderr, "%c", data);
    } else
        cpu->memory [ addr ] = data;
}

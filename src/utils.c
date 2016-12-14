#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "types.h"
#include "disassembler.h"

off_t fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    fprintf(stderr, "Cannot determine size of %s: %s\n", filename, strerror(errno));

    return -1;
}

void init_cpu( _cpu_info *cpu ) {
    cpu->memory = calloc ( 1, 64 * 1024 ) ; // Allocs 64Kb of ram
    cpu->cycles = 0;
    cpu->pc     = 0;
    cpu->a      = 0;
    cpu->b      = 0;
    cpu->c      = 0;
    cpu->d      = 0;
    cpu->e      = 0;
    cpu->h      = 0;
    cpu->l      = 0;

    cpu->flags.z   = 0;
    cpu->flags.s   = 0;
    cpu->flags.p   = 0;
    cpu->flags.cy  = 0;
    cpu->flags.ac  = 0;

    cpu->enable_interrupts     = 0;
    cpu->instructions_executed = 0;

    cpu->interrupt_addr = 0x10;
    cpu->interrupt_addr = 0x08;
    cpu->shift_offset   = 0;
    cpu->shift0         = 0;
    cpu->shift1         = 0;

    cpu->portin0 = 0x0e;
    cpu->portin1 = 0x08;
    cpu->portin2 = 0x00;
}

void unimplemented_opcode( _cpu_info *cpu ) {
    disassembler ( cpu->memory, cpu->pc );
    exit(-1);
}

int parity_bit ( int b ) {
    int bits = 0;
    int i    = 0;

    for (i = 0; i < 8; ++i)
        if ( b & ( 1 << i ) )
            bits += 1;

    return bits & 1 ? 0 : 1;
}

void print_registers ( _cpu_info *cpu ) {
    uint8_t f = ( (cpu->flags.z ) ? 0x40 : 0x00 ) |
                ( (cpu->flags.s ) ? 0x80 : 0x00 ) |
                ( (cpu->flags.p ) ? 0x04 : 0x00 ) |
                ( (cpu->flags.cy) ? 0x01 : 0x00 ) |
                ( (cpu->flags.ac) ? 0x10 : 0x00 ) |
                0x02;

    printf(" AF: %02x%02x BC: %02x%02x DE: %02x%02x HL: %02x%02x PC: %04x SP: %04x",
            cpu->a, f, cpu->b, cpu->c, cpu->d, cpu->e, cpu->h, cpu->l, cpu->pc, cpu->sp);
    printf(" F: %c%c%c%c%c CYCLES: %16llu IPS: %16llu\n",
            cpu->flags.z  ? 'z' : '.',
            cpu->flags.s  ? 's' : '.',
            cpu->flags.p  ? 'p' : '.',
            cpu->flags.cy ? 'c' : '.',
            cpu->flags.ac ? 'a' : '.',
            cpu->cycles              ,
            cpu->instructions_executed);
    /*printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(f));*/
}

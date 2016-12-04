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
    cpu->memory = malloc ( 64 * 1024 ) ; // Allocs 64Kb of ram
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

    cpu->enable_interrupts = 0;
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

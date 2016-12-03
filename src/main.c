#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>

#include "utils.h"
#include "types.h"

int main(int argc, char *argv[]) {
    FILE *f = NULL;
    _cpu_info cpu;

    if ( argc == 1 ) {
        printf("Usage: %s file\n", argv[0]);
        return EXIT_FAILURE;
    } else {
        init_cpu(&cpu);
        off_t buffer_size = -1;
        f = fopen(argv[1], "rb");
        buffer_size = fsize( argv[1] );
        /*buffer = malloc( buffer_size );*/

        if ( fread(cpu.memory, buffer_size, 1, f) != 1 ) {
            printf("Something went weird while reding into buffer\n");
        }

        printf("Opening: %s\nFile contains: %lu bytes\n", argv[1], buffer_size);
    }

    while ( cpu.pc < 100 ) {
        cpu.pc += disassembler(cpu.memory, cpu.pc);

        if ( cpu.pc > 100 ) break;
    }

    return 0;
}

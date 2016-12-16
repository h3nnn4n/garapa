#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#include <sys/types.h>

#include "utils.h"
#include "types.h"
#include "emulator.h"
#include "graphics.h"
#include "disassembler.h"
#include "time_keeper.h"

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

        uint16_t offset = 0x000;

        assert( offset + buffer_size <= 64 * 1024 && "ROM too big for the 64k bytes RAM");
        if ( fread(cpu.memory + offset, buffer_size, 1, f) != 1 ) {
            printf("Something went weird while reding into buffer\n");
        }

        fclose(f);

        printf("Opening: %s\nFile contains: %lu bytes\n", argv[1], buffer_size);
    }

    sdl_init();
    atexit(SDL_Quit);

    while ( 1 ) {
        emulator ( &cpu );
    }

    return 0;
}

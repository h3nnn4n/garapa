#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#include <sys/types.h>

#include "utils.h"
#include "types.h"
#include "memory.h"
#include "cartridge.h"
#include "decoder.h"
#include "graphics.h"
#include "display.h"
#include "disassembler.h"
#include "time_keeper.h"

int main(int argc, char *argv[]) {
    _cpu_info cpu;

    if ( argc == 1 ) {
        printf("Usage: %s file\n", argv[0]);
        return EXIT_FAILURE;
    }

    sdl_init();
    atexit(sdl_quit);

    init_cpu(&cpu);

    load_rom ( &cpu, argv[1], 0x0000 );

    print_rom_info(&cpu);

    while ( 1 ) {
        decoder        ( &cpu );
        /*display_update ( &cpu );*/ // Those will go inside the opcodes
        /*timer_update   ( &cpu );*/
        /*input_update   ( &cpu );*/
    }

    return 0;
}

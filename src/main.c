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
#include "automated_tests.h"

int main(int argc, char *argv[]) {
    _cpu_info cpu;
    sdl_init();
    atexit(sdl_quit);

    if ( argc == 1 ) {
        test_control.test_enable = 1;
        test_run ();
        return 0;
    }

    test_control.test_enable = 0;

    init_cpu(&cpu);

    load_rom ( &cpu, argv[1], 0x0000 );

    print_rom_info(&cpu);

    while ( 1 ) {
        decoder        ( &cpu );
    }

    return 0;
}

/******************************************************************************
 * Copyright (C) 2016  Renan S. Silva                                         *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgment in the product documentation would be   *
 *    appreciated but is not required.                                        *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#include <sys/types.h>

#include "rev.h"
#include "utils.h"
#include "types.h"
#include "memory.h"
#include "cartridge.h"
#include "other_window.h"
#include "decoder.h"
#include "graphics.h"
#include "display.h"
#include "disassembler.h"
#include "time_keeper.h"
#include "file_control.h"
#include "automated_tests.h"

int main(int argc, char *argv[]) {
    _cpu_info cpu;
    sdl_init();

    srand48(mix(clock(), time(NULL), getpid()));
    srand(mix(clock(), time(NULL), getpid()));

    other_window_init();
    atexit(sdl_quit);

    if ( argc == 1 ) {
        test_control.test_enable = 1;
        test_run ();
        return 0;
    }

    test_control.test_enable = 0;

    init_cpu(&cpu);
    init_file_control();

    load_rom ( &cpu, argv[1], 0x0000 );

    /*print_rom_info(&cpu);*/

    reset_code_and_data();

    set_cpu_pointer( &cpu );

    while ( 1 ) {
        decoder        ( &cpu );
    }

    return 0;
}

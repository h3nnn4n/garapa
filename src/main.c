/******************************************************************************
 * Copyright (C) 2016-2020  Renan S. Silva                                    *
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

#define PY_SSIZE_T_CLEAN

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>

#include "audio.h"
#include "automated_tests.h"
#include "cartridge.h"
#include "decoder.h"
#include "disassembler.h"
#include "display.h"
#include "graphics.h"
#include "memory.h"
#include "python_api.h"
#include "time_keeper.h"
#include "types.h"
#include "utils.h"

int main(int argc, char **argv) {
    _cpu_info cpu;
    py_init(argc, argv);

    atexit(graphics_exit);

    if (graphics_init())
        exit(-1);

    if (argc == 1) {
        test_control.test_enable = 1;
        test_run();
        return 0;
    }

    test_control.test_enable = 0;

    init_cpu(&cpu);

    apu_sdl_init(&cpu);

    load_rom(&cpu, argv[1], 0x0000);

    /*print_rom_info(&cpu);*/

    while (1) {
        decoder(&cpu);
    }

    return 0;
}

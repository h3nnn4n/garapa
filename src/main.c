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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "automated_tests.h"
#include "config.h"
#include "core.h"
#include "python_api.h"
#include "types.h"

int main(int argc, char **argv) {
    int            c;
    static _config config;
    memset(&config, 0, sizeof(config));

    while (1) {
        static struct option long_options[] = {{"enable-video", no_argument, &config.enable_video, 1},
                                               {"disable-video", no_argument, &config.enable_video, 0},
                                               {"enable-audio", no_argument, &config.enable_audio, 1},
                                               {"disable-audio", no_argument, &config.enable_audio, 0},
                                               {"run-test-roms", no_argument, &config.run_test_roms, 1},
                                               {"rom", required_argument, 0, 'r'},
                                               {"python", required_argument, 0, 'p'},
                                               {0, 0, 0, 0}};

        int option_index = 0;

        c = getopt_long(argc, argv, "abc:d:f:", long_options, &option_index);

        if (c == -1)
            break;

        switch (c) {
            case 0:
                if (long_options[option_index].flag != 0)
                    break;

                printf("option %s", long_options[option_index].name);
                if (optarg)
                    printf(" with arg %s", optarg);

                printf("\n");
                break;

            case 'r': {
                char *rom_name = malloc(sizeof(char) * (strlen(optarg) + 2));
                memcpy(rom_name, optarg, sizeof(char) * (strlen(optarg) + 1));
                config.rom_name = rom_name;
            } break;

            case 'p': {
                char *python_filename = malloc(sizeof(char) * (strlen(optarg) + 2));
                memcpy(python_filename, optarg, sizeof(char) * (strlen(optarg) + 1));
                config.python_filename = python_filename;
            } break;

            case '?':
                /* getopt_long already printed an error message. */
                break;

            default: abort();
        }
    }

    set_config(&config);

    if (get_config_flag("run_test_roms")) {
        test_control.test_enable = 1;
        test_run();
    } else if (get_config_value_s("rom_name") != NULL) {
        _context *context = build_emulation_context();

        if (config.python_filename != NULL)
            py_init(argc, argv);

        emulation_loop(context);
    }

    return 0;
}

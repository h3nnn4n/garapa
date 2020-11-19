#include "core.h"
#include "automated_tests.h"
#include "cartridge.h"
#include "decoder.h"
#include "graphics.h"
#include "python_api.h"
#include "types.h"
#include "utils.h"

_context *build_emulation_context(int argc, char **argv) {
    if (graphics_init())
        exit(-1);

    test_control.test_enable = 0;
    if (argc == 1) {
        test_control.test_enable = 1;
        return NULL;
    }

    _context *context = malloc(sizeof(_context));
    context->cpu_info = malloc(sizeof(_cpu_info));

    init_cpu(context->cpu_info);
    load_rom(context->cpu_info, argv[1], 0x0000);

    set_current_context(context);

    py_init(argc, argv);

    atexit(graphics_exit);

    return context;
}

void emulation_loop(_context *context) {
    while (1) {
        decoder(context->cpu_info);
    }
}

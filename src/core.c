#include "core.h"
#include "types.h"
#include "python_api.h"
#include "decoder.h"
#include "cartridge.h"
#include "graphics.h"
#include "utils.h"
#include "automated_tests.h"

_context* build_emulation_context(int argc, char **argv) {
    test_control.test_enable = 0;
    if ( argc == 1 ) {
        test_control.test_enable = 1;
        return NULL;
    }

    _context *context = malloc(sizeof(_context));
    context->cpu_info = malloc(sizeof(_cpu_info));

    py_init(argc, argv);

    if (graphics_init()) exit(-1);
    atexit(graphics_exit);

    init_cpu(context->cpu_info);
    load_rom(context->cpu_info, argv[1], 0x0000);

    return context;
}

void emulation_loop(_context *context) {
    while ( 1 ) {
        decoder        ( context->cpu_info );
    }
}

#include "core.h"
#include "audio.h"
#include "automated_tests.h"
#include "cartridge.h"
#include "config.h"
#include "decoder.h"
#include "graphics.h"
#include "python_api.h"
#include "types.h"
#include "utils.h"

_context *build_emulation_context() {
    if (graphics_init())
        exit(-1);

    _context *context = malloc(sizeof(_context));
    context->cpu_info = malloc(sizeof(_cpu_info));

    init_cpu(context->cpu_info);
    load_rom(context->cpu_info, get_config_value_s("rom_name"), 0x0000);
    apu_sdl_init(context->cpu_info);

    set_current_context(context);

    atexit(graphics_exit);

    return context;
}

void emulation_loop(_context *context) {
    while (1) {
        decoder(context->cpu_info);
    }
}

#include <sys/types.h>
#include <julia.h>

#include "julia_api.h"
#include "types.h"
#include "memory.h"

_cpu_info *cpu_ptr;

void garapa_jl_quit_on_error(){
    if (jl_exception_occurred()) {
        jl_call2(jl_get_function(jl_base_module, "show"), jl_stderr_obj(), jl_exception_occurred());
        jl_printf(jl_stderr_stream(), "\n");

        exit(EXIT_FAILURE);
    }
}

void garapa_jl_init() {
    jl_init();

    jl_load("japi.jl");

    jl_function_t *hello = jl_get_function(jl_current_module, "garapa_hello");
    jl_call0(hello);

    garapa_jl_quit_on_error();
}

void garapa_jl_load_file(char *file){
    printf("Loading %s\n", file);
    jl_load(file);

    jl_function_t *cpu_loop = jl_get_function(jl_current_module, "garapa_init");

    jl_call0(cpu_loop);

    garapa_jl_quit_on_error();
}

void garapa_jl_exit() {
    jl_atexit_hook(EXIT_SUCCESS);
}

void garapa_jl_display_update() {
    static jl_function_t *display_update = NULL;

    if ( display_update == NULL ) {
        display_update = jl_get_function(jl_current_module, "garapa_display_update");
    }

    jl_call0(display_update);

    garapa_jl_quit_on_error();
}

void garapa_jl_set_cpu_pointer( _cpu_info *cpu ) {
    cpu_ptr = cpu;
}

_cpu_info* garapa_jl_get_cpu_pointer(){
    return cpu_ptr;
}

uint16_t garapa_jl_read_pc() {
    return garapa_jl_get_cpu_pointer()->pc;
}

uint8_t garapa_jl_read_byte(uint16_t addr) {
    return _read_byte( garapa_jl_get_cpu_pointer(), addr );
}

void garapa_jl_write_byte(uint16_t addr, uint8_t data) {
    printf("%04x %02x\n", addr, data);
    write_byte( garapa_jl_get_cpu_pointer(), addr, data );
}

void garapa_jl_cpu_loop_call() {
    static jl_function_t *cpu_loop = NULL;

    if ( cpu_loop == NULL ) {
        cpu_loop = jl_get_function(jl_current_module, "garapa_cpu_loop");
    }

    jl_call0(cpu_loop);

    garapa_jl_quit_on_error();
}

void garapa_jl_draw() {
    static jl_function_t *drawer = NULL;

    if ( drawer == NULL ) {
        drawer = jl_get_function(jl_current_module, "drawer");
    }

    jl_call0(drawer);

    garapa_jl_quit_on_error();
}

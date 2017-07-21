#ifndef JULIA_API_H
#define JULIA_API_H

#include <julia.h>

#include "types.h"

void garapa_jl_init();
void garapa_jl_exit();
void garapa_jl_quit_on_error();
void garapa_jl_cpu_loop_call();
void garapa_jl_display_update();
void garapa_jl_set_cpu_pointer( _cpu_info *cpu );
void garapa_jl_load_file(char *file);
void garapa_jl_draw();

void garapa_jl_write_byte(uint16_t addr, uint8_t data);

uint8_t garapa_jl_read_byte(uint16_t addr);
uint16_t garapa_jl_read_pc();

_cpu_info* garapa_jl_get_cpu_pointer();

#endif /* JULIA_API_H */

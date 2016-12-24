#ifndef GRAPHICS_meu_H
#define GRAPHICS_meu_H

#define SDL_MAIN_HANDLED

#include "types.h"

uint32_t *get_frame_buffer (                ) ;
void     flip_screen      (                ) ;
void     sdl_init         (                ) ;
void     input_update     ( _cpu_info *cpu ) ;
void     sdl_quit         (                ) ;

#endif /* GRAPHICS_meu_H */

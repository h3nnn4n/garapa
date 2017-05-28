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
#ifndef GRAPHICS_meu_H
#define GRAPHICS_meu_H

#define SDL_MAIN_HANDLED

#include "types.h"

uint32_t *get_frame_buffer (                ) ;
void     flip_screen       ( _cpu_info *cpu ) ;
void     sdl_init          (                ) ;
void     input_update      ( _cpu_info *cpu ) ;
void     sdl_quit          (                ) ;

void draw_rectangle(int x, int y, int x2, int y2, int r, int g, int b);
void draw_text(char *text, int x, int y, int r, int g, int b);
void draw_text_with_bg(char *text, int x, int y, int r, int g, int b);

#endif /* GRAPHICS_meu_H */

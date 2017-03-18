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
#ifndef OTHER_WINDOW_H
#define OTHER_WINDOW_H

#include "types.h"

#define MAX_SPRITE 1000
#define __X_SIZE 10
#define __Y_SIZE 22

typedef struct {
    int posx;
    int posy;
    int id;
} _sprite_list;

typedef struct {
    _sprite_list sprite_list[MAX_SPRITE];
    int used_sprites;
} _sprite_t_info;

typedef struct {
    int data[__X_SIZE][__Y_SIZE];
} _bg_info;

void sprite_info_add(int posx, int posy, int id);
void sprite_info_reset();
_bg_info* get_bg_info_pointer();

void draw_text(char *text, int x, int y, int r, int g, int b) ;
void draw_rectangle(int x, int y, int r, int g, int b) ;

void set_cpu_pointer(_cpu_info *cpu);

void other_window_init ( ) ;
void other_sdl_quit ( ) ;
void other_flip_screen ( ) ;
uint32_t *other_get_frame_buffer () ;
uint32_t *other_get_frame_buffer_vision () ;

#endif /* OTHER_WINDOW_H */

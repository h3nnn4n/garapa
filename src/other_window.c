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
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_video.h"
#include "SDL_render.h"

#include "other_window.h"

#include "types.h"
#include "graphics.h"

#include "tetris.h"

/*#define __use_other_sdl*/

/*#ifdef __use_other_sdl*/

static SDL_Window   *other_window;
static SDL_Renderer *other_renderer;
static SDL_Texture  *other_bitmap;
static uint32_t *other_pixels;

static int other_screenx = 160 * 4;
static int other_screeny = 144 * 4;

_sprite_t_info sprite_t_info;
_bg_info bg_info;

TTF_Font* font;

/*#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"*/
/*#define BYTE_TO_BINARY(byte)  \*/
      /*(byte & 0x80 ? '1' : '0'), \*/
  /*(byte & 0x40 ? '1' : '0'), \*/
  /*(byte & 0x20 ? '1' : '0'), \*/
  /*(byte & 0x10 ? '1' : '0'), \*/
  /*(byte & 0x08 ? '1' : '0'), \*/
  /*(byte & 0x04 ? '1' : '0'), \*/
  /*(byte & 0x02 ? '1' : '0'), \*/
  /*(byte & 0x01 ? '1' : '0')*/

uint32_t *other_get_frame_buffer () {
    return other_pixels;
}

void sprite_info_reset() {
    sprite_t_info.used_sprites = 0;
}

void sprite_info_add(int posx, int posy, int id) {
    sprite_t_info.sprite_list[sprite_t_info.used_sprites].posx = posx;
    sprite_t_info.sprite_list[sprite_t_info.used_sprites].posy = posy;
    sprite_t_info.sprite_list[sprite_t_info.used_sprites].id   = id;

    sprite_t_info.used_sprites++;
}

void other_window_init ( ) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    other_window = SDL_CreateWindow("The Other Window",
        0,
        SDL_WINDOWPOS_CENTERED,
        other_screenx,
        other_screeny,
        0);

    other_renderer = SDL_CreateRenderer(other_window, -1, 0);
    other_bitmap   = SDL_CreateTexture (other_renderer,
               SDL_PIXELFORMAT_RGB888,
               SDL_TEXTUREACCESS_STREAMING,
               other_screenx,
               other_screeny);
    other_pixels   = malloc ( sizeof ( uint32_t ) * other_screenx * other_screeny);
    SDL_RenderSetScale(other_renderer, 1, 1);

    memset(other_pixels, 255, other_screenx * other_screeny * sizeof(uint32_t));

    SDL_UpdateTexture(other_bitmap, NULL, other_pixels, other_screenx * sizeof(uint32_t));

    font = TTF_OpenFont("inconsolata.ttf", 18);
}

_bg_info* get_bg_info_pointer () {
    return &bg_info;
}

void draw_rectangle(int x, int y, int r, int g, int b) {
    int size = 1;
    int x2 = x / size;
    int y2 = y / size;
    int w2 = 8 / size;
    SDL_SetRenderDrawColor(other_renderer, r, g, b, 0);
    SDL_Rect dstrect = { x2, y2, w2, w2 };
    SDL_RenderFillRect(other_renderer, &dstrect);
}

void draw_falling_blocks() {
    for (int i = 0; i < sprite_t_info.used_sprites; ++i) {
        /*printf(" %d %d %d\n", i, sprite_t_info.sprite_list[i].posx, sprite_t_info.sprite_list[i].posy);*/
        if ( sprite_t_info.sprite_list[i].posx >= 16 && sprite_t_info.sprite_list[i].posx <= 88 ) {
            draw_rectangle(sprite_t_info.sprite_list[i].posx, sprite_t_info.sprite_list[i].posy, 0, 0, 0);
        }
    }

    /*printf("\n");*/
}

void draw_bg() {
    for (int i = 16; i <= 88; i += 8) {
        for (int j = 8; j <= 136; j += 8) {
            assert(i/8 * 10 + j/8 <= 10*22);
            if ( bg_info.data[i/8 - 2][j/8 - 1] ) {
                draw_rectangle(i, j, 255, 0, 0);
            }
        }
    }
}

void draw_text(char *text, int x, int y, int r, int g, int b) {
    int texW = 0;
    int texH = 0;

    SDL_Color color = { r, g, b, 0 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(other_renderer, surface);

    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { x, y, texW, texH };

    SDL_RenderCopy(other_renderer, texture, NULL, &dstrect);
}

void evaluate_cost() {
    char text[256];
    int aggregate_height_cost = aggregate_height();

    sprintf(text, "aggregate height: %d", aggregate_height_cost);
    draw_text(text, 100, 0, 255, 0, 0);

    int complete_rows_cost = complete_rows();

    sprintf(text, "complete_rows: %d", complete_rows_cost);
    draw_text(text, 100, 20, 255, 0, 0);
}

void other_flip_screen ( ) {
    SDL_RenderClear(other_renderer);
    SDL_RenderCopy(other_renderer, other_bitmap, NULL, NULL);

    draw_bg();
    draw_falling_blocks();

    evaluate_cost();

    SDL_UpdateTexture(other_bitmap, NULL, other_pixels, other_screenx * sizeof(uint32_t));

    SDL_RenderPresent(other_renderer);
}

void other_sdl_quit ( ) {
    SDL_Quit();
}

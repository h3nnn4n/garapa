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
#include "trainer.h"
#include "lelmark.h"
#include "graphics.h"
#include "file_control.h"

#include "tetris.h"


#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
      (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

#define __use_other_sdl
#define __draw_other_window

static _cpu_info* cpu_info;

#if defined(__draw_other_window) && defined(__use_other_sdl)

static SDL_Window   *other_window;
static SDL_Renderer *other_renderer;
static SDL_Texture  *other_bitmap;
static uint32_t *other_pixels;

TTF_Font* font;

static int other_screenx = 160 * 4;
static int other_screeny = 144 * 4;

#endif

/*static _point best;*/
static _best_piece best_piece;

static _move_queue move_queue;

static _ai_state ai_state;

_sprite_t_info sprite_t_info;
_bg_info bg_info;

void set_cpu_pointer(_cpu_info *cpu) {
    cpu_info = cpu;
}

int piece_moved () {
    static int x = -1;
    static int y = -1;

    int ret = 0;

    int x2 = get_cpu_pointer()->mem_controller.memory[0xff92] - 8;
    int y2 = get_cpu_pointer()->mem_controller.memory[0xff93] - 16;

    if ( x != x2 || y != y2 ) {
        /*printf("Piece moved\n");*/
        ret = 1;
    }

    x = x2;
    y = y2;

    return ret;
}

void rotation_changed_hook () {
    static int rotation = -1; //get_cpu_pointer()->mem_controller.memory[0xc203];

    if ( rotation != get_cpu_pointer()->mem_controller.memory[0xc203] ) {
        if ( piece_moved () ) {
            move_queue.wait_rotation = 0;
            /*printf("rotation hook\n");*/
            best_piece.nrotations--;
        }
    }

    rotation = get_cpu_pointer()->mem_controller.memory[0xc203];
}

void joystick_hook () {
    int x = get_cpu_pointer()->mem_controller.memory[0xff92] - 8;
    /*int y = get_cpu_pointer()->mem_controller.memory[0xff93] - 16;*/
    _cpu_info *cpu = get_cpu_pointer();

    /*rotation_changed_hook();*/

    /*printf("Joystick hook\n");*/

    start_game_hook();

    if ( cpu->pc == 0x2a0e ) {
        switch ( ai_state.game_state ) {
            case INGAME:
                switch ( move_queue.ready ) {
                    case 0:
                        cpu->joystick.button_start  = 1;
                        cpu->joystick.button_select = 1;
                        cpu->joystick.button_b      = 1;
                        cpu->joystick.button_a      = 1;
                        cpu->joystick.button_down   = 1;
                        cpu->joystick.button_up     = 1;
                        cpu->joystick.button_left   = 1;
                        cpu->joystick.button_right  = 1;

                        move_queue.ready = 1;
                        break;
                    case 1:
                        move_queue.ready = 0;

                        if ( best_piece.nrotations > 0 /*&& move_queue.wait_rotation == 0*/ ) {
                            /*printf("Called rotation %2d\n", best_piece.nrotations);*/
                            cpu->joystick.button_b   = 0;
                            move_queue.wait_rotation = 1;
                            best_piece.nrotations--;
                        } else
                        if ( best_piece.coord.x > x ) {
                            cpu->joystick.button_right = 0;
                        } else
                        if ( best_piece.coord.x < x ) {
                            cpu->joystick.button_left  = 0;
                        } else
                        if ( best_piece.coord.x == x ) {
                            cpu->joystick.button_down  = 0;
                        }
                        break;
                    case 2:
                        break;
                    default:
                        fprintf(stderr, "Invalid ready status at joystick_hook\n");
                        abort();
                }
                break;
            case BOOTING:
            case GAMEOVER:
                /*printf("BOOT/GAMEOVER button\n");*/
                switch ( move_queue.ready ) {
                    case 0:
                        cpu->joystick.button_start  = 1;
                        cpu->joystick.button_select = 1;
                        cpu->joystick.button_b      = 1;
                        cpu->joystick.button_a      = 1;
                        cpu->joystick.button_down   = 1;
                        cpu->joystick.button_up     = 1;
                        cpu->joystick.button_left   = 1;
                        cpu->joystick.button_right  = 1;

                        move_queue.ready = 1;
                        break;
                    case 1:
                        if ( get_brain_pointer()->rng && drand48 () < .25 ) {
                            move_queue.ready = 0;

                            cpu->joystick.button_start = 0;
                            break;
                        } else if ( get_brain_pointer()->rng == 0 ) {
                            move_queue.ready = 0;

                            cpu->joystick.button_start = 0;
                            break;
                        }
                    case 2:
                        break;
                    default:
                        fprintf(stderr, "Invalid ready status at joystick_hook\n");
                        abort();
                }
                break;
            default:
                break;
        }
    }
}

/*_obj_costs* get_obj_cost_pointer() {*/
    /*return &obj_cost;*/
/*}*/

_cpu_info *get_cpu_pointer() {
    return cpu_info;
}

_best_piece *get_best_piece_pointer() {
    return &best_piece;
}

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
#ifdef __draw_other_window
    return other_pixels;
#else
    return NULL;
#endif
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
#ifdef __draw_other_window
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
#endif

    move_queue.ready         = 0;
    move_queue.wait_rotation = 0;
    ai_state.game_state      = BOOTING;

    boot_brain();
}

_bg_info* get_bg_info_pointer () {
    return &bg_info;
}
void draw_rectangle(int x, int y, int x2, int y2, int r, int g, int b) {
#ifdef __draw_other_window
    SDL_SetRenderDrawColor(other_renderer, r, g, b, 0);
    SDL_Rect dstrect = { x, y, x2, y2 };
    SDL_RenderFillRect(other_renderer, &dstrect);
#endif
}

void draw_square(int x, int y, int r, int g, int b) {
#ifdef __draw_other_window
    int size = 1;
    int x2 = x / size;
    int y2 = y / size;
    int w2 = 8 / size;
    SDL_SetRenderDrawColor(other_renderer, r, g, b, 0);
    SDL_Rect dstrect = { x2, y2, w2, w2 };
    SDL_RenderFillRect(other_renderer, &dstrect);
#endif
}

void draw_falling_blocks() {
    for (int i = 0; i < sprite_t_info.used_sprites; ++i) {
        /*printf(" %d %d %d\n", i, sprite_t_info.sprite_list[i].posx, sprite_t_info.sprite_list[i].posy);*/
        int x = sprite_t_info.sprite_list[i].posx;
        int y = sprite_t_info.sprite_list[i].posy;

        if (  x >= 16 && x <= 88 ) {
            /*int x2 = get_cpu_pointer()->mem_controller.memory[0xff92] - 8;*/
            /*int y2 = get_cpu_pointer()->mem_controller.memory[0xff93] - 16;*/
            /*printf("x,y : %3d %3d\n", (x - x2) / 8, (y - y2) / 8);*/
            draw_square(x, y, 0, 0, 0);
        }
    }

    int x = get_cpu_pointer()->mem_controller.memory[0xff92] - 8;
    int y = get_cpu_pointer()->mem_controller.memory[0xff93] - 16;

    draw_square(x, y, 0, 127, 127);

    /*printf("\n");*/
}

void draw_bg() {
    draw_rectangle(16, 8, 80, 136, 0x80, 0x80, 0x80);
    for (int i = 16; i <= 88; i += 8) {
        for (int j = 8; j <= 136; j += 8) {
            assert(i/8 * __X_SIZE + j/8 <= __X_SIZE * __Y_SIZE);
            if ( bg_info.data[i/8 - 2][j/8 - 1] == 1 ) {
                draw_square(i, j, 255, 0, 0);
            }
            if ( bg_info.data[i/8 - 2][j/8 - 1] > 1 ) {
                draw_square(i, j, 0, 255, 0);
            }
        }
    }
}

void draw_best() {
    draw_square(best_piece.coord.x + best_piece.blocks.a.x * 8 , best_piece.coord.y + best_piece.blocks.a.y * 8, 60 , 100, 255 );
    draw_square(best_piece.coord.x + best_piece.blocks.b.x * 8 , best_piece.coord.y + best_piece.blocks.b.y * 8, 60 , 100, 255 );
    draw_square(best_piece.coord.x + best_piece.blocks.c.x * 8 , best_piece.coord.y + best_piece.blocks.c.y * 8, 60 , 100, 255 );
    draw_square(best_piece.coord.x + best_piece.blocks.d.x * 8 , best_piece.coord.y + best_piece.blocks.d.y * 8,  0 ,  0, 255 );
}

void draw_text(char *text, int x, int y, int r, int g, int b) {
#ifdef __draw_other_window
    int texW = 0;
    int texH = 0;

    SDL_Color color = { r, g, b, 0 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(other_renderer, surface);

    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { x, y, texW, texH };

    SDL_RenderCopy(other_renderer, texture, NULL, &dstrect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
#endif
}

void print_cost() {
    char text[256];

    int pos = 180;

    sprintf(text, "total: %f", get_cost() );
    draw_text(text, 10, pos, 0x2a, 0x7d, 0xd5);

    for (int i = 0; i < N_GENES; i += 3) {
        pos += 20;

        /*sprintf(text, "%2d %7.2f %7.2f %7.2f = %10.2f",*/
            /*(int)i/3,*/
            /*get_brain_pointer()->population[get_brain_pointer()->current].weight[i+0],*/
            /*get_brain_pointer()->population[get_brain_pointer()->current].weight[i+1],*/
            /*get_brain_pointer()->population[get_brain_pointer()->current].weight[i+2],*/
            /*get_brain_pointer()->population[get_brain_pointer()->current].cost[(int)i/3]);*/

        /*if ( get_brain_pointer()->population[get_brain_pointer()->current].cost[(int)i/3] < 0 ) {*/
            /*draw_text(text, 10, pos, 0xff, 0x45, 0x00);*/
        /*} else {*/
            /*draw_text(text, 10, pos, 0x2a, 0x7d, 0xd5);*/
        /*}*/

        sprintf(text, "%2d %7.2f %7.2f %7.2f = %10.2f",
            (int)i/3,
            best_piece.parameters.weight[i+0],
            best_piece.parameters.weight[i+1],
            best_piece.parameters.weight[i+2],
            best_piece.parameters.cost[(int)i/3]);

        if ( best_piece.parameters.cost[(int)i/3] < 0 ) {
            draw_text(text, 10, pos, 0xff, 0x45, 0x00);
        } else {
            draw_text(text, 10, pos, 0x2a, 0x7d, 0xd5);
        }
    }
}

void mem_fiddling() {
    if ( cpu_info->mem_controller.memory[0xffe1] == 0x0000 ) {
        char text[256];
        int index;
        int pos = 140;

        sprintf(text, "0xff80 = %04x "BYTE_TO_BINARY_PATTERN, cpu_info->mem_controller.memory[0xff80], BYTE_TO_BINARY(cpu_info->mem_controller.memory[0xff80]));
        draw_text(text, 400, 20, 0x2a, 0x90, 0xf5);

        sprintf(text, "0xff81 = %04x "BYTE_TO_BINARY_PATTERN, cpu_info->mem_controller.memory[0xff81], BYTE_TO_BINARY(cpu_info->mem_controller.memory[0xff81]));
        draw_text(text, 400, 40, 0x2a, 0x90, 0xf5);

        index = 0xff93;
        sprintf(text, "y: 0x%04x = %02d ", index, cpu_info->mem_controller.memory[index]);
        draw_text(text, 400, 60, 0x2a, 0x90, 0xf5);

        index = 0xff92;
        sprintf(text, "x: 0x%04x = %02d ", index, cpu_info->mem_controller.memory[index]);
        draw_text(text, 400, 80, 0x2a, 0x90, 0xf5);

        ////////////////////

        sprintf(text,"Most Cleared: %5d", get_brain_pointer()->most_lines_cleared);
        draw_text(text, 400, pos, 0x2a, 0x90, 0xf5);
        pos += 20;

        sprintf(text,"Worst Cleared: %4d", get_brain_pointer()->worst_lines_cleared);
        draw_text(text, 400, pos, 0x2a, 0x90, 0xf5);
        pos += 20;

        int a = get_cpu_pointer()->mem_controller.memory[0x9951];
        int b = get_cpu_pointer()->mem_controller.memory[0x9950];
        int c = get_cpu_pointer()->mem_controller.memory[0x994f];
        int d = get_cpu_pointer()->mem_controller.memory[0x994e];

        a = a == 0x2f ? 0 : a;
        b = b == 0x2f ? 0 : b;
        c = c == 0x2f ? 0 : c;
        d = d == 0x2f ? 0 : d;

        /*sprintf(text, "best: %3d , %3d ", best_piece.coord.x, best_piece.coord.y);*/
        /*draw_text(text, 40, pos, 0x2a, 0x90, 0xf5);*/
        /*pos += 20;*/

        sprintf(text,"Lines Cleared: %4d", a  * 1    +
                                           b  * 10   +
                                           c  * 100  +
                                           d  * 1000);
        draw_text(text, 400, pos, 0x2a, 0x90, 0xf5);
        pos += 20;

        sprintf(text,"Least Cleared: %4d", get_brain_pointer()->population[get_brain_pointer()->current].worst);
        draw_text(text, 400, pos, 0x2a, 0x90, 0xf5);
        pos += 20;

        index = 0x994f;
        sprintf(text, "0x%04x = %02d %02x ", index, cpu_info->mem_controller.memory[index], cpu_info->mem_controller.memory[index]);
        draw_text(text, 400, pos, 0x2a, 0x90, 0xf5);
        pos += 20;

        index = 0x9950;
        sprintf(text, "0x%04x = %02d %02x ", index, cpu_info->mem_controller.memory[index], cpu_info->mem_controller.memory[index]);
        draw_text(text, 400, pos, 0x2a, 0x90, 0xf5);
        pos += 20;

        index = 0x9951;
        sprintf(text, "0x%04x = %02d %02x ", index, cpu_info->mem_controller.memory[index], cpu_info->mem_controller.memory[index]);
        draw_text(text, 400, pos, 0x2a, 0x90, 0xf5);

        ////////////////////
        /*sprintf(text, "best: %3d , %3d ", best_piece.coord.x, best_piece.coord.y);*/
        /*draw_text(text, 20, pos, 0xff, 0x00, 0x00);*/
        ////////////////////

        pos = 0;

        sprintf(text, "diversity:   %f", get_brain_pointer()->diversity);
        draw_text(text, 110, pos, 0x2a, 0x90, 0xf5);
        pos += 20;

        sprintf(text, "generations: %3d", get_brain_pointer()->elapsed_generations);
        draw_text(text, 110, pos, 0x2a, 0x90, 0xf5);
        pos += 20;

        sprintf(text, "current:     %3d/%d", get_brain_pointer()->current, POP_SIZE);
        draw_text(text, 110, pos, 0x2a, 0x90, 0xf5);
        pos += 20;

        sprintf(text, "runs:        %3d/%d", get_brain_pointer()->runs, get_brain_pointer()->max_runs);
        draw_text(text, 110, pos, 0x2a, 0x90, 0xf5);
        pos += 20;

        ////////////////////
    }
}

void print_current_piece(){
    char text[256];
    char name[256];
    int index;
    int value;

    index = 0xc203;
    value = cpu_info->mem_controller.memory[index];

    switch (value) {
        case 0x0c:
        case 0x0d:
        case 0x0e:
        case 0x0f:
            sprintf(name, "Square");
            break;
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
            sprintf(name, "J");
            break;
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
            sprintf(name, "L");
            break;
        case 0x08:
        case 0x09:
        case 0x0a:
        case 0x0b:
            sprintf(name, "I");
            break;
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
            sprintf(name, "S");
            break;
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
            sprintf(name, "Z");
            break;
        case 0x18:
        case 0x19:
        case 0x1a:
        case 0x1b:
            sprintf(name, "T");
            break;
        default:
            sprintf(name, "??");
            break;
    }

    sprintf(text, "0x%04x = %02x %s", index, cpu_info->mem_controller.memory[index], name);
    draw_text(text, 400, 100, 0x2a, 0x90, 0xf5);
}

void print_screen_state(){
    char text[256];
    char screen[256];
    int index;

    index = cpu_info->mem_controller.memory[0xffe1];

    switch (index) {
        case 0xff:
            sprintf(screen, "????");
            break;
        case 0x24:
        case 0x25:
            sprintf(screen, "credits");
            break;
        case 0x06:
        case 0x07:
            sprintf(screen, "menu");
            break;
        case 0x08:
        case 0x0e:
            sprintf(screen, "gametype");
            break;
        case 0x10:
        case 0x11:
            sprintf(screen, "A-Game");
            break;
        case 0x00:
        case 0x0a:
            sprintf(screen, "In Game");
            break;
        case 0x01:
        case 0x0d:
        case 0x04:
            sprintf(screen, "Game Over");
            break;
        case 0x15:
        case 0x12:
            sprintf(screen, "A-Game score");
            break;
        case 0x2e:
        case 0x2f:
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34:
            sprintf(screen, "Win screen");
            break;
        default:
            sprintf(screen, "Unknow");
            printf("Unknow screen: %2x %3d\n", index, index);
            break;
    }

    sprintf(text, "screen: %02x %s", index, screen);
    draw_text(text, 400, 0, 0x2a, 0x90, 0xf5);
}

void new_piece_on_screen_hook() {
    static int old_pos = 100;
    _cpu_info *cpu = get_cpu_pointer();

    _brain *brain = get_brain_pointer();

    /*uint16_t y_pos = 0xffb2;*/
    uint16_t y_pos = 0xff93;

    piece_moved();

    /*int x = get_cpu_pointer()->mem_controller.memory[0xff92] - 8;*/
    /*int y = get_cpu_pointer()->mem_controller.memory[0xff93] - 16;*/

    if ( abs(cpu->mem_controller.memory[y_pos] - old_pos) > 8 ) {
        /*printf("New piece\n");*/
        /*evaluate_cost();*/

        brain->population[brain->current].pieces_spawned_total++;
        brain->population[brain->current].pieces_spawned[brain->runs]++;

        update_fitness();
        get_best_move();
        /*best = get_best_move();*/
        /*printf("%3d %3d\n", x, y);*/
        screen_update();
    } else if ( old_pos < cpu->mem_controller.memory[y_pos] ) {
        /*printf("%3d %3d %3d %3d\n", x, y, old_pos, cpu->mem_controller.memory[y_pos]);*/
    }

    old_pos = cpu->mem_controller.memory[y_pos];
}

void game_over_hook() {
    static int old = -1;
    int atual = cpu_info->mem_controller.memory[0xffe1];

    if ( atual == 0x000d && old != atual ) {
        reset_bg();
        check_stop_condition();
        update_fitness();
        ai_state.game_state = GAMEOVER;
        finished_evaluating_individual();
        reset_file_control();
    }

    old = atual;
}
void start_game_hook() {
    static int old = -1;
    int atual = cpu_info->mem_controller.memory[0xffe1];

    if ( atual == 0x0000 && old != atual ) {
        get_best_move();
        get_brain_pointer()->rng = 0;
        move_queue.ready = 0;
        ai_state.game_state = INGAME;
        set_can_write_file_control();
    }

    old = atual;
}

void logic_update() {
    start_game_hook();
    game_over_hook();

    if ( cpu_info->mem_controller.memory[0xffe1] == 0x0000 ) {
        new_piece_on_screen_hook();
    }
}

void screen_update() {
#if defined(__draw_other_window) && defined(__use_other_sdl)
    SDL_RenderClear(other_renderer);
    SDL_RenderCopy(other_renderer, other_bitmap, NULL, NULL);

    print_screen_state();

    if ( cpu_info->mem_controller.memory[0xffe1] == 0x0000 ) {
        draw_bg();
        /*draw_falling_blocks();*/
        draw_best();

        print_cost();

        print_current_piece();
    }

    mem_fiddling();

    SDL_UpdateTexture(other_bitmap, NULL, other_pixels, other_screenx * sizeof(uint32_t));

    SDL_RenderPresent(other_renderer);
#endif
}

void other_flip_screen ( ) {
    logic_update();
    /*screen_update();*/
}

void other_sdl_quit ( ) {
    SDL_Quit();
}

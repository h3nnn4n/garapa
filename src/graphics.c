#include <stdint.h>
#include <stdlib.h>

#include "SDL.h"
#include "SDL_video.h"
#include "SDL_render.h"

#include "types.h"
#include "graphics.h"

#define __use_sdl

#ifdef __use_sdl

static SDL_Window   *window;
static SDL_Renderer *renderer;
static SDL_Texture  *bitmap;
static uint32_t *pixels;
static int screenx = 160;
static int screeny = 144;

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

uint32_t *get_frame_buffer () {
    return pixels;
}

void sdl_init ( ) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("here comes dat gameboi",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenx * 4,
        screeny * 4,
        0);

    renderer = SDL_CreateRenderer(window, -1, 0);
    bitmap   = SDL_CreateTexture (renderer,
               SDL_PIXELFORMAT_RGB888,
               SDL_TEXTUREACCESS_STREAMING,
               screenx,
               screeny);
    pixels   = malloc ( sizeof ( uint32_t ) * screenx * screeny);
    SDL_RenderSetScale(renderer, 4, 4);

    memset(pixels, 255, screenx * screeny * sizeof(uint32_t));

    SDL_UpdateTexture(bitmap, NULL, pixels, screenx * sizeof(uint32_t));
}

void flip_screen ( ) {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bitmap, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_UpdateTexture(bitmap, NULL, pixels, screenx * sizeof(uint32_t));
}

void input_update ( _cpu_info *cpu ) {
    SDL_Event ev;

    // Input documentation from: http://computerarcheology.com/Arcade/SpaceInvaders/Hardware.html
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
            case SDL_KEYDOWN:
                switch(ev.key.keysym.sym) {
                    case SDLK_z:
                        cpu->joystick.button_a      = 0;
                        break;
                    case SDLK_x:
                        cpu->joystick.button_b      = 0;
                        break;
                    case SDLK_a:
                        cpu->joystick.button_select = 0;
                        break;
                    case SDLK_s:
                        cpu->joystick.button_start  = 0;
                        break;
                    case SDLK_LEFT:
                        cpu->joystick.button_left   = 0;
                        break;
                    case SDLK_RIGHT:
                        cpu->joystick.button_right  = 0;
                        break;
                    case SDLK_DOWN:
                        cpu->joystick.button_down   = 0;
                        break;
                    case SDLK_UP:
                        cpu->joystick.button_up     = 0;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch(ev.key.keysym.sym) {
                    case SDLK_z:
                        cpu->joystick.button_a      = 1;
                        break;
                    case SDLK_x:
                        cpu->joystick.button_b      = 1;
                        break;
                    case SDLK_a:
                        cpu->joystick.button_select = 1;
                        break;
                        case SDLK_s:
                        cpu->joystick.button_start  = 1;
                        break;
                    case SDLK_LEFT:
                        cpu->joystick.button_left   = 1;
                        break;
                    case SDLK_RIGHT:
                        cpu->joystick.button_right  = 1;
                        break;
                    case SDLK_DOWN:
                        cpu->joystick.button_down   = 1;
                        break;
                    case SDLK_UP:
                        cpu->joystick.button_up     = 1;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                exit(0);
                break;
            default:
                /*cpu->portin0 = 0x0e;*/
                /*cpu->portin1 = 0x08;*/
                /*cpu->portin2 = 0x00;*/
                break;
        }
    }
    /*printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(cpu->portin1));*/
}

void sdl_quit ( ) {
    SDL_Quit();
}

#else

#pragma GCC diagnostic ignored "-Wunused-parameter"

void flip_screen ( ) {}
void sdl_init ( ) {}
void input_update ( _cpu_info *cpu ) {}
void sdl_quit ( ) {}
uint32_t *get_frame_buffer () { return NULL; }

#endif

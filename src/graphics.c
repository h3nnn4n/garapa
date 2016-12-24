#include <SDL/SDL.h>
#include <stdint.h>
#include <stdlib.h>

#include "types.h"
#include "graphics.h"

#define __use_sdl

#ifdef __use_sdl

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

void sdl_init ( ) {
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(160, 144, 8, SDL_DOUBLEBUF);

    SDL_WM_SetCaption("8080 Emulator", NULL);

    SDL_EnableKeyRepeat(0, 0);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
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

void flip_screen ( ) {
    SDL_Flip ( screen );
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

#endif

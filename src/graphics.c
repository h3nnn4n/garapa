#include <SDL/SDL.h>
#include <stdint.h>
#include <stdlib.h>

#include "types.h"
#include "graphics.h"

/*#define __use_sdl*/

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
    screen = SDL_SetVideoMode(224, 256, 8, SDL_DOUBLEBUF);

    SDL_WM_SetCaption("8080 Emulator", NULL);

    /*SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);*/
    SDL_EnableKeyRepeat(0, 0);
}

void update_input ( _cpu_info *cpu ) {
    SDL_Event ev;

    // Input documentation from: http://computerarcheology.com/Arcade/SpaceInvaders/Hardware.html
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
            case SDL_KEYDOWN:
                switch(ev.key.keysym.sym) {
                    case SDLK_LEFT:
                        cpu->portin1 |= (1 << 5); break;
                    case SDLK_RIGHT:
                        cpu->portin1 |= (1 << 6); break;
                    case SDLK_c: // Coint
                        cpu->portin1 |= (1 << 0); break;
                    case SDLK_x: // 1P start
                        cpu->portin1 |= (1 << 2); break;
                    case SDLK_z: // 1P shot
                        cpu->portin1 |= (1 << 4); break;
                    default:
                        break;
                } break;
            case SDL_QUIT:
                exit(0);
                break;
            default:
                cpu->portin0 = 0x0e;
                cpu->portin1 = 0x08;
                cpu->portin2 = 0x00;
                break;
        }
    }
    /*printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(cpu->portin1));*/
}

void update_screen ( _cpu_info *cpu ) {
    unsigned char* raster = screen->pixels;
    uint16_t base = 0x2400;
    int x = 0;
    int y = 255;

    SDL_LockSurface( screen );

    for (int offset = 0; offset < 256 * 224/8; ++offset) {
        for (int shift = 0; shift < 8; ++shift) {
            raster[y*224 + x] = (cpu->memory[base + offset] >> shift) & 1 ? 0xff : 0x00;
            if ( --y < 0 ) {
                y = 255;
                ++x;
            }
        }
    }

    SDL_UnlockSurface( screen );
    SDL_Flip( screen );
}

void sdl_quit ( ) {
    SDL_Quit();
}

#else

#pragma GCC diagnostic ignored "-Wunused-parameter"

void sdl_init ( ) {}
void update_screen ( _cpu_info *cpu ) {}
void update_input ( _cpu_info *cpu ) {}
void sdl_quit ( ) {}
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {}

#endif

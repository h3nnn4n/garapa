#include <SDL/SDL.h>

#include "types.h"
#include "graphics.h"

/*#define __use_sdl*/

#ifdef __use_sdl

void sdl_init ( ) {
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(224, 256, 0, SDL_HWPALETTE);
    SDL_WM_SetCaption("8080 Emulator", NULL);

    black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    white = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);
}

void update_screen ( _cpu_info *cpu ) {
    SDL_LockSurface( screen );
    putpixel(screen, 2, 2, white );
    for (int i = 0; i < 224; ++i) {
        for (int j = 0; j < 256; ++j) {
            putpixel(screen, i, j, cpu->memory[0x2400 + i*224 + j] == 0 ? black : white );
        }
    }
    SDL_UnlockSurface( screen );
    SDL_UpdateRect(screen, 0, 0, 224, 256);
}

void sdl_quit ( ) {
    SDL_Quit();
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
        case 1:
            *p = pixel;
            break;

        case 2: *(Uint16 *)p = pixel; break; 
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

#else

#pragma GCC diagnostic ignored "-Wunused-parameter"

void sdl_init ( ) {}
void update_screen ( _cpu_info *cpu ) {}
void sdl_quit ( ) {}
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {}

#endif

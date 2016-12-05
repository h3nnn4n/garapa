#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL/SDL.h>

#include "types.h"

SDL_Surface *screen;

Uint32 black;
Uint32 white;

void sdl_init ( ) ;
void update_screen ( _cpu_info *cpu ) ;
void update_input ( _cpu_info *cpu ) ;
void sdl_quit ( ) ;
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel) ;

#endif /* GRAPHICS_H */

#include <stdio.h>
#include <stdint.h>
#include <string.h>


#include "types.h"
#include "overlay.h"
#include "graphics.h"

void draw_array(_cpu_info *cpu, uint16_t base, uint8_t offset, int x, int y, int r, int g, int b) {
    char text[512];
    text[0] = '\0';

    for (uint16_t addr = base; addr < base+offset; ++addr) {
        char text2[256];
        sprintf(text2, "%2x ", cpu->mem_controller.memory[addr]);
        strcat(text, text2);
    }

    draw_text_with_bg_overlay(text, x, y, r, g, b);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void overlay_main( _cpu_info *cpu ) {
    /*char text[256];*/
    int pos;

    // Draws the centre of the piece
    /*int px = cpu->mem_controller.memory[0xc202];*/
    /*int py = cpu->mem_controller.memory[0xc201];*/
    /*draw_rectangle(px*4, py*4, 8*4, 8*4, 127, 127, 0);*/
    /*sprintf(text,"%2d %2d", px, py);*/
    /*draw_text(text, px * 4, py * 4, 255, 0, 0);*/

    // Draws the bottom rightmost block
    /*int px2 = cpu->mem_controller.memory[0xff92];*/
    /*int py2 = cpu->mem_controller.memory[0xff93];*/
    /*sprintf(text,"%2d %2d", px2, py2);*/
    /*draw_rectangle(px2*4, py2*4, 8*4, 8*4, 0, 255, 0);*/
    /*draw_text(text, px2 * 4, py2 * 4, 0, 0, 255);*/

    // mirror for `0xff92`
    /*int px3 = cpu->mem_controller.memory[0xffb3];*/
    /*int py3 = cpu->mem_controller.memory[0xffb2];*/
    /*sprintf(text,"%2d %2d", px3, py3);*/
    /*draw_rectangle(px3*4, py3*4, 8*4, 8*4, 0, 127, 127);*/
    /*draw_text(text, px3 * 4, py3 * 4, 0, 255, 255);*/

    pos += 20; draw_array(cpu, 0xc0a0, 3, 20, pos, 255, 255, 0); // Score
    pos += 20; draw_array(cpu, 0xc0a3, 8, 20, pos, 255, 255, 0); // FInished row points
}

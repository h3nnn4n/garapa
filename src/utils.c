#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "types.h"
#include "disassembler.h"

off_t fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    fprintf(stderr, "Cannot determine size of %s: %s\n", filename, strerror(errno));
return -1;
}

void init_cpu( _cpu_info *cpu ) {
    cpu->memory = calloc ( 1, 0xffff ) ; // Allocs 64Kb of ram
    cpu->rom    = calloc ( 1, 512 * 1024 ) ;
    cpu->active_bank = 1;

    cpu->DMA_in_progress = 0;
    cpu->cycles_clock    = 0;
    cpu->cycles_machine  = 0;
    cpu->cycles_left     = 0;

    cpu->joystick.select_button    = 1;
    cpu->joystick.select_direction = 1;

    cpu->joystick.button_a      = 1;
    cpu->joystick.button_b      = 1;
    cpu->joystick.button_select = 1;
    cpu->joystick.button_start  = 1;

    cpu->joystick.button_left   = 1;
    cpu->joystick.button_right  = 1;
    cpu->joystick.button_down   = 1;
    cpu->joystick.button_up     = 1;

    cpu->pc     = 0;
    cpu->a      = 0;
    cpu->b      = 0;
    cpu->c      = 0;
    cpu->d      = 0;
    cpu->e      = 0;
    cpu->h      = 0;
    cpu->l      = 0;

    cpu->lcd.active_line      = 0;
    cpu->lcd.mode             = 0;

    cpu->lcd.power            = 1;
    cpu->lcd.window_tilemap   = 0;
    cpu->lcd.window_enabled   = 0;
    cpu->lcd.bg_and_tilemap   = 0;
    cpu->lcd.bg_tileset       = 0;
    cpu->lcd.sprite_size      = 0;
    cpu->lcd.sprite_enable    = 0;
    cpu->lcd.bg_enable        = 0;

    cpu->lcd.lyc_enable       = 0;
    cpu->lcd.mode2_oam        = 0;
    cpu->lcd.mode1_vblank     = 0;
    cpu->lcd.mode0_hblank     = 0;
    cpu->lcd.lyc_ly_triggered = 0;

    cpu->lcd.scy            = 0;
    cpu->lcd.scx            = 0;
    cpu->lcd.bgx            = 0;
    cpu->lcd.bgy            = 0;

    cpu->lcd.bg_palette[0] = 3;
    cpu->lcd.bg_palette[1] = 2;
    cpu->lcd.bg_palette[2] = 1;
    cpu->lcd.bg_palette[3] = 0;

    cpu->lcd.spr1_palette[0] = 0;
    cpu->lcd.spr1_palette[1] = 1;
    cpu->lcd.spr1_palette[2] = 2;
    cpu->lcd.spr1_palette[3] = 3;

    cpu->lcd.spr2_palette[0] = 0;
    cpu->lcd.spr2_palette[1] = 1;
    cpu->lcd.spr2_palette[2] = 2;
    cpu->lcd.spr2_palette[3] = 3;

    cpu->lcd.colors[0] = 0xffffff;
    cpu->lcd.colors[1] = 0xc0c0c0;
    cpu->lcd.colors[2] = 0x808080;
    cpu->lcd.colors[3] = 0x000000;

    cpu->flags.z   = 0;
    cpu->flags.n   = 0;
    cpu->flags.h   = 0;
    cpu->flags.c   = 0;

    cpu->halted                = 0;
    cpu->stoped                = 0;
    cpu->enable_interrupts     = 0;
    cpu->instructions_executed = 0;
    cpu->interrupt_mask        = 0;
    cpu->interrupt_flag        = 0;

    cpu->timer.TAC  = 0;
    cpu->timer.TIMA = 0;
    cpu->timer.DIV  = 0;
    cpu->timer.TMA  = 0;

    cpu->interrupts.masked_vblank   = 1;
    cpu->interrupts.masked_lcdstat  = 1;
    cpu->interrupts.masked_timer    = 1;
    cpu->interrupts.masked_serial   = 1;
    cpu->interrupts.masked_joypad   = 1;

    cpu->interrupts.pending_vblank  = 0;
    cpu->interrupts.pending_lcdstat = 0;
    cpu->interrupts.pending_timer   = 0;
    cpu->interrupts.pending_serial  = 0;
    cpu->interrupts.pending_joypad  = 0;

    // BIOS SKIP
    cpu->pc = 0x100;
    cpu->sp = 0xfffe;

    cpu->a  = 0x01;
    cpu->b  = 0x00;
    cpu->c  = 0x13;
    cpu->d  = 0x00;
    cpu->e  = 0xd8;
    cpu->h  = 0x01;
    cpu->l  = 0x4d;

    cpu->flags.z = 1;
    cpu->flags.n = 0;
    cpu->flags.h = 1;
    cpu->flags.c = 1;

    cpu->memory[0xff10] = 0x80;
    cpu->memory[0xff11] = 0xbf;
    cpu->memory[0xff12] = 0xf3;
    cpu->memory[0xff14] = 0xbf;
    cpu->memory[0xff16] = 0x3f;
    cpu->memory[0xff19] = 0xbf;
    cpu->memory[0xff1a] = 0x7f;
    cpu->memory[0xff1b] = 0xff;
    cpu->memory[0xff1c] = 0x9f;
    cpu->memory[0xff1e] = 0xbf;
    cpu->memory[0xff20] = 0xff;
    cpu->memory[0xff23] = 0xbf;
    cpu->memory[0xff24] = 0x77;
    cpu->memory[0xff25] = 0xf3;
    cpu->memory[0xff26] = 0xf1;
    cpu->memory[0xff40] = 0x91;
    cpu->memory[0xff47] = 0xfc;
    cpu->memory[0xff48] = 0xff;
    cpu->memory[0xff49] = 0xff;
}

void unimplemented_opcode( _cpu_info *cpu ) {
    disassembler ( cpu->memory, cpu->pc );
    exit(-1);
}

int parity_bit ( int b ) {
    int bits = 0;
    int i    = 0;

    for (i = 0; i < 8; ++i)
        if ( b & ( 1 << i ) )
            bits += 1;

    return bits & 1 ? 0 : 1;
}

void print_registers ( _cpu_info *cpu ) {
    uint8_t f = ( (cpu->flags.z ) ? 0x80 : 0x00 ) |
                ( (cpu->flags.n ) ? 0x40 : 0x00 ) |
                ( (cpu->flags.h ) ? 0x20 : 0x00 ) |
                ( (cpu->flags.c ) ? 0x10 : 0x00 ) ;

    printf(" AF: %02x%02x BC: %02x%02x DE: %02x%02x HL: %02x%02x PC: %04x SP: %04x",
            cpu->a, f, cpu->b, cpu->c, cpu->d, cpu->e, cpu->h, cpu->l, cpu->pc, cpu->sp);
    printf(" F: %c%c%c%c CYCLES: %16llu IPS: %16llu\n",
            cpu->flags.z  ? 'z' : '.',
            cpu->flags.n  ? 'n' : '.',
            cpu->flags.h  ? 'h' : '.',
            cpu->flags.c  ? 'c' : '.',
            cpu->cycles_machine      ,
            cpu->instructions_executed);
    /*printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(f));*/
}

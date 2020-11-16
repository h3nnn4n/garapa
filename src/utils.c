/******************************************************************************
 * Copyright (C) 2016-2018  Renan S. Silva                                    *
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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "audio.h"
#include "cartridge.h"
#include "disassembler.h"
#include "display.h"
#include "types.h"
#include "utils.h"

void load_rom(_cpu_info *cpu, const char *fname, uint16_t offset) {
    FILE *f = NULL;
    /*off_t buffer_size = -1;*/

    f = fopen(fname, "rb");

    if (f == NULL)
        fprintf(stderr, "Error while opening: %s\n", fname);

    /*buffer_size = fsize( fname );*/

    if (fread(cpu->mem_controller.memory, 0x0200, 1, f) != 1) {
        printf("Something went weird while reading the ROM\n");
        exit(-1);
    }

    cpu->mem_controller.ram_size = get_ram_code(cpu->mem_controller.memory);
    cpu->mem_controller.rom_size = get_rom_code(cpu->mem_controller.memory);

    cpu->mem_controller.rom           = calloc(1, get_rom_size(cpu->mem_controller.memory));
    cpu->mem_controller.cartridge_ram = calloc(1, get_ram_size(cpu->mem_controller.memory));

    fseek(f, 0, SEEK_SET);

    if (fread(cpu->mem_controller.rom + offset, get_rom_size(cpu->mem_controller.memory), 1, f) != 1) {
        printf("Something went weird while reading the ROM\n");
        exit(-1);
    }

    /*memcpy(&cpu->mem_controller.memory[0x0000], &cpu->mem_controller.rom[0x0000], 0x4000);*/
    /*memcpy(&cpu->mem_controller.memory[0x4000], &cpu->mem_controller.rom[0x4000], 0x4000);*/

    fclose(f);

    /*fprintf(stderr, "Loaded %s into %04x to %04x\n", fname, offset, offset +(uint16_t) buffer_size);*/
}

off_t fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    fprintf(stderr, "Cannot determine size of %s: %s\n", filename, strerror(errno));
    return -1;
}

void init_cpu(_cpu_info *cpu) {
    cpu->mem_controller.memory          = calloc(1, 0xffff);
    cpu->mem_controller.rom             = NULL;
    cpu->mem_controller.cartridge_ram   = NULL;
    cpu->mem_controller.rom_bank_number = 1;
    cpu->mem_controller.ram_bank_number = 0;
    cpu->mem_controller.ram_mode        = 0;
    cpu->mem_controller.ram_enable      = 0;

    cpu->halt_bug       = 0;
    cpu->cycles_clock   = 0;
    cpu->cycles_machine = 0;
    cpu->cycles_left    = 0;

    cpu->dma.oam_dma_source      = 0;
    cpu->dma.oam_dma_next_source = 0;
    cpu->dma.oam_dma_delay_timer = 0;
    cpu->dma.oam_dma_index       = 0;
    cpu->dma.oam_dma_timer       = 0;

    cpu->joystick.select_button    = 1;
    cpu->joystick.select_direction = 1;

    cpu->joystick.button_a      = 1;
    cpu->joystick.button_b      = 1;
    cpu->joystick.button_select = 1;
    cpu->joystick.button_start  = 1;

    cpu->joystick.button_left  = 1;
    cpu->joystick.button_right = 1;
    cpu->joystick.button_down  = 1;
    cpu->joystick.button_up    = 1;

    apu_reset(cpu);

    cpu->pc = 0;
    cpu->a  = 0;
    cpu->b  = 0;
    cpu->c  = 0;
    cpu->d  = 0;
    cpu->e  = 0;
    cpu->h  = 0;
    cpu->l  = 0;

    cpu->lcd.lyc_trigger  = 0;
    cpu->lcd.active_line  = 0;
    cpu->lcd.mode         = 0;
    cpu->lcd.m3_cycles    = 0;
    cpu->lcd.stat_irq     = 0;
    cpu->lcd.lyc_delay    = 0;
    cpu->lcd.cycles_spent = 0;

    cpu->lcd.power          = 1;
    cpu->lcd.window_tilemap = 0;
    cpu->lcd.window_enabled = 0;
    cpu->lcd.bg_and_tilemap = 1;
    cpu->lcd.bg_tileset     = 0;
    cpu->lcd.sprite_size    = 0;
    cpu->lcd.sprite_enable  = 0;
    cpu->lcd.bg_enable      = 1;

    cpu->lcd.lyc_enable       = 0;
    cpu->lcd.mode2_oam        = 0;
    cpu->lcd.mode1_vblank     = 0;
    cpu->lcd.mode0_hblank     = 0;
    cpu->lcd.lyc_ly_triggered = 0;

    cpu->lcd.window_scroll_y = 0;
    cpu->lcd.window_scroll_x = 0;
    cpu->lcd.bg_scroll_x     = 0;
    cpu->lcd.bg_scroll_y     = 0;

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

    cpu->flags.z = 0;
    cpu->flags.n = 0;
    cpu->flags.h = 0;
    cpu->flags.c = 0;

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

    cpu->timer.TIMA_timer        = 0;
    cpu->timer.TIMA_reload_timer = 0;
    cpu->timer._timer            = 0;
    cpu->timer._timer_old        = 0;

    cpu->interrupts.masked_vblank  = 1;
    cpu->interrupts.masked_lcdstat = 1;
    cpu->interrupts.masked_timer   = 1;
    cpu->interrupts.masked_serial  = 1;
    cpu->interrupts.masked_joypad  = 1;

    cpu->interrupts.pending_vblank  = 0;
    cpu->interrupts.pending_lcdstat = 0;
    cpu->interrupts.pending_timer   = 0;
    cpu->interrupts.pending_serial  = 0;
    cpu->interrupts.pending_joypad  = 0;

    // BIOS SKIP
    cpu->timer._timer = 0xabcc;
    cpu->timer.DIV    = 0xab;

    cpu->pc = 0x100;
    cpu->sp = 0xfffe;

    cpu->a = 0x01;
    cpu->b = 0x00;
    cpu->c = 0x13;
    cpu->d = 0x00;
    cpu->e = 0xd8;
    cpu->h = 0x01;
    cpu->l = 0x4d;

    cpu->flags.z = 1;
    cpu->flags.n = 0;
    cpu->flags.h = 1;
    cpu->flags.c = 1;

    for (int i = 0xff00; i < 0xffff; ++i)
        cpu->mem_controller.memory[i] = 0xff;

    cpu->mem_controller.memory[0xff00] = 0xcf;
    cpu->mem_controller.memory[0xff01] = 0x00;
    cpu->mem_controller.memory[0xff02] = 0x7e;
    cpu->mem_controller.memory[0xff03] = 0xff;
    cpu->mem_controller.memory[0xff05] = 0x00;
    cpu->mem_controller.memory[0xff06] = 0x00;
    cpu->mem_controller.memory[0xff07] = 0xf8;
    cpu->mem_controller.memory[0xff08] = 0xff;
    cpu->mem_controller.memory[0xff09] = 0xff;
    cpu->mem_controller.memory[0xff10] = 0x80;
    cpu->mem_controller.memory[0xff0a] = 0xff;
    cpu->mem_controller.memory[0xff0f] = 0xe1;
    cpu->mem_controller.memory[0xff11] = 0xbf;
    cpu->mem_controller.memory[0xff12] = 0xf3;
    cpu->mem_controller.memory[0xff14] = 0xbf;
    cpu->mem_controller.memory[0xff16] = 0x3f;
    cpu->mem_controller.memory[0xff17] = 0x00;
    cpu->mem_controller.memory[0xff19] = 0xbf;
    cpu->mem_controller.memory[0xff1a] = 0x7f;
    cpu->mem_controller.memory[0xff1b] = 0xff;
    cpu->mem_controller.memory[0xff1c] = 0x9f;
    cpu->mem_controller.memory[0xff1e] = 0xbf;
    cpu->mem_controller.memory[0xff20] = 0xff;
    cpu->mem_controller.memory[0xff21] = 0x00;
    cpu->mem_controller.memory[0xff22] = 0x00;
    cpu->mem_controller.memory[0xff23] = 0xbf;
    cpu->mem_controller.memory[0xff24] = 0x77;
    cpu->mem_controller.memory[0xff25] = 0xf3;
    cpu->mem_controller.memory[0xff26] = 0xf1;
    cpu->mem_controller.memory[0xff40] = 0x91;
    cpu->mem_controller.memory[0xff42] = 0x00;
    cpu->mem_controller.memory[0xff43] = 0x00;
    cpu->mem_controller.memory[0xff45] = 0x00;
    cpu->mem_controller.memory[0xff47] = 0xfc;
    cpu->mem_controller.memory[0xff48] = 0xff;
    cpu->mem_controller.memory[0xff49] = 0xff;
    cpu->mem_controller.memory[0xff4a] = 0x00;
    cpu->mem_controller.memory[0xff4b] = 0x00;
}

void unimplemented_opcode(_cpu_info *cpu) {
    disassembler(cpu);
    exit(-1);
}

int parity_bit(int b) {
    int bits = 0;
    int i    = 0;

    for (i = 0; i < 8; ++i)
        if (b & (1 << i))
            bits += 1;

    return bits & 1 ? 0 : 1;
}

void print_registers(_cpu_info *cpu) {
    uint8_t f = ((cpu->flags.z) ? 0x80 : 0x00) | ((cpu->flags.n) ? 0x40 : 0x00) | ((cpu->flags.h) ? 0x20 : 0x00) |
                ((cpu->flags.c) ? 0x10 : 0x00);

    printf(" AF: %02x%02x BC: %02x%02x DE: %02x%02x HL: %02x%02x PC: %04x SP: %04x", cpu->a, f, cpu->b, cpu->c, cpu->d,
           cpu->e, cpu->h, cpu->l, cpu->pc, cpu->sp);
    printf(" F: %c%c%c%c CYCLES: %16llu IPS: %16llu\n", cpu->flags.z ? 'z' : '.', cpu->flags.n ? 'n' : '.',
           cpu->flags.h ? 'h' : '.', cpu->flags.c ? 'c' : '.', cpu->cycles_machine, cpu->instructions_executed);
    /*printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(f));*/
}

void print_state(_cpu_info *cpu) {
    disassembler(cpu);
    print_registers(cpu);
}

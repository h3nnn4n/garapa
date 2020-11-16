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
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#include "audio_types.h"

typedef struct {              // This is the struct that is
    uint8_t button_right;     // used to communicate with
    uint8_t button_left;      // the external world; In this
    uint8_t button_up;        // case it is SDL
    uint8_t button_down;      //
                              // Individual bits for each
    uint8_t button_a;         // button.
    uint8_t button_b;         //
    uint8_t button_select;    //
    uint8_t button_start;     //
                              //
    uint8_t select_button;    // Here are the flags to select
    uint8_t select_direction; // which column is being read
} _joystick;                  //

typedef struct {
    int16_t posx;
    int16_t posy;
    uint8_t hflip;
    uint8_t vflip;

    uint8_t palette_number;

    uint8_t priority; // 1 if above background
                      // PS: Actual bit in OAM is 0 for when above
    uint8_t tile;
    uint8_t tileaddr;

    uint8_t color_bit1;
    uint8_t color_bit2;

    uint16_t tile_addr;
} _sprite_info;

typedef struct {
    _sprite_info sprites[40];
    uint8_t      sprite_pivot;
    uint16_t     pixel_pipeline_cycles;

    uint8_t priority_cache[144 * 160];
    uint8_t sprite_x_cache[144 * 160];
    uint8_t sprite_stall_buckets[(168 + 256 + 7) / 8]; // Big enough? Should be
} _lcd_status;

typedef struct { // This is a struct that holds all information
                 // relative to the LCD controller. Somethings
                 // are mirrored from RAM.

    _lcd_status lcd_status; // Status about the inner workings of the LCD
                            // such as cache and sprite priority

    uint8_t active_line; // The line number being draw, can be read from 0xff44
    uint8_t lyc_trigger; // The line on 0xff45 used to trigger LYC
    uint8_t stat_irq;    // Used to simulate a falling edge detector

    uint16_t cycles_spent; // How many cycles the LCD spent on the active scanline
    uint8_t  lyc_delay;    // LY = LYC has a delay, this controls it
    uint8_t  mode_cmp;     // Used to emulate something fishy for the LCD

    // Those are the bits from LCD controll at 0xff40
    uint8_t power;          // Bit 7
    uint8_t window_tilemap; // Bit 6
    uint8_t window_enabled; // Bit 5
    uint8_t bg_and_tilemap; // Bit 4
    uint8_t bg_tileset;     // Bit 3
    uint8_t sprite_size;    // Bit 2
    uint8_t sprite_enable;  // Bit 1
    uint8_t bg_enable;      // Bit 0

    // Those are the bits from LCD Stat at 0xff41
    // Bit 7 is ununsed
    uint8_t lyc_enable;       // Bit 6
    uint8_t mode2_oam;        // Bit 5
    uint8_t mode1_vblank;     // Bit 4
    uint8_t mode0_hblank;     // Bit 3
    uint8_t lyc_ly_triggered; // Bit 2
    uint8_t mode;             // Bit 0 and 1

    uint16_t m3_cycles; // Extra Cycles for mode3

    // The color palettes for
    uint8_t  bg_palette[4];   // background at 0xff47
    uint8_t  spr1_palette[4]; // sprite 1 at 0xff48
    uint8_t  spr2_palette[4]; // sprite 2 at 0xff49
    uint32_t colors[4];       // This is the colors that the UI draws
                              // Maybe I should hide it somehere else?

    //// Bad Naming FTW
    // uint8_t scy;              // Windows Y scroll at 0xff4a
    // uint8_t scx;              // Windows X scroll at 0xff4b
    // uint8_t bgx;              // Background Y scroll at 0xff42
    // uint8_t bgy;              // Background X scroll at 0xff42

    uint8_t bg_scroll_y;     // Background Y scroll at 0xff42
    uint8_t bg_scroll_x;     // Background X scroll at 0xff42
    uint8_t window_scroll_x; // Windows Y scroll at 0xff4a
    uint8_t window_scroll_y; // Windows X scroll at 0xff4b

    // TODO: Use this
    uint8_t window_line_y; // To keep track of which lines of the window where draw
} _lcd;                    // TODO: I think that I should have a mirror here for LY compare from 0xff45

typedef struct {               // Timer struct
    uint8_t TIMA_timer;        // AntonioND reports that when TIMA resets, it takes 4 cycles for
                               // it to get the value from TMA. Meanwhile the value is 0x00. This
                               // does not add to the total of cycles necessary to overflow;
    uint8_t TIMA_reload_timer; // After the m-cycles where TIMA is 0x00, it is reloaded with TMA.
                               // Attemping to writing to TIMA while TMA is being written will be ignored;
    uint16_t _timer_old;       //
    uint16_t _timer;           //

    uint16_t running; // Bit 2 from TAC
    uint16_t speed;   //
                      //
    uint16_t TIMA;    // Time Counter, R/W from 0xff03. It is incremeneted at a
                      // Speed specified from TAC bit 0 and 1. When Value gets
                      // higher than 0xff it resets to the value of TMA;
    uint16_t DIV;     // Divider Register, incremented at 16384 hz, R/W from 0xff04
                      // If written to it is always reset to 0;
    uint16_t TMA;     // Timer Modulo, R/W from 0xff06. Holds the value that
                      // TIMA is reset to on overflow
    uint8_t TAC;      // Timer Control. R/W from 0xff07. Some bits are mirroed
                      // in the beggining of this struct;
} _timer;             //

typedef struct {             // Interrupt controll Struct;
    uint8_t pending_vblank;  // The pending_XYZ with a value of
    uint8_t pending_lcdstat; // 1 indicates that there is a XYZ
    uint8_t pending_timer;   // interrupt waiting to be serviced;
    uint8_t pending_serial;  //
    uint8_t pending_joypad;  //
                             //
    uint8_t masked_vblank;   // A value of 1 here means that the
    uint8_t masked_lcdstat;  // respective interrupt is masked and
    uint8_t masked_timer;    // wont be serviced.
    uint8_t masked_serial;   //
    uint8_t masked_joypad;   //

    uint8_t ununsed_bits; // Ununsed bits that are R/W
                          // Gekkio's unused_hwio-GS tests it
} _interrupts;            //

typedef struct { // The flags from the CPU
    uint8_t z;   // Zero flag
    uint8_t n;   // Subtraction bit
    uint8_t h;   // Half carry bit
    uint8_t c;   // Carry bit
} _cpu_flags;    //

typedef struct {             // Struct that holds the info relative to the ROM and RAM banking
                             // The values here are control states and not values mapped from RAM;
    uint8_t *memory;         // Holds the address space visible to the CPU.
    uint8_t *rom;            // Pointer to the DATA read from the cartridge (ROM)
    uint8_t *cartridge_ram;  // Pointer to Cartridge RAM, if any;
                             //
    uint8_t ram_size;        // Cartridge RAM/ROM Size, in bytes. This is read from the
    uint8_t rom_size;        // cartridge header and resources are allocated accordingly;
                             //
    uint8_t rom_bank_number; // The ROM bank in use; Read from 0x2000-0x3fff
    uint8_t ram_bank_number; // The RAM bank in use; Read from 0x4000-0x5fff; Works as the
                             // upper 2 bits for the ROM depending on ram_mode;
    uint8_t ram_mode;        // Read from 0x6000-0x7fff; A value of zero means that
                             // the ram_bank_number works as the upper 2 bits after rom_bank_number,
                             // otherwise it activates ram banking;
    uint8_t ram_enable;      // Ram R/W flag; if 0 no data cam be accessed; Disabled by default;
                             // Read from 0x0000-0x1fff
} _mem_controller;           // TODO: Rewrite it, so that the _mem_controller works as a bus;
                             // This way all memory access will be centralized here

typedef struct {                  // Struct for holding all the DMA information
    uint16_t oam_dma_source;      // The source;
    uint16_t oam_dma_next_source; // Source for the next DMA, if any;
    uint8_t  oam_dma_delay_timer; // Delay until next DMA;
    uint16_t oam_dma_index;       // Index for the running DMA;
    uint16_t oam_dma_timer;       // How long the DMA has been running;
} _dma;                           //

typedef struct {                    // Struct that holds ALL information relative to the gameboy state;
    _mem_controller mem_controller; // The structs for other parts of the gameboy's inner state
    _interrupts     interrupts;     //
    _joystick       joystick;       //
    _cpu_flags      flags;          //
    _timer          timer;          //
    _lcd            lcd;            //
    _dma            dma;            //
    _apu            apu;            //
                                    //
    uint8_t enable_interrupts;      // Interrupt enable flag
    uint8_t pending_interrupts;     // Interrupt delay. In some situations interrupts take some extra cycles
                                    // to be serviced, this counter controls it;
                                    //
    uint8_t interrupt_mask;         // This value is mapped from _interrupts masked_XYZ and 0xffff
    uint8_t interrupt_flag;         // This value is mapped from _interrupts pending_XYZ and 0xff0f
                                    //
    uint8_t halted;                 // 1 if the cpu is halted
    uint8_t stoped;                 // 1 if the cpu is stoped
                                    //
    uint8_t opcode;                 // The current opcode being executed
    uint8_t cycles_left; // Checks when the CPU is free to fetch/dedoce/execute the next instruction. This is not used
    unsigned long long cycles_machine;        // M-Cycles - machine cycles
    unsigned long long cycles_clock;          // T-Cycles - timer   cycles
    unsigned long long instructions_executed; // Just a counter of how many instructions were executed
                                              //
    uint8_t halt_bug;                         // Used to signal the halt bug
                                              //
    uint16_t pc;                              // The registers for the cpu
    uint16_t sp;                              //
                                              //
    uint8_t a;                                //
    uint8_t b;                                //
    uint8_t c;                                //
    uint8_t d;                                //
    uint8_t e;                                //
    uint8_t h;                                //
    uint8_t l;                                //

    uint8_t die; // Used by the test routines. Signals when emulator should stop;
} _cpu_info;     //

#endif /* TYPES_H */

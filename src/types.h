#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef struct {
    uint8_t button_right;
    uint8_t button_left;
    uint8_t button_up;
    uint8_t button_down;

    uint8_t button_a;
    uint8_t button_b;
    uint8_t button_select;
    uint8_t button_start;

    uint8_t select_button;
    uint8_t select_direction;
} _joystick;

typedef struct {
    uint8_t active_line;
    uint8_t mode;

    uint8_t power;            // Bit 7
    uint8_t window_tilemap;   // Bit 6
    uint8_t window_enabled;   // Bit 5
    uint8_t bg_and_tilemap;   // Bit 4
    uint8_t bg_tileset;       // Bit 3
    uint8_t sprite_size;      // Bit 2
    uint8_t sprite_enable;    // Bit 1
    uint8_t bg_enable;        // Bit 0

    uint8_t lyc_enable;
    uint8_t mode2_oam;
    uint8_t mode1_vblank;
    uint8_t mode0_hblank;
    uint8_t lyc_ly_triggered;

    uint8_t  bg_palette[4];
    uint8_t  spr1_palette[4];
    uint8_t  spr2_palette[4];
    uint32_t colors[4];

    uint8_t scy;
    uint8_t scx;
    uint8_t bgx;
    uint8_t bgy;
} _lcd;

typedef struct {
    uint16_t running;
    uint16_t speed;

    uint16_t TIMA;
    uint16_t DIV;
    uint16_t TMA;
    uint8_t  TAC;
} _timer;

typedef struct {
    uint8_t pending_vblank;
    uint8_t pending_lcdstat;
    uint8_t pending_timer;
    uint8_t pending_serial;
    uint8_t pending_joypad;

    uint8_t masked_vblank;
    uint8_t masked_lcdstat;
    uint8_t masked_timer;
    uint8_t masked_serial;
    uint8_t masked_joypad;
} _interrupts;

typedef struct {
    uint8_t z  ; // Zero flag
    uint8_t n  ; // Subtraction bit ?
    uint8_t h  ; // Half carry bit
    uint8_t c  ; // Carry bit
} _cpu_flags;

typedef struct {
    _interrupts interrupts;
    _joystick   joystick;

    uint8_t *memory;
    uint8_t *rom;
    uint8_t active_bank;

    uint8_t enable_interrupts;
    uint8_t pending_interrupts;

    uint8_t interrupt_mask;
    uint8_t interrupt_flag;

    uint8_t halted;
    uint8_t stoped;

    uint8_t            cycles_left;    // Checks when the CPU is free to fetch/dedoce/execute the next instruction
    unsigned long long cycles_machine; // M-Cycles - machine cycles
    unsigned long long cycles_clock;   // T-Cycles - timer   cycles
    unsigned long long instructions_executed;

    _cpu_flags flags;
    _timer     timer;
    _lcd       lcd;

    uint8_t DMA_in_progress;

    uint16_t pc;
    uint16_t sp;

    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
} _cpu_info;

#endif /* TYPES_H */

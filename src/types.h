#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

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

    unsigned char *memory;

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

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef struct {
    uint8_t z  ; // Zero flag
    uint8_t s  ; // Signal bit
    uint8_t p  ; // Parity bit
    uint8_t cy ; // Cary bit
    uint8_t ac ; // Auxiliary carry
} _cpu_flags;

typedef struct {
    unsigned char *memory;
    unsigned char enable_interrupts;

    uint32_t cycles;
    uint32_t instructions_executed;

    _cpu_flags flags;

    uint16_t pc;
    uint16_t sp;
    uint16_t interrupt_addr;

    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
} _cpu_info;

#endif /* TYPES_H */

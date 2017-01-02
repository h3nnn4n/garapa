#include "types.h"
#include "memory.h"

#include "time_keeper.h"

void write_byte_at_sp ( _cpu_info *cpu, uint8_t data ) {
    timer_tick_and_full_mcycle( cpu );

    cpu->sp -= 1;
    write_byte ( cpu, cpu->sp, data );
}

uint8_t read_byte_at_sp ( _cpu_info *cpu ) {
    timer_tick_and_full_mcycle( cpu );

    uint8_t value = read_byte ( cpu, cpu->sp );
    cpu->sp += 1;

    return value;
}

uint8_t read_byte_at_pc ( _cpu_info *cpu ) {
    timer_tick_and_full_mcycle( cpu );

    uint8_t value = read_byte ( cpu, cpu->pc );
    cpu->pc += 1;

    return value;
}

uint8_t read_byte_with_tick ( _cpu_info *cpu, uint16_t addr ) {
    timer_tick_and_full_mcycle( cpu );

    uint8_t value = read_byte ( cpu, addr );

    return value;
}

void write_byte_with_tick ( _cpu_info *cpu, uint16_t addr, uint8_t data ) {
    timer_tick_and_full_mcycle( cpu );

    write_byte ( cpu, addr, data );
}

uint16_t read_bc ( _cpu_info *cpu ) {
    return (cpu->b << 8) | cpu->c;
}

uint16_t read_de ( _cpu_info *cpu ) {
    return (cpu->d << 8) | cpu->e;
}

uint16_t read_hl ( _cpu_info *cpu ) {
    return (cpu->h << 8) | cpu->l;
}

void write_bc_16 ( _cpu_info *cpu, uint16_t data ) {
    cpu->c = (data & 0x00ff) >> 0;
    cpu->b = (data & 0xff00) >> 8;
}

void write_de_16 ( _cpu_info *cpu, uint16_t data ) {
    cpu->e = (data & 0x00ff) >> 0;
    cpu->d = (data & 0xff00) >> 8;
}

void write_hl_16 ( _cpu_info *cpu, uint16_t data ) {
    cpu->l = (data & 0x00ff) >> 0;
    cpu->h = (data & 0xff00) >> 8;
}

void write_hl ( _cpu_info *cpu, uint8_t lo, uint8_t hi ) {
    cpu->l = lo;
    cpu->h = hi;
}
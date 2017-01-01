#include "types.h"
#include "memory.h"

#include "time_keeper.h"

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

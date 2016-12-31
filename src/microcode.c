#include "types.h"
#include "memory.h"

uint8_t read_byte_at_pc ( _cpu_info *cpu ) {
    uint8_t value = read_byte ( cpu, cpu->pc );
    cpu->pc += 1;
    cpu->cycles_machine += 1;
    cpu->cycles_clock   += 4;

    return value;
}

uint8_t read_byte_with_tick ( _cpu_info *cpu, uint16_t addr ) {
    uint8_t value = read_byte ( cpu, addr );
    cpu->cycles_machine += 1;
    cpu->cycles_clock   += 4;

    return value;
}

void write_byte_with_tick ( _cpu_info *cpu, uint16_t addr, uint8_t data ) {
    write_byte ( cpu, addr, data );
    cpu->cycles_machine += 1;
    cpu->cycles_clock   += 4;
}

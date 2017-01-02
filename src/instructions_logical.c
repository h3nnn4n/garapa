#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "types.h"
#include "utils.h"
#include "halfcarry.h"
#include "memory.h"
#include "microcode.h"

#include "instructions_logical.h"

void emulate_ANA ( _cpu_info *cpu ) {
    switch ( cpu->opcode ) {
        case 0xa0: // ANA B
            cpu->flags.h  = ((cpu->a | cpu->b) & 0x08) != 0;
            cpu->a       &= cpu->b;
            break;
        case 0xa1: // ANA C
            cpu->flags.h  = ((cpu->a | cpu->c) & 0x08) != 0;
            cpu->a       &= cpu->c;
            break;
        case 0xa2: // ANA D
            cpu->flags.h  = ((cpu->a | cpu->d) & 0x08) != 0;
            cpu->a       &= cpu->d;
            break;
        case 0xa3: // ANA E
            cpu->flags.h  = ((cpu->a | cpu->e) & 0x08) != 0;
            cpu->a       &= cpu->e;
            break;
        case 0xa4: // ANA H
            cpu->flags.h  = ((cpu->a | cpu->h) & 0x08) != 0;
            cpu->a       &= cpu->h;
            break;
        case 0xa5: // ANA L
            cpu->flags.h  = ((cpu->a | cpu->l) & 0x08) != 0;
            cpu->a       &= cpu->l;
            break;
        case 0xa6: // ANA M
            cpu->flags.h  = ((cpu->a | read_byte ( cpu, read_hl ( cpu ))) & 0x08) != 0;
            cpu->a       &= read_byte_with_tick ( cpu, read_hl ( cpu ));
            break;
        case 0xa7: // ANA A
            cpu->flags.h  = ((cpu->a | cpu->a) & 0x08) != 0;
            cpu->a       &= cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.c  = 0;
    cpu->flags.h  = 1;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.n  = 0;
}

void emulate_ANI ( _cpu_info *cpu ) {
    switch ( cpu->opcode ) {
        case 0xe6: // ANI
            cpu->a &= read_byte_at_pc ( cpu );
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.c  = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.n  = 0;
    cpu->flags.h  = 1;
}

void emulate_XOR ( _cpu_info *cpu ) {
    switch ( cpu->opcode ) {
        case 0xa8: // XOR B
            cpu->a ^= cpu->b;
            break;
        case 0xa9: // XOR C
            cpu->a ^= cpu->c;
            break;
        case 0xaa: // XOR D
            cpu->a ^= cpu->d;
            break;
        case 0xab: // XOR E
            cpu->a ^= cpu->e;
            break;
        case 0xac: // XOR H
            cpu->a ^= cpu->h;
            break;
        case 0xad: // XOR L
            cpu->a ^= cpu->l;
            break;
        case 0xae: // XOR M
            cpu->a ^= read_byte_with_tick ( cpu, read_hl ( cpu ));
            break;
        case 0xaf: // XOR A
            cpu->a ^= cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.c  = 0;
    cpu->flags.h  = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.n  = 0;
}

void emulate_XRI ( _cpu_info *cpu ) {
    switch ( cpu->opcode ) {
        case 0xee: // XRI D8
            cpu->a ^= read_byte_at_pc ( cpu );
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.c  = 0;
    cpu->flags.h  = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.n  = 0;
}

void emulate_ORA ( _cpu_info *cpu ) {
    switch ( cpu->opcode ) {
        case 0xb0: // ORA B
            cpu->a |= cpu->b;
            break;
        case 0xb1: // ORA C
            cpu->a |= cpu->c;
            break;
        case 0xb2: // ORA D
            cpu->a |= cpu->d;
            break;
        case 0xb3: // ORA E
            cpu->a |= cpu->e;
            break;
        case 0xb4: // ORA H
            cpu->a |= cpu->h;
            break;
        case 0xb5: // ORA L
            cpu->a |= cpu->l;
            break;
        case 0xb6: // ORA M
            cpu->a |= read_byte_with_tick ( cpu, read_hl ( cpu ));
            break;
        case 0xb7: // ORA A
            cpu->a |= cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.c = 0;
    cpu->flags.h = 0;
    cpu->flags.z = (cpu->a == 0);
    cpu->flags.n = 0;
}

void emulate_ORI ( _cpu_info *cpu ) {
    switch ( cpu->opcode ) {
        case 0xf6: // ORI D8
            cpu->a |= read_byte_at_pc ( cpu );
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.c  = 0;
    cpu->flags.h  = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.n  = 0;
}

void emulate_CMP ( _cpu_info *cpu ) {
    uint8_t        answer = 0;
    uint8_t        old    = 0;

    switch ( cpu->opcode ) {
        case 0xb8: // CMP B
            answer        = cpu->a - cpu->b;
            old           = cpu->b;
            break;
        case 0xb9: // CMP C
            answer        = cpu->a - cpu->c;
            old           = cpu->c;
            break;
        case 0xba: // CMP D
            answer        = cpu->a - cpu->d;
            old           = cpu->d;
            break;
        case 0xbb: // CMP E
            answer        = cpu->a - cpu->e;
            old           = cpu->e;
            break;
        case 0xbc: // CMP H
            answer        = cpu->a - cpu->h;
            old           = cpu->h;
            break;
        case 0xbd: // CMP L
            answer        = cpu->a - cpu->l;
            old           = cpu->l;
            break;
        case 0xbe: // CMP M
            answer        = cpu->a - read_byte_with_tick ( cpu, read_hl ( cpu ));
            old           = read_byte ( cpu, read_hl ( cpu ) );
            break;
        case 0xbf: // CMP A
            answer        = cpu->a - cpu->a;
            old           = cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.n    = 1;
    cpu->flags.h    = ( answer & 0x0f ) > (cpu->a & 0x0f);
    cpu->flags.c    = ( cpu->a < old  );
}

void emulate_CPI ( _cpu_info *cpu ) {
    uint8_t        answer  = 0;
    uint8_t        operand = 0;

    switch ( cpu->opcode ) {
        case 0xfe: // CPI A
            operand          = read_byte_at_pc ( cpu );
            cpu->flags.h = ((cpu->a - operand) & 0x0f) > (cpu->a & 0x0f);
            answer       = cpu->a - operand;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.n    = 1;
    cpu->flags.c    = ( cpu->a < operand );
}


void emulate_RAL ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc+1];
    uint8_t answer = 0;
    uint8_t      t = 0;
    uint16_t  addr = 0;

    switch ( *opcode ) {
        case 0x16: // RAL
            addr = ( cpu->h << 8 ) | cpu->l;
            t = read_byte( cpu, addr );
            answer = ( t << 1 ) | ( cpu->flags.c  ? 0x1 : 0x0 );
            write_byte(cpu, addr, answer);
            cpu->flags.c  = (t & 0x80) != 0;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z = answer == 0;
    cpu->flags.n = 0;
    cpu->flags.h = 0;

    /* FIXME */ abort();
    cpu->cycles_machine += 1 ;
    cpu->pc     += 2 ;
}

void emulate_RAR ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc+1];
    uint8_t answer = 0;
    uint8_t      t = 0;
    uint16_t  addr = 0;

    switch ( *opcode ) {
        case 0x1f: // RAR
            addr = ( cpu->h << 8 ) | cpu->l;
            t = read_byte( cpu, addr );
            answer = ( t >> 1 ) | (( cpu->flags.c  & 0x1 ) << 7 );
            write_byte(cpu, addr, answer);
            cpu->flags.c  = t & 0x1;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z = answer == 0;
    cpu->flags.n = 0;
    cpu->flags.h = 0;

    /* FIXME */ abort();
    cpu->cycles_machine += 1 ;
    cpu->pc     += 2 ;
}

void emulate_CMA ( _cpu_info *cpu ) {
    switch ( cpu->opcode ) {
            case 0x2f: // CMA
                cpu->a ^= 0xff;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.h = 1 ;
    cpu->flags.n = 1 ;
}

void emulate_CMC ( _cpu_info *cpu ) {
    switch ( cpu->opcode ) {
            case 0x3f: // CMC
                cpu->flags.c  = !cpu->flags.c ;
                cpu->flags.n  = 0;
                cpu->flags.h  = 0;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

void emulate_STC ( _cpu_info *cpu ) {
    switch ( cpu->opcode ) {
            case 0x37: // STC
                cpu->flags.c  = 1;
                cpu->flags.n  = 0;
                cpu->flags.h  = 0;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

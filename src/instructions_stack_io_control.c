#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "memory.h"
#include "types.h"

#include "instructions_stack_io_control.h"

void emulate_POP ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xc1: // POP B
            cpu->c = cpu->memory[ ( cpu->sp + 0 ) & 0xffff ];
            cpu->b = cpu->memory[ ( cpu->sp + 1 ) & 0xffff ];
            cpu->sp += 2;
            break;
        case 0xd1: // POP D
            cpu->e = cpu->memory[ ( cpu->sp + 0 ) & 0xffff ];
            cpu->d = cpu->memory[ ( cpu->sp + 1 ) & 0xffff ];
            cpu->sp += 2;
            break;
        case 0xe1: // POP H
            cpu->l = cpu->memory[ ( cpu->sp + 0 ) & 0xffff ];
            cpu->h = cpu->memory[ ( cpu->sp + 1 ) & 0xffff ];
            cpu->sp += 2;
            break;
        case 0xf1: // POP PSW
            cpu->a = cpu->memory[cpu->sp+1];
            cpu->flags.z  = ((cpu->memory[cpu->sp] & 0x80));
            cpu->flags.n  = ((cpu->memory[cpu->sp] & 0x40));
            cpu->flags.h  = ((cpu->memory[cpu->sp] & 0x20));
            cpu->flags.c  = ((cpu->memory[cpu->sp] & 0x10));
            cpu->sp += 2;
            cpu->cycles_machine -= 1;
            break;
        default:
            assert( 0 && "Code should not get here\n" ); }

    cpu->cycles_machine += 11;
    cpu->pc     += 1 ;
}

void emulate_PUSH ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xc5: // PUSH B
            cpu->memory[cpu->sp-2] = cpu->c;
            cpu->memory[cpu->sp-1] = cpu->b;
            cpu->sp -= 2;
            break;
        case 0xd5: // PUSH D
            cpu->memory[cpu->sp-2] = cpu->e;
            cpu->memory[cpu->sp-1] = cpu->d;
            cpu->sp -= 2;
            break;
        case 0xe5: // PUSH H
            cpu->memory[cpu->sp-2] = cpu->l;
            cpu->memory[cpu->sp-1] = cpu->h;
            cpu->sp -= 2;
            break;
        case 0xf5: // PUSH PSW
            cpu->memory[cpu->sp-1] = cpu->a;
            cpu->memory[cpu->sp-2] = ( cpu->flags.z  ? 0x80 : 0x0 ) |
                                     ( cpu->flags.n  ? 0x40 : 0x0 ) |
                                     ( cpu->flags.h  ? 0x20 : 0x0 ) |
                                     ( cpu->flags.c  ? 0x10 : 0x0 ) ;
            cpu->sp = cpu->sp - 2;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 11;
    cpu->pc     += 1 ;
}


void emulate_XTHL ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xe3: // XTHL
            {
            uint8_t l = cpu->l;
            uint8_t h = cpu->h;
            cpu->l = cpu->memory[cpu->sp + 0];
            cpu->h = cpu->memory[cpu->sp + 1];
            cpu->memory[cpu->sp + 0] = l;
            cpu->memory[cpu->sp + 1] = h;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 18;
    cpu->pc     += 1 ;
}

void emulate_SPHL ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xf9: // SPHL B
            cpu->sp = cpu->h << 8 | cpu->l;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 5;
    cpu->pc     += 1 ;
}

void emulate_EI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
            case 0xfb: // EI
            cpu->enable_interrupts = 1;
            cpu->pending_interrupts = 2;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 4 ;
    cpu->pc     += 1 ;
}

void emulate_DI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
            case 0xf3: // EI
            cpu->enable_interrupts = 0;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 4 ;
    cpu->pc     += 1 ;
}

void emulate_NOP ( _cpu_info *cpu ) {
    cpu->cycles_machine += 4 ;
    cpu->pc     += 1 ;
}

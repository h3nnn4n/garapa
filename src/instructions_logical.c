#include <stdint.h>
#include <assert.h>

#include "types.h"
#include "utils.h"
#include "halfcary.h"

#include "instructions_logical.h"

void emulate_ANA ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
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
            cpu->flags.h  = ((cpu->a | cpu->memory[cpu->h << 8 | cpu->l]) & 0x08) != 0;
            cpu->a       &= cpu->memory[cpu->h << 8 | cpu->l];
            cpu->cycles  += 3;
            break;
        case 0xa7: // ANA A
            cpu->flags.h  = ((cpu->a | cpu->a) & 0x08) != 0;
            cpu->a       &= cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.c  = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.n  = (0x80 == (cpu->a & 0x80));

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_ANI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xe6: // ANI
            cpu->a       &= opcode[1];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.c  = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.n  = 0;
    cpu->flags.h  = 1;

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
}

void emulate_XRA ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xa8: // XRA B
            cpu->a ^= cpu->b;
            break;
        case 0xa9: // XRA C
            cpu->a ^= cpu->c;
            break;
        case 0xaa: // XRA D
            cpu->a ^= cpu->d;
            break;
        case 0xab: // XRA E
            cpu->a ^= cpu->e;
            break;
        case 0xac: // XRA H
            cpu->a ^= cpu->h;
            break;
        case 0xad: // XRA L
            cpu->a ^= cpu->l;
            break;
        case 0xae: // XRA M
            cpu->a ^= cpu->memory[cpu->h << 8 | cpu->l];
            cpu->cycles += 3;
            break;
        case 0xaf: // XRA A
            cpu->a ^= cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.c  = 0;
    cpu->flags.h  = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.n  = (0x80 == (cpu->a & 0x80));

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_XRI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xee: // XRI D8
            cpu->a ^= opcode[1];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.c  = 0;
    cpu->flags.h  = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.n  = 0;

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
}

void emulate_ORA ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
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
            cpu->a |= cpu->memory[cpu->h << 8 | cpu->l];
            cpu->cycles += 3;
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

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_ORI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xf6: // ORI D8
            cpu->a |= opcode[1];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.c  = 0;
    cpu->flags.h  = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.n  = 0;

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
}

void emulate_CMP ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint8_t        answer = 0;

    switch ( *opcode ) {
        case 0xb8: // CMP B
            answer        = cpu->a - cpu->b;
            cpu->flags.h  = !halfcary_sub( cpu->a, cpu->b, answer );
            cpu->flags.c  = cpu->a < cpu->b;
            break;
        case 0xb9: // CMP C
            answer        = cpu->a - cpu->c;
            cpu->flags.h  = !halfcary_sub( cpu->a, cpu->c, answer );
            cpu->flags.c  = cpu->a < cpu->c; break;
        case 0xba: // CMP D
            answer        = cpu->a - cpu->d;
            cpu->flags.h  = !halfcary_sub( cpu->a, cpu->d, answer );
            cpu->flags.c  = cpu->a < cpu->d;
            break;
        case 0xbb: // CMP E
            answer        = cpu->a - cpu->e;
            cpu->flags.h  = !halfcary_sub( cpu->a, cpu->e, answer );
            cpu->flags.c  = cpu->a < cpu->e;
            break;
        case 0xbc: // CMP H
            answer        = cpu->a - cpu->h;
            cpu->flags.h  = !halfcary_sub( cpu->a, cpu->h, answer );
            cpu->flags.c  = cpu->a < cpu->h;
            break;
        case 0xbd: // CMP L
            answer        = cpu->a - cpu->l;
            cpu->flags.h  = !halfcary_sub( cpu->a, cpu->l, answer );
            cpu->flags.c  = cpu->a < cpu->l;
            break;
        case 0xbe: // CMP M
            answer        = cpu->a - cpu->memory[cpu->h << 8 | cpu->l];
            cpu->flags.h  = !halfcary_sub( cpu->a, cpu->memory[cpu->h << 8 | cpu->l], answer );
            cpu->flags.c  = cpu->a < cpu->memory[cpu->h << 8 | cpu->l];
            cpu->cycles  += 3;
            break;
        case 0xbf: // CMP A
            answer        = cpu->a - cpu->a;
            cpu->flags.h  = !halfcary_sub( cpu->a, cpu->a, answer );
            cpu->flags.c  = cpu->a < cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0                  ; // Only the last 8 bits matters, hence the mask
    cpu->flags.n    = ( answer & 0x80 ) != 0       ; // Checks if the MSB is 1

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_CPI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint8_t        answer = 0;

    switch ( *opcode ) {
        case 0xfe: // CPI A
            answer        = cpu->a - opcode[1];
            cpu->flags.h  = ((cpu->a - opcode[1]) & 0x0f) > (cpu->a & 0x0f);
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.n    = 1;
    cpu->flags.c    = ( cpu->a < opcode[1] );

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
}

void emulate_RLC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint8_t t = 0;

    switch ( *opcode ) {
        case 0x07: // RLC
            t = cpu->a;
            cpu->flags.c  = 0x80 == (t & 0x80);
            cpu->a = ( t << 1 ) | cpu->flags.c ;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_RRC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint8_t t = 0;

    switch ( *opcode ) {
        case 0x0f: // RRC
            t = cpu->a;
            cpu->a = ( t >> 1 ) | (( t & 0x1 ) << 7 );
            cpu->flags.c  = t & 0x1;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_RAL ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint8_t t = 0;

    switch ( *opcode ) {
        case 0x17: // RAL
            t = cpu->a;
            cpu->a = ( t << 1 ) | ( cpu->flags.c  ? 0x1 : 0x0 );
            cpu->flags.c  = 0 < ( t & 0x80 );
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_RAR ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint8_t t = 0;

    switch ( *opcode ) {
        case 0x1f: // RAR
            t = cpu->a;
            cpu->a = ( t >> 1 ) | (( cpu->flags.c  & 0x1 ) << 7 );
            cpu->flags.c  = t & 0x1;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_CMA ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
            case 0x2f: // CMA
                cpu->a ^= 0xff;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_CMC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
            case 0x3f: // CMC
                cpu->flags.c  = !cpu->flags.c ;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_STC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
            case 0x37: // STC
            cpu->flags.c  = 1;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "types.h"
#include "utils.h"
#include "emulator.h"
#include "disassembler.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
      (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

static int halfcary_sub( uint8_t a, uint8_t b, uint8_t c ) {
    uint8_t b1 = ( a & 0x08 ) >> 1;
    uint8_t b2 = ( b & 0x08 ) >> 2;
    uint8_t b3 = ( c & 0x08 ) >> 3;
    static int table[] = { 0, 1, 1, 1, 0, 0, 0, 1 };

    return table[ b1 | b2 | b3 ];
}

static int halfcary( uint8_t a, uint8_t b, uint8_t c ) {
    uint8_t b1 = ( a & 0x08 ) >> 1;
    uint8_t b2 = ( b & 0x08 ) >> 2;
    uint8_t b3 = ( c & 0x08 ) >> 3;
    static int table[] = { 0, 0, 1, 0, 1, 0, 1, 1 };

    return table[ b1 | b2 | b3 ];
}


void print_registers ( _cpu_info *cpu ) {
    uint8_t f = ( (cpu->flags.z ) ? 0x40 : 0x00 ) |
                ( (cpu->flags.s ) ? 0x80 : 0x00 ) |
                ( (cpu->flags.p ) ? 0x04 : 0x00 ) |
                ( (cpu->flags.cy) ? 0x01 : 0x00 ) |
                ( (cpu->flags.ac) ? 0x10 : 0x00 ) |
                0x02;

    printf(" AF: %02x%02x BC: %02x%02x DE: %02x%02x HL: %02x%02x PC: %04x SP: %04x",
            cpu->a, f, cpu->b, cpu->c, cpu->d, cpu->e, cpu->h, cpu->l, cpu->pc, cpu->sp);
    printf(" F: %c%c%c%c%c CYCLES: %16llu IPS: %16llu\n",
            cpu->flags.z  ? 'z' : '.',
            cpu->flags.s  ? 's' : '.',
            cpu->flags.p  ? 'p' : '.',
            cpu->flags.cy ? 'c' : '.',
            cpu->flags.ac ? 'a' : '.',
            cpu->cycles              ,
            cpu->instructions_executed);
    /*printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(f));*/
}

void emulate_CMC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
            case 0x3f: // CMC
                cpu->flags.cy = !cpu->flags.cy;
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

void emulate_STC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
            case 0x37: // STC
            cpu->flags.cy = 1;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
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

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_EI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
            case 0xfb: // EI
            cpu->enable_interrupts = 1;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_ACI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t t;

    switch ( *opcode ) {
        case 0xce: // ACI
            t             = cpu->a + (uint8_t) opcode[1] + cpu->flags.cy;
            cpu->flags.ac = halfcary( cpu->a, opcode[1], t);
            cpu->a        = t & 0xff;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.cy = (t > 0xff);
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.s  = (0x80 == (cpu->a & 0x80));
    cpu->flags.p  = parity_bit(cpu->a);

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
}

void emulate_ADI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t t;

    switch ( *opcode ) {
            case 0xc6: // ADI
            cpu->flags.ac = halfcary( cpu->a, opcode[1], cpu->a + opcode[1] );
            t             = cpu->a + (uint8_t) opcode[1];
            cpu->a        = (uint8_t) t;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.cy = (t > 0xff);
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.s  = (0x80 == (cpu->a & 0x80));
    cpu->flags.p  = parity_bit(cpu->a);

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
}

void emulate_ANA ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xa0: // ANA B
            cpu->flags.ac = ((cpu->a | cpu->b) & 0x08) != 0;
            cpu->a       &= cpu->b;
            break;
        case 0xa1: // ANA C
            cpu->flags.ac = ((cpu->a | cpu->c) & 0x08) != 0;
            cpu->a       &= cpu->c;
            break;
        case 0xa2: // ANA D
            cpu->flags.ac = ((cpu->a | cpu->d) & 0x08) != 0;
            cpu->a       &= cpu->d;
            break;
        case 0xa3: // ANA E
            cpu->flags.ac = ((cpu->a | cpu->e) & 0x08) != 0;
            cpu->a       &= cpu->e;
            break;
        case 0xa4: // ANA H
            cpu->flags.ac = ((cpu->a | cpu->h) & 0x08) != 0;
            cpu->a       &= cpu->h;
            break;
        case 0xa5: // ANA L
            cpu->flags.ac = ((cpu->a | cpu->l) & 0x08) != 0;
            cpu->a       &= cpu->l;
            break;
        case 0xa6: // ANA M
            cpu->flags.ac = ((cpu->a | cpu->memory[cpu->h << 8 | cpu->l]) & 0x08) != 0;
            cpu->a       &= cpu->memory[cpu->h << 8 | cpu->l];
            cpu->cycles  += 3;
            break;
        case 0xa7: // ANA A
            cpu->flags.ac = ((cpu->a | cpu->a) & 0x08) != 0;
            cpu->a       &= cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.cy = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.s  = (0x80 == (cpu->a & 0x80));
    cpu->flags.p  = parity_bit(cpu->a);

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

    cpu->flags.cy = 0;
    cpu->flags.ac = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.s  = (0x80 == (cpu->a & 0x80));
    cpu->flags.p  = parity_bit(cpu->a);

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

    cpu->flags.cy = 0;
    cpu->flags.ac = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.s  = (0x80 == (cpu->a & 0x80));
    cpu->flags.p  = parity_bit(cpu->a);

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

    cpu->flags.cy = 0;
    cpu->flags.ac = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.s  = (0x80 == (cpu->a & 0x80));
    cpu->flags.p  = parity_bit(cpu->a);

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

    cpu->flags.cy = 0;
    cpu->flags.ac = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.s  = (0x80 == (cpu->a & 0x80));
    cpu->flags.p  = parity_bit(cpu->a);

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_ANI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xe6: // ANI
            cpu->flags.ac = ((cpu->a | opcode[1]) & 0x08) != 0;
            cpu->a       &= opcode[1];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.cy = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.s  = (0x80 == (cpu->a & 0x80));
    cpu->flags.p  = parity_bit(cpu->a);

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
}

void emulate_DCX ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint32_t       answer = 0;

    switch ( *opcode ) {
        case 0x0b: // DCX B
            answer  = cpu->b << 8 | cpu->c;
            answer -= 1;
            cpu->b  = (answer >> 8 ) & 0xff;
            cpu->c  = (answer >> 0 ) & 0xff;
            break;
        case 0x1b: // DCX D
            answer  = cpu->d << 8 | cpu->e;
            answer -= 1;
            cpu->d  = (answer >> 8 ) & 0xff;
            cpu->e  = (answer >> 0 ) & 0xff;
            break;
        case 0x2b: // DCX H
            answer  = cpu->h << 8 | cpu->l;
            answer -= 1;
            cpu->h  = (answer >> 8 ) & 0xff;
            cpu->l  = (answer >> 0 ) & 0xff;
            break;
        case 0x3b: // DCX SP
            answer  = cpu->sp;
            answer -= 1;
            cpu->sp  = answer & 0xffff;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 5 ;
    cpu->pc     += 1 ;
}

void emulate_DAD ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint32_t       answer = 0;

    switch ( *opcode ) {
        case 0x09: // DAD B
            answer  = cpu->h << 8 | cpu->l;
            answer += cpu->b << 8 | cpu->c;
            cpu->flags.cy = 0 < ( answer & 0xffff0000 );
            break;
        case 0x19: // DAD D
            answer  = cpu->h << 8 | cpu->l;
            answer += cpu->d << 8 | cpu->e;
            cpu->flags.cy = 0 < ( answer & 0xffff0000 );
            break;
        case 0x29: // DAD H
            answer  = cpu->h << 8 | cpu->l;
            answer += cpu->h << 8 | cpu->l;
            cpu->flags.cy = 0 < ( answer & 0xffff0000 );
            break;
        case 0x39: // DAD SP
            answer  = cpu->h << 8 | cpu->l;
            answer += cpu->sp;
            cpu->flags.cy = 0 < ( answer & 0xffff0000 );
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->h  = (answer >> 8 ) & 0xff;
    cpu->l  = (answer >> 0 ) & 0xff;

    cpu->cycles += 10;
    cpu->pc     += 1 ;
}

void emulate_RAL ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint8_t t = 0;

    switch ( *opcode ) {
        case 0x17: // RAL
            t = cpu->a;
            cpu->a = ( t << 1 ) | ( cpu->flags.cy ? 0x1 : 0x0 );
            cpu->flags.cy = 0 < ( t & 0x80 );
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
            cpu->a = ( t >> 1 ) | (( cpu->flags.cy & 0x1 ) << 7 );
            cpu->flags.cy = t & 0x1;
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
            cpu->flags.cy = t & 0x1;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_RLC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint8_t t = 0;

    switch ( *opcode ) {
        case 0x07: // RLC
            t = cpu->a;
            cpu->flags.cy = 0x80 == (t & 0x80);
            cpu->a = ( t << 1 ) | cpu->flags.cy;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_XCHG ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint8_t t = 0;

    switch ( *opcode ) {
        case 0xeb: // XCHG
            t = cpu->d;
            cpu->d = cpu->h;
            cpu->h = t;

            t = cpu->e;
            cpu->e = cpu->l;
            cpu->l = t;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ; // FIXME THis is 5
    cpu->pc     += 1 ;
}

void emulate_IN ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xdb: // IN
            {
                uint8_t port = opcode[1];
                switch ( port ) {
                    case 0x01: // COIN-IN port. always zero for now
                        {
                            cpu->a = 0;
                        }
                        break;
                    case 0x02: // TILT port. What this does?
                        {
                            cpu->a = 0;
                        }
                        break;
                    case 0x03: // Shiftregister port
                        {
                        uint16_t v = (cpu->shift1<<8) | cpu->shift0;
                        cpu->a = ((v >> (8 - cpu->shift_offset)) & 0xff);
                        }
                        break;
                    default:
                        disassembler ( cpu->memory, cpu->pc );
                        print_registers(cpu);
                        printf("PORT N %4x\n", port);
                        assert( 0 && "NEW PORT\n" );
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10;
    cpu->pc     += 2 ;
}

void emulate_OUT ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xd3: // OUT
            {
                uint8_t port = opcode[1];
                switch ( port ) {
                    case 0x02: // Sets the offset size for the shift register
                        cpu->shift_offset = cpu->a & 0x7;
                        break;
                    case 0x03: // sound port
                        // Do nothing
                        break;
                    case 0x04: // Throws data at the shiftregister
                        cpu->shift0 = cpu->shift1;
                        cpu->shift1 = cpu->a;
                        break;
                    case 0x05: // sound port
                        // Do nothing
                        break;
                    case 0x06: // Watchchdog port
                        // Do nothing
                        break;
                    default:
                        disassembler ( cpu->memory, cpu->pc );
                        print_registers(cpu);
                        printf("PORT N %4x\n", port);
                        assert( 0 && "NEW PORT\n" );
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10;
    cpu->pc     += 2 ;
}

void emulate_CMP ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint8_t        answer = 0;

    switch ( *opcode ) {
        case 0xb8: // CMP B
            answer        = cpu->a - cpu->b;
            cpu->flags.ac = !halfcary_sub( cpu->a, cpu->b, answer );
            cpu->flags.cy = cpu->a < cpu->b;
            break;
        case 0xb9: // CMP C
            answer        = cpu->a - cpu->c;
            cpu->flags.ac = !halfcary_sub( cpu->a, cpu->c, answer );
            cpu->flags.cy = cpu->a < cpu->c;
            break;
        case 0xba: // CMP D
            answer        = cpu->a - cpu->d;
            cpu->flags.ac = !halfcary_sub( cpu->a, cpu->d, answer );
            cpu->flags.cy = cpu->a < cpu->d;
            break;
        case 0xbb: // CMP E
            answer        = cpu->a - cpu->e;
            cpu->flags.ac = !halfcary_sub( cpu->a, cpu->e, answer );
            cpu->flags.cy = cpu->a < cpu->e;
            break;
        case 0xbc: // CMP H
            answer        = cpu->a - cpu->h;
            cpu->flags.ac = !halfcary_sub( cpu->a, cpu->h, answer );
            cpu->flags.cy = cpu->a < cpu->h;
            break;
        case 0xbd: // CMP L
            answer        = cpu->a - cpu->l;
            cpu->flags.ac = !halfcary_sub( cpu->a, cpu->l, answer );
            cpu->flags.cy = cpu->a < cpu->l;
            break;
        case 0xbe: // CMP M
            answer        = cpu->a - cpu->memory[cpu->h << 8 | cpu->l];
            cpu->flags.ac = !halfcary_sub( cpu->a, cpu->memory[cpu->h << 8 | cpu->l], answer );
            cpu->flags.cy = cpu->a < cpu->memory[cpu->h << 8 | cpu->l];
            cpu->cycles  += 3;
            break;
        case 0xbf: // CMP A
            answer        = cpu->a - cpu->a;
            cpu->flags.ac = !halfcary_sub( cpu->a, cpu->a, answer );
            cpu->flags.cy = cpu->a < cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0                  ; // Only the last 8 bits matters, hence the mask
    cpu->flags.s    = ( answer & 0x80 ) != 0       ; // Checks if the MSB is 1
    cpu->flags.p    = parity_bit ( answer & 0xff ) ; // Returns 1 if the number os bits set as 1 is even

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_CPI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint8_t        answer = 0;

    switch ( *opcode ) {
        case 0xfe: // CPI A
            answer        = cpu->a - opcode[1];
            cpu->flags.ac = !halfcary_sub( cpu->a, opcode[1], answer );
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0                  ; // Only the last 8 bits matters, hence the mask
    cpu->flags.s    = ( answer & 0x80 ) != 0       ; // Checks if the MSB is 1
    cpu->flags.cy   = ( cpu->a < opcode[1] )       ; // Checks for the carry bit
    cpu->flags.p    = parity_bit ( answer & 0xff ) ; // Returns 1 if the number os bits set as 1 is even
    /*cpu->flags.ac   = (((answer & 0xf) +*/
                        /*(opcode[1] & 0xf)) & 0x10)*/
                                            /*> 0x0f; // Half carry magic*/
    /*cpu->flags.ac   =!(!(answer & 15 && answer & 31));// Half carry magic*/

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
}

void emulate_ADC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t answer = 0;
    uint8_t a, b;
    a = cpu->a;

    switch ( *opcode ) {
        case 0x88: // ADC B
            answer = cpu->a + cpu->b + cpu->flags.cy;
            cpu->a = answer & 0xff;
            b = cpu->b;
            break;
        case 0x89: // ADC C
            answer = cpu->a + cpu->c + cpu->flags.cy;
            cpu->a = answer & 0xff;
            b = cpu->c;
            break;
        case 0x8a: // ADC D
            answer = cpu->a + cpu->d + cpu->flags.cy;
            cpu->a = answer & 0xff;
            b = cpu->d;
            break;
        case 0x8b: // ADC E
            answer = cpu->a + cpu->e + cpu->flags.cy;
            cpu->a = answer & 0xff;
            b = cpu->e;
            break;
        case 0x8c: // ADC H
            answer = cpu->a + cpu->h + cpu->flags.cy;
            cpu->a = answer & 0xff;
            b = cpu->h;
            break;
        case 0x8d: // ADC L
            answer = cpu->a + cpu->l + cpu->flags.cy;
            cpu->a = answer & 0xff;
            b = cpu->l;
            break;
        case 0x8e: // ADC M
            answer = cpu->a + cpu->memory[cpu->h << 8 | cpu->l] + cpu->flags.cy;
            cpu->a = answer & 0xff;
            b = cpu->memory[cpu->h << 8 | cpu->l];
            cpu->cycles += 3;
            break;
        case 0x8f: // ADC A
            answer = cpu->a + cpu->a + cpu->flags.cy;
            cpu->a = answer & 0xff;
            b = cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.s    = ( answer & 0x80 ) != 0;
    cpu->flags.cy   = ( answer > 0xff );
    cpu->flags.p    = parity_bit ( answer & 0xff );
    cpu->flags.ac   = halfcary( a, b, answer );

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

// TODO: Find out what this does and DO IT
// I dont trust this code
void emulate_DAA ( _cpu_info *cpu ) {
#pragma message ( "STUB DAA" )
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0x27: // DAA
            {
                uint8_t carry = (uint8_t)cpu->flags.cy;
                uint8_t add = 0;
                uint16_t ans = 0;

                if (cpu->flags.ac || (cpu->a & 0x0f) > 9) {
                    add = 0x06;
                }

                if (cpu->flags.cy || (cpu->a >> 4) > 9 || ((cpu->a >> 4) >= 9 && (cpu->a & 0x0f) > 9)) {
                    add |= 0x60;
                    carry = 1;
                }

                ans = add + cpu->a;

                cpu->flags.ac = halfcary( cpu->a, add, ans );
                cpu->a        = ans & 0xff;
                cpu->flags.p  = parity_bit(ans);
                cpu->flags.cy = carry;
                cpu->flags.z  = ( ans & 0xff ) == 0;
                cpu->flags.s  = ( ans & 0x80 ) != 0;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_SBI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t answer = 0;

    switch ( *opcode ) {
        case 0xde: // SBI D8
            answer = cpu->a - opcode[1] - cpu->flags.cy;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.s    = ( answer & 0x80 ) != 0;
    cpu->flags.cy   = ( answer > 0xff );
    cpu->flags.p    = parity_bit ( answer & 0xff );
    cpu->flags.ac   = !halfcary_sub( cpu->a, opcode[1], answer );

    cpu->a          = answer & 0xff;

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
}

void emulate_SBB ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t answer = 0;

    switch ( *opcode ) {
        case 0x98:
            answer        = cpu->a - cpu->b - cpu->flags.cy;
            cpu->flags.ac = !halfcary_sub(cpu->a, cpu->b , answer);
            break;
        case 0x99:
            answer        = cpu->a - cpu->c - cpu->flags.cy;
            cpu->flags.ac = !halfcary_sub(cpu->a, cpu->c , answer);
            break;
        case 0x9a:
            answer        = cpu->a - cpu->d - cpu->flags.cy;
            cpu->flags.ac = !halfcary_sub(cpu->a, cpu->d , answer);
            break;
        case 0x9b:
            answer        = cpu->a - cpu->e - cpu->flags.cy;
            cpu->flags.ac = !halfcary_sub(cpu->a, cpu->e , answer);
            break;
        case 0x9c:
            answer        = cpu->a - cpu->h - cpu->flags.cy;
            cpu->flags.ac = !halfcary_sub(cpu->a, cpu->h , answer);
            break;
        case 0x9d:
            answer        = cpu->a - cpu->l - cpu->flags.cy;
            cpu->flags.ac = !halfcary_sub(cpu->a, cpu->l , answer);
            break;
        case 0x9e:
            {
            uint16_t addr = cpu->h << 8 | cpu->l;
            answer        = cpu->a - cpu->memory[addr] - cpu->flags.cy;
            cpu->flags.ac = !halfcary_sub(cpu->a, cpu->memory[addr], answer);
            cpu->cycles   += 3;
            }
            break;
        case 0x9f:
            answer        = cpu->a - cpu->a - cpu->flags.cy;
            cpu->flags.ac = !halfcary_sub(cpu->a, cpu->a , answer);
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0       ; // Only the last 8 bits matters, hence the mask
    cpu->flags.s    = ( answer & 0x80 ) != 0       ; // Checks if the MSB is 1
    cpu->flags.cy   = ( answer > 0xff )            ; // Checks for the carry bit
    cpu->flags.p    = parity_bit ( answer & 0xff ) ; // Returns 1 if the number os bits set as 1 is even
    cpu->a          = answer & 0xff                ; // Stores the 8 bit result in the A register

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_SUI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t answer = 0;

    switch ( *opcode ) {
        case 0xd6: // SUI D8
            answer = cpu->a - opcode[1];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0       ; // Only the last 8 bits matters, hence the mask
    cpu->flags.s    = ( answer & 0x80 ) != 0       ; // Checks if the MSB is 1
    cpu->flags.cy   = ( answer > 0xff )            ; // Checks for the carry bit
    cpu->flags.p    = parity_bit ( answer & 0xff ) ; // Returns 1 if the number os bits set as 1 is even
    cpu->flags.ac   = !halfcary_sub( cpu->a, opcode[1], answer );

    cpu->a = answer & 0xff;

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
}

void emulate_DCR ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t answer = 0;

    /*H_FLAG = !(((reg) & 0x0f) == 0x0f);         \*/

    switch ( *opcode ) {
        case 0x05: // DCR B
            answer        = cpu->b - 1;
            cpu->b        = answer & 0xff;
            cpu->flags.ac = !((cpu->b & 0x0f) == 0x0f);
            break;
        case 0x0d: // DCR C
            answer        = cpu->c - 1;
            cpu->c        = answer & 0xff;
            cpu->flags.ac = !((cpu->c & 0x0f) == 0x0f);
            break;
        case 0x15: // DCR D
            answer        = cpu->d - 1;
            cpu->d        = answer & 0xff;
            cpu->flags.ac = !((cpu->d & 0x0f) == 0x0f);
            break;
        case 0x1d: // DCR E
            answer        = cpu->e - 1;
            cpu->e        = answer & 0xff;
            cpu->flags.ac = !((cpu->e & 0x0f) == 0x0f);
            break;
        case 0x25: // DCR H
            answer        = cpu->h - 1;
            cpu->h        = answer & 0xff;
            cpu->flags.ac = !((cpu->h & 0x0f) == 0x0f);
            break;
        case 0x2d: // DCR L
            answer        = cpu->l - 1;
            cpu->l        = answer & 0xff;
            cpu->flags.ac = !((cpu->l & 0x0f) == 0x0f);
            break;
        case 0x35: // DCR M
            answer                            = cpu->memory[cpu->h << 8 | cpu->l] - 1;
            cpu->memory[cpu->h << 8 | cpu->l] = answer & 0xff;
            cpu->flags.ac                     = !((cpu->memory[cpu->h << 8 | cpu->l] & 0x0f) == 0x0f);
            cpu->cycles += 5;
            break;
        case 0x3d: // DCR A
            answer        = cpu->a - 1;
            cpu->a        = answer & 0xff;
            cpu->flags.ac = !((cpu->a & 0x0f) == 0x0f);
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.s    = ( answer & 0x80 ) != 0;
    cpu->flags.p    = parity_bit ( answer & 0xff );
    /*cpu->flags.cy   = 0;*/

    cpu->cycles += 5 ;
    cpu->pc     += 1 ;
}

void emulate_STAX ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0x02: // STAX B
            cpu->memory[ ( cpu->b << 8 ) | cpu->c ] = cpu->a;
            break;
        case 0x12: // STAX D
            cpu->memory[ ( cpu->d << 8 ) | cpu->e ] = cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 7 ;
    cpu->pc     += 1 ;
}

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
            cpu->flags.z  = ((cpu->memory[cpu->sp] & 0x40));
            cpu->flags.s  = ((cpu->memory[cpu->sp] & 0x80));
            cpu->flags.p  = ((cpu->memory[cpu->sp] & 0x04));
            cpu->flags.cy = ((cpu->memory[cpu->sp] & 0x01));
            cpu->flags.ac = ((cpu->memory[cpu->sp] & 0x10));
            cpu->sp += 2;
            cpu->cycles -= 1;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 11;
    cpu->pc     += 1 ;
}

void emulate_INTERRUPT ( _cpu_info *cpu ) {
    if ( cpu->enable_interrupts ) {
        /*printf("INTERRUPT: %08x  -  ", cpu->interrupt_addr );*/
        /*print_registers(cpu);*/

        uint16_t ret           = cpu->pc;
        cpu->memory[cpu->sp-1] = (ret >> 8) & 0xff;
        cpu->memory[cpu->sp-2] = (ret & 0xff);
        cpu->sp                = cpu->sp - 2;

        cpu->pc = cpu->interrupt_addr;
    } else {
        // Do nothing
    }
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
            cpu->memory[cpu->sp-2] = ( cpu->flags.z  ? 0x40 : 0x0 ) |
                                     ( cpu->flags.s  ? 0x80 : 0x0 ) |
                                     ( cpu->flags.p  ? 0x04 : 0x0 ) |
                                     ( cpu->flags.cy ? 0x01 : 0x0 ) |
                                     ( cpu->flags.ac ? 0x10 : 0x0 ) |
                                     0x02;
            cpu->sp = cpu->sp - 2;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 11;
    cpu->pc     += 1 ;
}

void emulate_INR ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t answer = 0;

    switch ( *opcode ) {
        case 0x04: // INR B
            answer = cpu->b + 1;
            cpu->b = answer & 0xff;
            cpu->flags.ac = (answer & 0x0f) == 0x00;
            break;
        case 0x0c: // INR C
            answer = cpu->c + 1;
            cpu->c = answer & 0xff;
            cpu->flags.ac = (answer & 0x0f) == 0x00;
            break;
        case 0x14: // INR D
            answer = cpu->d + 1;
            cpu->d = answer & 0xff;
            cpu->flags.ac = (answer & 0x0f) == 0x00;
            break;
        case 0x1c: // INR E
            answer = cpu->e + 1;
            cpu->e = answer & 0xff;
            cpu->flags.ac = (answer & 0x0f) == 0x00;
            break;
        case 0x24: // INR H
            answer = cpu->h + 1;
            cpu->h = answer & 0xff;
            cpu->flags.ac = (answer & 0x0f) == 0x00;
            break;
        case 0x2c: // INR L
            answer = cpu->l + 1;
            cpu->l = answer & 0xff;
            cpu->flags.ac = (answer & 0x0f) == 0x00;
            break;
        case 0x34: // INR M
            cpu->memory[cpu->h << 8 | cpu->l] += 1;
            answer = cpu->memory[cpu->h << 8 | cpu->l];
            cpu->flags.ac = (answer & 0x0f) == 0x00;
            cpu->cycles += 5;
            break;
        case 0x3c: // INR A
            answer = cpu->a + 1;
            cpu->flags.ac = (answer & 0x0f) == 0x00;
            cpu->a = answer & 0xff;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
/*
 * main: emulator.c:1133: emulate_INR: Assertion `0 && "Code should not get here\n"' failed.
 */

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.s    = ( answer & 0x80 ) != 0;
    cpu->flags.p    = parity_bit ( answer & 0xff );

    cpu->cycles += 5 ;
    cpu->pc     += 1 ;
}

void emulate_INX ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0x03: // INX B
            cpu->c += 1;
            if ( cpu->c == 0 )
                cpu->b += 1;
            break;
        case 0x13: // INX D
            cpu->e += 1;
            if ( cpu->e == 0 )
                cpu->d += 1;
            break;
        case 0x23: // INX H
            cpu->l += 1;
            if ( cpu->l == 0 )
                cpu->h += 1;
            break;
        case 0x33: // INX SP
            cpu->sp += 1;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 5 ;
    cpu->pc     += 1 ;
}

void emulate_SUB ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t answer = (uint16_t) cpu->a;

    switch ( *opcode ) {
        case 0x90:
            answer -= (uint16_t) cpu->b;
            cpu->flags.ac   = !halfcary_sub( cpu->a, cpu->b, answer );
            break;
        case 0x91:
            answer -= (uint16_t) cpu->c;
            cpu->flags.ac   = !halfcary_sub( cpu->a, cpu->c, answer );
            break;
        case 0x92:
            answer -= (uint16_t) cpu->d;
            cpu->flags.ac   = !halfcary_sub( cpu->a, cpu->d, answer );
            break;
        case 0x93:
            answer -= (uint16_t) cpu->e;
            cpu->flags.ac   = !halfcary_sub( cpu->a, cpu->e, answer );
            break;
        case 0x94:
            answer -= (uint16_t) cpu->h;
            cpu->flags.ac   = !halfcary_sub( cpu->a, cpu->h, answer );
            break;
        case 0x95:
            answer -= (uint16_t) cpu->l;
            cpu->flags.ac   = !halfcary_sub( cpu->a, cpu->l, answer );
            break;
        case 0x96:
            answer -= (uint16_t) cpu->memory[(cpu->h<<8) | (cpu->l)];
            cpu->flags.ac   = !halfcary_sub( cpu->a, cpu->memory[(cpu->h<<8) | (cpu->l)], answer );
            cpu->cycles += 3;
            break;
        case 0x97:
            answer -= (uint16_t) cpu->a;
            cpu->flags.ac   = !halfcary_sub( cpu->a, cpu->a, answer );
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.s    = ( answer & 0x80 ) != 0;
    cpu->flags.cy   = ( answer > 0xff );
    cpu->flags.p    = parity_bit ( answer & 0xff );

    cpu->a          = answer & 0xff;

    cpu->pc        += 1 ;
    cpu->cycles    += 4 ;
}

void emulate_ADD ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t answer = (uint16_t) cpu->a;

    switch ( *opcode ) {
        case 0x80:
            answer += (uint16_t) cpu->b;
            cpu->flags.ac   = halfcary( cpu->a, cpu->b, answer );
            break;
        case 0x81:
            answer += (uint16_t) cpu->c;
            cpu->flags.ac   = halfcary( cpu->a, cpu->c, answer );
            break;
        case 0x82:
            answer += (uint16_t) cpu->d;
            cpu->flags.ac   = halfcary( cpu->a, cpu->d, answer );
            break;
        case 0x83:
            answer += (uint16_t) cpu->e;
            cpu->flags.ac   = halfcary( cpu->a, cpu->e, answer );
            break;
        case 0x84:
            answer += (uint16_t) cpu->h;
            cpu->flags.ac   = halfcary( cpu->a, cpu->h, answer );
            break;
        case 0x85:
            answer += (uint16_t) cpu->l;
            cpu->flags.ac   = halfcary( cpu->a, cpu->l, answer );
            break;
        case 0x86:
            answer += (uint16_t) cpu->memory[(cpu->h<<8) | (cpu->l)];
            cpu->flags.ac   = halfcary( cpu->a, cpu->memory[(cpu->h<<8) | (cpu->l)], answer );
            cpu->cycles += 3;
            break;
        case 0x87:
            answer += (uint16_t) cpu->a;
            cpu->flags.ac   = halfcary( cpu->a, cpu->a, answer );
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.s    = ( answer & 0x80 ) != 0;
    cpu->flags.cy   = ( answer > 0xff );
    cpu->flags.p    = parity_bit ( answer & 0xff );

    cpu->a          = answer & 0xff;

    cpu->pc        += 1 ;
    cpu->cycles    += 4 ;
}

void emulate_NOP ( _cpu_info *cpu ) {
    cpu->cycles += 4 ;
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

    cpu->cycles += 5;
    cpu->pc     += 1 ;
}

void emulate_SHLD ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0x22: // SHLD D16
            {
            uint16_t addr = opcode[2] << 8 | opcode[1];
            cpu->memory[addr+0] = cpu->l;
            cpu->memory[addr+1] = cpu->h;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 16;
    cpu->pc     += 3 ;
}

void emulate_PCHL ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xe9: // PCHL
            cpu->pc = cpu->h << 8 | cpu->l;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 5 ;
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

    cpu->cycles += 18;
    cpu->pc     += 1 ;
}

void emulate_LHLD ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0x2a: // LHLD B
            cpu->l = cpu->memory[(opcode[2] << 8 | opcode[1]) + 0];
            cpu->h = cpu->memory[(opcode[2] << 8 | opcode[1]) + 1];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 16;
    cpu->pc     += 3 ;
}

void emulate_LXI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0x01: // LXI B
            cpu->b = opcode[2];
            cpu->c = opcode[1];
            break;
        case 0x11: // LXI D
            cpu->d = opcode[2];
            cpu->e = opcode[1];
            break;
        case 0x21: // LXI H
            cpu->h = opcode[2];
            cpu->l = opcode[1];
            break;
        case 0x31: // LXI SP
            cpu->sp = opcode[2] << 8 | opcode[1];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10;
    cpu->pc     += 3 ;
}

void emulate_MVI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0x06: // MVI B, D8
            cpu->b = opcode[1];
            break;
        case 0x0e: // MVI C, D8
            cpu->c = opcode[1];
            break;
        case 0x16: // MVI D, D8
            cpu->d = opcode[1];
            break;
        case 0x1e: // MVI E, D8
            cpu->e = opcode[1];
            break;
        case 0x26: // MVI H, D8
            cpu->h = opcode[1];
            break;
        case 0x2e: // MVI L, D8
            cpu->l = opcode[1];
            break;
        case 0x36: // MVI M, D8
            cpu->memory[cpu->h << 8 | cpu->l] = opcode[1];
            cpu->cycles += 3;
            break;
        case 0x3e: // MVI A, D8
            cpu->a = opcode[1];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
}

void emulate_RNC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xd0: // RNC
            if ( !cpu->flags.cy ) {
                addr = cpu->memory[cpu->sp+1] << 8 | cpu->memory[cpu->sp];
                cpu->sp += 2;
                cpu->pc = addr;
                cpu->cycles += 6;
            } else {
                cpu->pc += 1;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 5;
    /*cpu->pc     += 1 ;*/
}

void emulate_RP ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xf0:
            if ( cpu->flags.p ) {
                addr = cpu->memory[cpu->sp+1] << 8 | cpu->memory[cpu->sp];
                cpu->sp += 2;
                cpu->pc = addr;
                cpu->cycles += 6;
            } else {
                cpu->pc += 1;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 5;
    /*cpu->pc     += 1 ;*/
}

void emulate_RM ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xf8:
            if ( cpu->flags.s ) {
                addr = cpu->memory[cpu->sp+1] << 8 | cpu->memory[cpu->sp];
                cpu->sp += 2;
                cpu->pc = addr;
                cpu->cycles += 6;
            } else {
                cpu->pc += 1;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 5;
    /*cpu->pc     += 1 ;*/
}

void emulate_RC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xd8:
            if ( cpu->flags.cy ) {
                addr = cpu->memory[cpu->sp+1] << 8 | cpu->memory[cpu->sp];
                cpu->sp += 2;
                cpu->pc = addr;
                cpu->cycles += 6;
            } else {
                cpu->pc += 1;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 5;
    /*cpu->pc     += 1 ;*/
}

void emulate_RPO ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xe0:
            if ( !cpu->flags.p ) {
                addr = cpu->memory[cpu->sp+1] << 8 | cpu->memory[cpu->sp];
                cpu->sp += 2;
                cpu->pc = addr;
                cpu->cycles += 6;
            } else {
                cpu->pc += 1;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 5;
}

void emulate_RPE ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xe8:
            if ( cpu->flags.p ) {
                addr = cpu->memory[cpu->sp+1] << 8 | cpu->memory[cpu->sp];
                cpu->sp += 2;
                cpu->pc = addr;
                cpu->cycles += 6;
            } else {
                cpu->pc += 1;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 5;
}

void emulate_RNZ ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xc0:
            if ( !cpu->flags.z ) {
                addr = cpu->memory[cpu->sp+1] << 8 | cpu->memory[cpu->sp];
                cpu->sp += 2;
                cpu->pc = addr;
                cpu->cycles += 6;
            } else {
                cpu->pc += 1;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 5;
    /*cpu->pc     += 1 ;*/
}

void emulate_RZ ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xc8:
            if ( cpu->flags.z ) {
                addr = cpu->memory[cpu->sp+1] << 8 | cpu->memory[cpu->sp];
                cpu->sp += 2;
                cpu->pc = addr;
                cpu->cycles += 6;
            } else {
                cpu->pc += 1;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 5;
    /*cpu->pc     += 1 ;*/
}

void emulate_RET ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xc9:
            addr = cpu->memory[cpu->sp+1] << 8 | cpu->memory[cpu->sp];
            cpu->sp += 2;
            cpu->pc = addr;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10;
    /*cpu->pc     += 1 ;*/
}

uint8_t *get_reg_ref ( _cpu_info *cpu, uint8_t opcode ) {
    /*printf("reg = "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(opcode));*/

    switch ( opcode ) {
        case 0x00: // B
            /*printf(" b\n");*/
            return &(cpu->b);
            break;
        case 0x01: // C
            /*printf(" c\n");*/
            return &(cpu->c);
            break;
        case 0x02: // D
            /*printf(" d\n");*/
            return &(cpu->d);
            break;
        case 0x03: // E
            /*printf(" e\n");*/
            return &(cpu->e);
            break;
        case 0x04: // H
            /*printf(" h\n");*/
            return &(cpu->h);
            break;
        case 0x05: // L
            /*printf(" l\n");*/
            return &(cpu->l);
            break;
        case 0x07: // A
            /*printf(" a\n");*/
            return &(cpu->a);
            break;
        default:
            assert( 0 && "Invalid OPCODE for MOV");
    }
    return NULL;
}

void emulate_MOV ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0x46: // MOV B, M
            addr = cpu->h << 8 | cpu->l;
            cpu->b = cpu->memory[addr];
            cpu->cycles += 2;
            break;
        case 0x56: // MOV D, M
            addr = cpu->h << 8 | cpu->l;
            cpu->d = cpu->memory[addr];
            cpu->cycles += 2;
            break;
        case 0x66: // MOV H, M
            addr = cpu->h << 8 | cpu->l;
            cpu->h = cpu->memory[addr];
            cpu->cycles += 2;
            break;

        case 0x70: // MOV M, B
            addr = cpu->h << 8 | cpu->l;
            cpu->memory[addr] = cpu->b;
            cpu->cycles += 2;
            break;
        case 0x71: // MOV M, C
            addr = cpu->h << 8 | cpu->l;
            cpu->memory[addr] = cpu->c;
            cpu->cycles += 2;
            break;
        case 0x72: // MOV M, D
            addr = cpu->h << 8 | cpu->l;
            cpu->memory[addr] = cpu->d;
            cpu->cycles += 2;
            break;
        case 0x73: // MOV M, E
            addr = cpu->h << 8 | cpu->l;
            cpu->memory[addr] = cpu->e;
            cpu->cycles += 2;
            break;
        case 0x74: // MOV M, H
            addr = cpu->h << 8 | cpu->l;
            cpu->memory[addr] = cpu->h;
            cpu->cycles += 2;
            break;
        case 0x75: // MOV M, L
            addr = cpu->h << 8 | cpu->l;
            cpu->memory[addr] = cpu->l;
            cpu->cycles += 2;
            break;
      /*case 0x76: // HLT*/
        case 0x77: // MOV M, A
            addr = cpu->h << 8 | cpu->l;
            cpu->memory[addr] = cpu->a;
            cpu->cycles += 2;
            break;

        case 0x4e: // MOV C, M
            addr = cpu->h << 8 | cpu->l;
            cpu->c = cpu->memory[addr];
            cpu->cycles += 2;
            break;
        case 0x5e: // MOV E, M
            addr = cpu->h << 8 | cpu->l;
            cpu->e = cpu->memory[addr];
            cpu->cycles += 2;
            break;
        case 0x6e: // MOV L, M
            addr = cpu->h << 8 | cpu->l;
            cpu->l = cpu->memory[addr];
            cpu->cycles += 2;
            break;
        case 0x7e: // MOV A, M
            addr = cpu->h << 8 | cpu->l;
            cpu->a = cpu->memory[addr];
            cpu->cycles += 2;
            break;
        default:
            {
                /*printf("dst = ");*/
                uint8_t *dst = get_reg_ref( cpu, ( *opcode & 0x38 ) >> 3 );
                /*printf("src = ");*/
                uint8_t *src = get_reg_ref( cpu, ( *opcode & 0x07 ) >> 0 );
                *dst = *src;
            }
            break;
    }

    cpu->cycles += 5 ;
    cpu->pc     += 1 ;
}

void emulate_STA ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0x32:
            addr = opcode[2] << 8 | opcode[1];
            cpu->memory[addr] = cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 13;
    cpu->pc     += 3 ;
}

void emulate_LDA ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0x3a:
            addr = opcode[2] << 8 | opcode[1];
            cpu->a = cpu->memory[addr];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 13;
    cpu->pc     += 3;
}

void emulate_LDAX ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0x0a: // LDAX B
            addr = cpu->b << 8 | cpu->c;
            cpu->a = cpu->memory[addr];
            break;
        case 0x1a: // LDAX D
            addr = cpu->d << 8 | cpu->e;
            cpu->a = cpu->memory[addr];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 7 ;
    cpu->pc     += 1 ;
}

void emulate_JM ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xfa: // JM
            addr = opcode[2] << 8 | opcode[1];
            if ( cpu->flags.s ) {
                cpu->pc = addr;
                /*cpu->cycles += 5;*/
            } else {
                cpu->pc += 3;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10;
}

void emulate_JZ ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xca: // JZ
            addr = opcode[2] << 8 | opcode[1];
            if ( cpu->flags.z ) {
                cpu->pc = addr;
            } else {
                cpu->pc += 3;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10;
}

void emulate_JPO ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xe2: // JPO
            addr = opcode[2] << 8 | opcode[1];
            if ( !cpu->flags.p ) {
                cpu->pc = addr;
            } else {
                cpu->pc += 3;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10;
}

void emulate_JPE ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xea: // JPE
            addr = opcode[2] << 8 | opcode[1];
            if ( cpu->flags.p ) {
                cpu->pc = addr;
            } else {
                cpu->pc += 3;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10;
}

void emulate_JNZ ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xc2: // JNZ
            addr = opcode[2] << 8 | opcode[1];
            if ( !cpu->flags.z ) {
                cpu->pc = addr;
            } else {
                cpu->pc += 3;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10;
}

void emulate_JNC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xd2: // JNC
            if ( !cpu->flags.cy ) {
                addr = opcode[2] << 8 | opcode[1];
                cpu->pc = addr;
            } else {
                cpu->pc += 3;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10  ;
}

void emulate_JP ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xf2: // JP
            if ( cpu->flags.p ) {
                addr = opcode[2] << 8 | opcode[1];
                cpu->pc = addr;
            } else {
                cpu->pc += 3;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10  ;
}

void emulate_JC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xda: // JC
            if ( cpu->flags.cy ) {
                addr = opcode[2] << 8 | opcode[1];
                cpu->pc = addr;
            } else {
                cpu->pc += 3;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10  ;
}

void emulate_JMP ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xc3:
            addr = opcode[2] << 8 | opcode[1];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10  ;
    cpu->pc      = addr;
}

void emulate_CPO ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xe4:
            {
                if ( !cpu->flags.p ) {
                    uint16_t ret           = cpu->pc + 3;
                    cpu->memory[cpu->sp-1] = (ret >> 8) & 0xff;
                    cpu->memory[cpu->sp-2] = (ret & 0xff);
                    cpu->sp                = cpu->sp - 2;
                    cpu->pc                = opcode[2] << 8 | opcode[1];
                    cpu->cycles += 17;
                } else {
                    cpu->pc     += 3;
                    cpu->cycles += 11;
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

void emulate_CPE ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xec:
            {
                if ( cpu->flags.p ) {
                    uint16_t ret           = cpu->pc + 3;
                    cpu->memory[cpu->sp-1] = (ret >> 8) & 0xff;
                    cpu->memory[cpu->sp-2] = (ret & 0xff);
                    cpu->sp                = cpu->sp - 2;
                    cpu->pc                = opcode[2] << 8 | opcode[1];
                    cpu->cycles += 17;
                } else {
                    cpu->pc     += 3;
                    cpu->cycles += 11;
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

void emulate_CNC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xd4:
            {
                if ( !cpu->flags.cy ) {
                    uint16_t ret           = cpu->pc + 3;
                    cpu->memory[cpu->sp-1] = (ret >> 8) & 0xff;
                    cpu->memory[cpu->sp-2] = (ret & 0xff);
                    cpu->sp                = cpu->sp - 2;
                    cpu->pc                = opcode[2] << 8 | opcode[1];
                    cpu->cycles += 17;
                } else {
                    cpu->pc     += 3;
                    cpu->cycles += 11;
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

void emulate_CP ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xf4:
            {
                if ( cpu->flags.p ) {
                    uint16_t ret           = cpu->pc + 3;
                    cpu->memory[cpu->sp-1] = (ret >> 8) & 0xff;
                    cpu->memory[cpu->sp-2] = (ret & 0xff);
                    cpu->sp                = cpu->sp - 2;
                    cpu->pc                = opcode[2] << 8 | opcode[1];
                    cpu->cycles += 17;
                } else {
                    cpu->pc     += 3;
                    cpu->cycles += 11;
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

void emulate_CM ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xfc:
            {
                if ( cpu->flags.s ) {
                    uint16_t ret           = cpu->pc + 3;
                    cpu->memory[cpu->sp-1] = (ret >> 8) & 0xff;
                    cpu->memory[cpu->sp-2] = (ret & 0xff);
                    cpu->sp                = cpu->sp - 2;
                    cpu->pc                = opcode[2] << 8 | opcode[1];
                    cpu->cycles += 17;
                } else {
                    cpu->pc     += 3;
                    cpu->cycles += 11;
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

void emulate_CC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xdc:
            {
                if ( cpu->flags.cy ) {
                    uint16_t ret           = cpu->pc + 3;
                    cpu->memory[cpu->sp-1] = (ret >> 8) & 0xff;
                    cpu->memory[cpu->sp-2] = (ret & 0xff);
                    cpu->sp                = cpu->sp - 2;
                    cpu->pc                = opcode[2] << 8 | opcode[1];
                    cpu->cycles += 17;
                } else {
                    cpu->pc     += 3;
                    cpu->cycles += 11;
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

void emulate_CNZ ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xc4: // CNZ
            {
                if ( !cpu->flags.z ) {
                    uint16_t ret           = cpu->pc + 3;
                    cpu->memory[cpu->sp-1] = (ret >> 8) & 0xff;
                    cpu->memory[cpu->sp-2] = (ret & 0xff);
                    cpu->sp                = cpu->sp - 2;
                    cpu->pc                = opcode[2] << 8 | opcode[1];
                    cpu->cycles += 17;
                } else {
                    cpu->pc     += 3;
                    cpu->cycles += 11;
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

void emulate_CZ ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xcc: //CZ
            {
                if ( cpu->flags.z ) {
                    uint16_t ret           = cpu->pc + 3;
                    cpu->memory[cpu->sp-1] = (ret >> 8) & 0xff;
                    cpu->memory[cpu->sp-2] = (ret & 0xff);
                    cpu->sp                = cpu->sp - 2;
                    cpu->pc                = opcode[2] << 8 | opcode[1];
                    cpu->cycles += 17;
                } else {
                    cpu->pc     += 3;
                    cpu->cycles += 11;
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

void emulate_CALL ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xcd:
            {
                /*assert ( cpu->sp >= 2 && "Got a segfault in the 8080" );*/
                uint16_t ret           = cpu->pc + 3;
                cpu->memory[(cpu->sp-1) & 0xffff] = (ret >> 8) & 0xff;
                cpu->memory[(cpu->sp-2) & 0xffff] = (ret & 0xff);
                cpu->sp                = cpu->sp - 2;
                cpu->pc                = opcode[2] << 8 | opcode[1];
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 17;
}

unsigned short int emulator( _cpu_info *cpu ) {
    unsigned short int op_size = 1;

    unsigned char *opcode = &cpu->memory[cpu->pc];

#ifdef __show_step
    disassembler ( cpu->memory, cpu->pc );
    print_registers(cpu);
#endif

           if ( *opcode == 0x00 ) {
        emulate_NOP ( cpu );
    } else if ( *opcode == 0x05 || *opcode == 0x0d || *opcode == 0x15 || *opcode == 0x1d || *opcode == 0x25 || *opcode == 0x2d || *opcode == 0x35 || *opcode == 0x3d) {
        emulate_DCR ( cpu );
    } else if ( *opcode == 0x27 ) {
        emulate_DAA ( cpu );
    } else if ( *opcode == 0xd6 ) {
        emulate_SUI ( cpu );
    } else if ( *opcode == 0x1f ) {
        emulate_RAR ( cpu );
    } else if ( *opcode == 0x17 ) {
        emulate_RAL ( cpu );
    } else if ( *opcode == 0x07 ) {
        emulate_RLC ( cpu );
    } else if ( *opcode == 0x0f ) {
        emulate_RRC ( cpu );
    } else if ( *opcode == 0x3f ) {
        emulate_CMC ( cpu );
    } else if ( *opcode == 0x2f ) {
        emulate_CMA ( cpu );
    } else if ( *opcode == 0x32 ) {
        emulate_STA ( cpu );
    } else if ( *opcode == 0x37 ) {
        emulate_STC ( cpu );
    } else if ( *opcode == 0x3a ) {
        emulate_LDA ( cpu );
    } else if ( *opcode == 0x1a || *opcode == 0x0a ) {
        emulate_LDAX ( cpu );
    } else if ( *opcode == 0x01 || *opcode == 0x11 || *opcode == 0x21 || *opcode == 0x31 ) {
        emulate_LXI ( cpu );
    } else if ( *opcode == 0x03 || *opcode == 0x13 || *opcode == 0x23 || *opcode == 0x33 ) {
        emulate_INX ( cpu );
    } else if ( *opcode == 0x09 || *opcode == 0x19 || *opcode == 0x29 || *opcode == 0x39 ) {
        emulate_DAD ( cpu );
    } else if ( *opcode == 0x04 || *opcode == 0x0c || *opcode == 0x14 || *opcode == 0x1c || *opcode == 0x24 || *opcode == 0x2c || *opcode == 0x34 || *opcode == 0x3c ) {
        emulate_INR ( cpu );
    } else if ( *opcode == 0x3e || *opcode == 0x36 || *opcode == 0x26 || *opcode == 0x2e || *opcode == 0x06 || *opcode == 0x0e || *opcode == 0x16 || *opcode == 0x1e ) {
        emulate_MVI ( cpu );
    } else if ( ( *opcode >= 0x40 && *opcode <= 0x75 ) || ( *opcode >= 0x77 && *opcode <= 0x7f ) ) {
        emulate_MOV ( cpu );
    } else if ( *opcode >= 0x80 && *opcode <= 0x87 ) {
        emulate_ADD ( cpu );
    } else if ( *opcode >= 0x90 && *opcode <= 0x97 ) {
        emulate_SUB ( cpu );
    } else if ( *opcode >= 0x88 && *opcode <= 0x8f ) {
        emulate_ADC ( cpu );
    } else if ( *opcode == 0xde ) {
        emulate_SBI ( cpu );
    } else if ( *opcode >= 0x98 && *opcode <= 0x9f ) {
        emulate_SBB ( cpu );
    } else if ( *opcode == 0xfa ) {
        emulate_JM ( cpu );
    } else if ( *opcode == 0xca ) {
        emulate_JZ ( cpu );
    } else if ( *opcode == 0xe2 ) {
        emulate_JPO ( cpu );
    } else if ( *opcode == 0xea ) {
        emulate_JPE ( cpu );
    } else if ( *opcode == 0xc2 ) {
        emulate_JNZ ( cpu );
    } else if ( *opcode == 0xc3 ) {
        emulate_JMP ( cpu );
    } else if ( *opcode == 0xd2 ) {
        emulate_JNC ( cpu );
    } else if ( *opcode == 0xf2 ) {
        emulate_JP ( cpu );
    } else if ( *opcode == 0xda ) {
        emulate_JC ( cpu );
    } else if ( *opcode == 0xce ) {
        emulate_ACI ( cpu );
    } else if ( *opcode == 0xc6 ) {
        emulate_ADI ( cpu );
    } else if ( *opcode == 0xd0 ) {
        emulate_RNC ( cpu );
    } else if ( *opcode == 0xf0 ) {
        emulate_RP ( cpu );
    } else if ( *opcode == 0xd8 ) {
        emulate_RC ( cpu );
    } else if ( *opcode == 0xf8 ) {
        emulate_RM ( cpu );
    } else if ( *opcode == 0xe0 ) {
        emulate_RPO ( cpu );
    } else if ( *opcode == 0xe8 ) {
        emulate_RPE ( cpu );
    } else if ( *opcode == 0xc0 ) {
        emulate_RNZ ( cpu );
    } else if ( *opcode == 0xc8 ) {
        emulate_RZ ( cpu );
    } else if ( *opcode == 0xc9 ) {
        emulate_RET ( cpu );
    } else if ( *opcode == 0xe4 ) {
        emulate_CPO ( cpu );
    } else if ( *opcode == 0xec ) {
        emulate_CPE ( cpu );
    } else if ( *opcode == 0xd4 ) {
        emulate_CNC ( cpu );
    } else if ( *opcode == 0xf4 ) {
        emulate_CP ( cpu );
    } else if ( *opcode == 0xfc ) {
        emulate_CM ( cpu );
    } else if ( *opcode == 0xdc ) {
        emulate_CC ( cpu );
    } else if ( *opcode == 0xc4 ) {
        emulate_CNZ ( cpu );
    } else if ( *opcode == 0xcc ) {
        emulate_CZ ( cpu );
    } else if ( *opcode == 0xcd ) {
        emulate_CALL ( cpu );
    } else if ( *opcode == 0xe1 || *opcode == 0xd1 || *opcode == 0xc1 || *opcode == 0xf1 ) {
        emulate_POP ( cpu );
    } else if ( *opcode == 0x0b || *opcode == 0x1b || *opcode == 0x2b || *opcode == 0x3b ) {
        emulate_DCX ( cpu );
    } else if ( *opcode == 0xc5 || *opcode == 0xd5 || *opcode == 0xe5 || *opcode == 0xf5 ) {
        emulate_PUSH ( cpu );
    } else if ( *opcode == 0xdb ) {
        emulate_IN ( cpu );
    } else if ( *opcode == 0xd3 ) {
        emulate_OUT ( cpu );
    } else if ( *opcode == 0xee ) {
        emulate_XRI ( cpu );
    } else if ( *opcode >= 0xa8 && *opcode <= 0xaf ) {
        emulate_XRA ( cpu );
    } else if ( *opcode == 0xf6 ) {
        emulate_ORI ( cpu );
    } else if ( *opcode >= 0xb0 && *opcode <= 0xb7 ) {
        emulate_ORA ( cpu );
    } else if ( *opcode >= 0xb8 && *opcode <= 0xbf ) {
        emulate_CMP ( cpu );
    } else if ( *opcode >= 0xa0 && *opcode <= 0xa7 ) {
        emulate_ANA ( cpu );
    } else if ( *opcode == 0xe6 ) {
        emulate_ANI ( cpu );
    } else if ( *opcode == 0xeb ) {
        emulate_XCHG ( cpu );
    } else if ( *opcode == 0xf3 ) {
        emulate_DI ( cpu );
    } else if ( *opcode == 0xfb ) {
        emulate_EI ( cpu );
    } else if ( *opcode == 0xfe ) {
        emulate_CPI ( cpu );
    } else if ( *opcode == 0xf9 ) {
        emulate_SPHL ( cpu );
    } else if ( *opcode == 0xe9 ) {
        emulate_PCHL ( cpu );
    } else if ( *opcode == 0xe3 ) {
        emulate_XTHL ( cpu );
    } else if ( *opcode == 0x22 ) {
        emulate_SHLD ( cpu );
    } else if ( *opcode == 0x2a ) {
        emulate_LHLD ( cpu );
    } else if ( *opcode == 0x02 || *opcode == 0x12 ) {
        emulate_STAX ( cpu );
    } else {
        disassembler ( cpu->memory, cpu->pc );
        print_registers(cpu);
        printf(" %2X is not implemented\n", cpu->memory[cpu->pc]);
        exit(-1);
    }

    cpu->instructions_executed += 1;

#ifdef __show_step
    print_registers(cpu);
    puts("");
#endif

    return op_size;
}



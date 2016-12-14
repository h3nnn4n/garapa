#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "types.h"
#include "utils.h"
#include "halfcary.h"

#include "instructions_arithmetic.h"

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

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.s    = ( answer & 0x80 ) != 0;
    cpu->flags.cy   = ( answer > 0xff );
    cpu->flags.p    = parity_bit ( answer & 0xff );
    cpu->flags.ac   = !halfcary_sub( cpu->a, opcode[1], answer );

    cpu->a = answer & 0xff;

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

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.s    = ( answer & 0x80 ) != 0;
    cpu->flags.cy   = ( answer > 0xff );
    cpu->flags.p    = parity_bit ( answer & 0xff );
    cpu->a          = answer & 0xff;

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

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.s    = ( answer & 0x80 ) != 0;
    cpu->flags.p    = parity_bit ( answer & 0xff );

    cpu->cycles += 5 ;
    cpu->pc     += 1 ;
}

void emulate_DCR ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t answer = 0;

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

void emulate_DAA ( _cpu_info *cpu ) {
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

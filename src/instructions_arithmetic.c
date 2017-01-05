#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "types.h"
#include "utils.h"
#include "microcode.h"
#include "time_keeper.h"
#include "halfcarry.h"

#include "instructions_arithmetic.h"

void emulate_ADD ( _cpu_info *cpu ) {
    uint16_t answer = (uint16_t) cpu->a;

    switch ( cpu->opcode ) {
        case 0x80:
            answer += (uint16_t) cpu->b;
            cpu->flags.h    = halfcarry( cpu->a, cpu->b, answer );
            break;
        case 0x81:
            answer += (uint16_t) cpu->c;
            cpu->flags.h    = halfcarry( cpu->a, cpu->c, answer );
            break;
        case 0x82:
            answer += (uint16_t) cpu->d;
            cpu->flags.h    = halfcarry( cpu->a, cpu->d, answer );
            break;
        case 0x83:
            answer += (uint16_t) cpu->e;
            cpu->flags.h    = halfcarry( cpu->a, cpu->e, answer );
            break;
        case 0x84:
            answer += (uint16_t) cpu->h;
            cpu->flags.h    = halfcarry( cpu->a, cpu->h, answer );
            break;
        case 0x85:
            answer += (uint16_t) cpu->l;
            cpu->flags.h    = halfcarry( cpu->a, cpu->l, answer );
            break;
        case 0x86:
            answer += (uint16_t) read_byte_with_tick ( cpu, read_hl ( cpu ) );
            cpu->flags.h    = halfcarry( cpu->a, cpu->mem_controller.memory[ read_hl ( cpu ) ], answer );
            break;
        case 0x87:
            answer += (uint16_t) cpu->a;
            cpu->flags.h    = halfcarry( cpu->a, cpu->a, answer );
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.n    = 0;
    cpu->flags.c    = ( answer > 0xff );
    cpu->a          = answer & 0xff;
}

void emulate_ADI ( _cpu_info *cpu ) {
    uint16_t t;

    t = read_byte_at_pc ( cpu ) ;
    switch ( cpu->opcode ) {
        case 0xc6: // ADI
            cpu->flags.h  = halfcarry( cpu->a, t, cpu->a + t );
            cpu->flags.c  = (t + cpu->a) > 0xff;
            cpu->a       += (uint8_t) t;
            cpu->flags.z  = (cpu->a == 0);
            break;
        case 0xe8:
            cpu->flags.h  = halfcarry( cpu->sp, t, cpu->sp + (int8_t) t );
            cpu->flags.c  = ((cpu->sp + (int8_t)t) & 0xff) < (cpu->sp & 0xff);
            t             = cpu->sp + (int8_t) t;
            timer_tick_and_full_mcycle ( cpu );
            timer_tick_and_full_mcycle ( cpu );
            cpu->sp       = t;
            cpu->flags.z  = 0;
            break;
        case 0xf8:
            cpu->flags.h  = halfcarry( cpu->sp, t, cpu->sp + (int8_t) t );
            t             = cpu->sp + (int8_t) t;
            timer_tick_and_full_mcycle ( cpu );
            cpu->flags.c  = (t & 0xff) < (cpu->sp & 0xff);
            cpu->h        = (t & 0xff00) >> 8;
            cpu->l        = (t & 0x00ff) >> 0;
            cpu->flags.z  = 0;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.n  = 0;
}

void emulate_ADC ( _cpu_info *cpu ) {
    uint16_t answer = 0;
    uint8_t a, b;
    a = cpu->a;

    switch ( cpu->opcode ) {
        case 0x88: // ADC B
            answer = cpu->a + cpu->b + (cpu->flags.c != 0) ;
            cpu->a = answer & 0xff;
            b = cpu->b;
            break;
        case 0x89: // ADC C
            answer = cpu->a + cpu->c + (cpu->flags.c != 0) ;
            cpu->a = answer & 0xff;
            b = cpu->c;
            break;
        case 0x8a: // ADC D
            answer = cpu->a + cpu->d + (cpu->flags.c != 0) ;
            cpu->a = answer & 0xff;
            b = cpu->d;
            break;
        case 0x8b: // ADC E
            answer = cpu->a + cpu->e + (cpu->flags.c != 0) ;
            cpu->a = answer & 0xff;
            b = cpu->e;
            break;
        case 0x8c: // ADC H
            answer = cpu->a + cpu->h + (cpu->flags.c != 0) ;
            cpu->a = answer & 0xff;
            b = cpu->h;
            break;
        case 0x8d: // ADC L
            answer = cpu->a + cpu->l + (cpu->flags.c != 0) ;
            cpu->a = answer & 0xff;
            b = cpu->l;
            break;
        case 0x8e: // ADC M
            b = read_byte_with_tick ( cpu, read_hl ( cpu ) );
            answer = cpu->a + b + (cpu->flags.c != 0) ;
            answer = cpu->a + b + (cpu->flags.c != 0) ;
            cpu->a = answer & 0xff;
            break;
        case 0x8f: // ADC A
            answer = cpu->a + cpu->a + (cpu->flags.c != 0) ;
            cpu->a = answer & 0xff;
            b = cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.n    = 0;
    cpu->flags.c    = ( answer > 0xff );
    cpu->flags.h    = halfcarry( a, b, answer );
}

void emulate_ACI ( _cpu_info *cpu ) {
    uint16_t t;

    switch ( cpu->opcode ) {
        case 0xce: // ACI
            t             = read_byte_at_pc ( cpu );
            cpu->flags.h  = (((cpu->a & 0x0f) + (t & 0x0f) + (cpu->flags.c != 0)) >= 0x10);
            t            += cpu->a + (cpu->flags.c != 0);
            cpu->a        = t & 0xff;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.c  = (t > 0xff);
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.n  = 0;
}

void emulate_SUB ( _cpu_info *cpu ) {
    uint16_t answer = (uint16_t) cpu->a;

    switch ( cpu->opcode ) {
        case 0x90:
            answer -= (uint16_t) cpu->b;
            break;
        case 0x91:
            answer -= (uint16_t) cpu->c;
            break;
        case 0x92:
            answer -= (uint16_t) cpu->d;
            break;
        case 0x93:
            answer -= (uint16_t) cpu->e;
            break;
        case 0x94:
            answer -= (uint16_t) cpu->h;
            break;
        case 0x95:
            answer -= (uint16_t) cpu->l;
            break;
        case 0x96:
            answer -= (uint16_t) read_byte_with_tick ( cpu, read_hl ( cpu ));
            break;
        case 0x97:
            answer -= (uint16_t) cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.n    = 1;
    cpu->flags.c    = ( answer > 0xff );
    cpu->flags.h    = (answer & 0x0f) > (cpu->a & 0x0f);

    cpu->a          = answer & 0xff;
}

void emulate_SUI ( _cpu_info *cpu ) {
    uint16_t answer = 0;
    uint16_t operand = 0;

    switch ( cpu->opcode ) {
        case 0xd6: // SUI D8
            operand = read_byte_at_pc ( cpu );
            answer = cpu->a - operand;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.n    = 1;
    cpu->flags.h    = halfcarry_sub( cpu->a, operand, answer );
    cpu->flags.c    = ( answer > 0xff );

    cpu->a = answer & 0xff;
}

void emulate_SBB ( _cpu_info *cpu ) {
    uint16_t answer = 0;
    uint16_t    old = 0;

    switch ( cpu->opcode ) {
        case 0x98:
            answer = cpu->a - cpu->b - (cpu->flags.c != 0) ;
            old    = cpu->b;
            break;
        case 0x99:
            answer = cpu->a - cpu->c - (cpu->flags.c != 0) ;
            old    = cpu->c;
            break;
        case 0x9a:
            answer = cpu->a - cpu->d - (cpu->flags.c != 0) ;
            old    = cpu->d;
            break;
        case 0x9b:
            answer = cpu->a - cpu->e - (cpu->flags.c != 0) ;
            old    = cpu->e;
            break;
        case 0x9c:
            answer = cpu->a - cpu->h - (cpu->flags.c != 0) ;
            old    = cpu->h;
            break;
        case 0x9d:
            answer = cpu->a - cpu->l - (cpu->flags.c != 0) ;
            old    = cpu->l;
            break;
        case 0x9e:
            {
            uint16_t addr = read_hl ( cpu );
            old    = read_byte_with_tick ( cpu, addr );
            answer = cpu->a - old - (cpu->flags.c != 0) ;
            }
            break;
        case 0x9f:
            answer = cpu->a - cpu->a - (cpu->flags.c != 0) ;
            old    = cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.n    = 1;
    cpu->flags.h    = ((cpu->a & 0x0f) - (cpu->flags.c != 0) - (old & 0x0f)) < 0;
    cpu->flags.c    = ( answer > 0xff );
    cpu->a          = answer & 0xff;
}

void emulate_SBI ( _cpu_info *cpu ) {
    uint16_t answer = 0;
    uint8_t  t = 0;

    switch ( cpu->opcode ) {
        case 0xde: // SBI D8
            t = read_byte_at_pc ( cpu );
            answer = cpu->a - t - (cpu->flags.c != 0) ;
            cpu->flags.h  = ((t & 0x0f) + (cpu->flags.c != 0)) > (cpu->a & 0x0f);
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.n    = 1;
    cpu->flags.c    = ( answer > 0xff );
    cpu->a          = answer & 0xff;
}

void emulate_INR ( _cpu_info *cpu ) {
    uint16_t answer = 0;

    switch ( cpu->opcode ) {
        case 0x04: // INR B
            answer = cpu->b + 1;
            cpu->b = answer & 0xff;
            cpu->flags.h  = (answer & 0x0f) == 0x00;
            break;
        case 0x0c: // INR C
            answer = cpu->c + 1;
            cpu->c = answer & 0xff;
            cpu->flags.h  = (answer & 0x0f) == 0x00;
            break;
        case 0x14: // INR D
            answer = cpu->d + 1;
            cpu->d = answer & 0xff;
            cpu->flags.h  = (answer & 0x0f) == 0x00;
            break;
        case 0x1c: // INR E
            answer = cpu->e + 1;
            cpu->e = answer & 0xff;
            cpu->flags.h  = (answer & 0x0f) == 0x00;
            break;
        case 0x24: // INR H
            answer = cpu->h + 1;
            cpu->h = answer & 0xff;
            cpu->flags.h  = (answer & 0x0f) == 0x00;
            break;
        case 0x2c: // INR L
            answer = cpu->l + 1;
            cpu->l = answer & 0xff;
            cpu->flags.h  = (answer & 0x0f) == 0x00;
            break;
        case 0x34: // INR M
            answer = read_byte_with_tick ( cpu, read_hl ( cpu ) );
            answer += 1;
            write_byte_with_tick ( cpu, read_hl ( cpu ), answer );
            cpu->flags.h  = (answer & 0x0f) == 0x00;
            break;
        case 0x3c: // INR A
            answer = cpu->a + 1;
            cpu->flags.h  = (answer & 0x0f) == 0x00;
            cpu->a = answer & 0xff;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.n    = 0;
}

void emulate_DCR ( _cpu_info *cpu ) {
    uint16_t answer = 0;

    switch ( cpu->opcode ) {
        case 0x05: // DCR B
            answer        = cpu->b - 1;
            cpu->b        = answer & 0xff;
            cpu->flags.h  = ((cpu->b & 0x0f) == 0x0f);
            break;
        case 0x0d: // DCR C
            answer        = cpu->c - 1;
            cpu->c        = answer & 0xff;
            cpu->flags.h  = ((cpu->c & 0x0f) == 0x0f);
            break;
        case 0x15: // DCR D
            answer        = cpu->d - 1;
            cpu->d        = answer & 0xff;
            cpu->flags.h  = ((cpu->d & 0x0f) == 0x0f);
            break;
        case 0x1d: // DCR E
            answer        = cpu->e - 1;
            cpu->e        = answer & 0xff;
            cpu->flags.h  = ((cpu->e & 0x0f) == 0x0f);
            break;
        case 0x25: // DCR H
            answer        = cpu->h - 1;
            cpu->h        = answer & 0xff;
            cpu->flags.h  = ((cpu->h & 0x0f) == 0x0f);
            break;
        case 0x2d: // DCR L
            answer        = cpu->l - 1;
            cpu->l        = answer & 0xff;
            cpu->flags.h  = ((cpu->l & 0x0f) == 0x0f);
            break;
        case 0x35: // DCR M
            answer = read_byte_with_tick ( cpu, read_hl ( cpu ) ) - 1;
            write_byte_with_tick ( cpu, read_hl ( cpu ), answer & 0xff );
            cpu->flags.h = ((answer & 0x0f) == 0x0f);
            break;
        case 0x3d: // DCR A
            answer        = cpu->a - 1;
            cpu->a        = answer & 0xff;
            cpu->flags.h  = ((cpu->a & 0x0f) == 0x0f);
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.n    = 1;
}

void emulate_INC ( _cpu_info *cpu ) {
    switch ( cpu->opcode ) {
        case 0x03: // INC B
            cpu->c += 1;
            if ( cpu->c == 0 )
                cpu->b += 1;
            break;
        case 0x13: // INC D
            cpu->e += 1;
            if ( cpu->e == 0 )
                cpu->d += 1;
            break;
        case 0x23: // INC H
            cpu->l += 1;
            if ( cpu->l == 0 )
                cpu->h += 1;
            break;
        case 0x33: // INC SP
            cpu->sp += 1;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    timer_tick_and_full_mcycle ( cpu );
}

void emulate_DCX ( _cpu_info *cpu ) {
    uint32_t       answer = 0;

    switch ( cpu->opcode ) {
        case 0x0b: // DCX B
            answer  = read_bc ( cpu );
            answer -= 1;
            write_bc_16 ( cpu, answer );
            break;
        case 0x1b: // DCX D
            answer  = read_de ( cpu );
            answer -= 1;
            write_de_16 ( cpu, answer );
            break;
        case 0x2b: // DCX H
            answer  = read_hl ( cpu );
            answer -= 1;
            write_hl_16 ( cpu, answer );
            break;
        case 0x3b: // DCX SP
            answer  = cpu->sp;
            answer -= 1;
            cpu->sp  = answer & 0xffff;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
    timer_tick_and_full_mcycle ( cpu );
}

void emulate_DAD ( _cpu_info *cpu ) {
    uint32_t answer = read_hl ( cpu );;

    switch ( cpu->opcode ) {
        case 0x09: // DAD B
            answer += read_bc ( cpu );
            break;
        case 0x19: // DAD D
            answer += read_de ( cpu );
            break;
        case 0x29: // DAD H
            answer += read_hl ( cpu );
            break;
        case 0x39: // DAD SP
            answer += cpu->sp;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    timer_tick_and_full_mcycle ( cpu );

    cpu->flags.h = ((answer & 0x7FF) < (read_hl ( cpu ) & 0x7FF));
    cpu->flags.c  = 0 < ( answer & 0xffff0000 );
    cpu->flags.n = 0;

    write_hl_16 ( cpu, answer );
}

void emulate_DAA ( _cpu_info *cpu ) {
    switch ( cpu->opcode ) {
        case 0x27: // DAA
            {
                uint16_t ans   = 0;

                ans = cpu->a;

                if(cpu->flags.n) {
                    if(cpu->flags.h) {
                        ans = (ans - 0x06) & 0xff;
                    }
                    if(cpu->flags.c) {
                        ans -= 0x60;
                    }
                } else {
                    if(cpu->flags.h || (ans & 0xf) > 9) {
                        ans += 0x06;
                    }
                    if(cpu->flags.c || ans > 0x9f) {
                        ans += 0x60;
                    }
                }

                cpu->a       = ans;
                cpu->flags.h = 0;
                cpu->flags.z = cpu->a == 0;

                if(ans >= 0x100) {
                    cpu->flags.c = 1;
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

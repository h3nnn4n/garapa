#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "types.h"
#include "utils.h"
#include "emulator.h"
#include "disassembler.h"
#include "halfcary.h"

uint8_t *get_reg_ref ( _cpu_info *cpu, uint8_t opcode ) {
    switch ( opcode ) {
        case 0x00: // B
            return &(cpu->b);
        case 0x01: // C
            return &(cpu->c);
        case 0x02: // D
            return &(cpu->d);
        case 0x03: // E
            return &(cpu->e);
        case 0x04: // H
            return &(cpu->h);
        case 0x05: // L
            return &(cpu->l);
        case 0x07: // A
            return &(cpu->a);
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
}

void emulate_INR ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t answer = 0;

    switch ( *opcode ) {
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
            cpu->memory[cpu->h << 8 | cpu->l] += 1;
            answer = cpu->memory[cpu->h << 8 | cpu->l];
            cpu->flags.h  = (answer & 0x0f) == 0x00;
            cpu->cycles += 5;
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
    cpu->flags.n    = ( answer & 0x80 ) != 0;

    cpu->cycles += 5 ;
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
            cpu->flags.n  = ((cpu->memory[cpu->sp] & 0x80));
            cpu->flags.c  = ((cpu->memory[cpu->sp] & 0x01));
            cpu->flags.h  = ((cpu->memory[cpu->sp] & 0x10));
            cpu->sp += 2;
            cpu->cycles -= 1;
            break;
        default:
            assert( 0 && "Code should not get here\n" ); }

    cpu->cycles += 11;
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
            cpu->memory[cpu->sp-2] = ( cpu->flags.z  ? 0x40 : 0x0 ) |
                                     ( cpu->flags.n  ? 0x80 : 0x0 ) |
                                     ( cpu->flags.c  ? 0x01 : 0x0 ) |
                                     ( cpu->flags.h  ? 0x10 : 0x0 ) |
                                     0x02;
            cpu->sp = cpu->sp - 2;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 11;
    cpu->pc     += 1 ;
}

void emulate_XOR ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
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
            cpu->a ^= cpu->memory[cpu->h << 8 | cpu->l];
            cpu->cycles += 3;
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
    cpu->flags.n  = 0; //(0x80 == (cpu->a & 0x80));

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_LD ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr   = 0;

    switch ( *opcode ) {
        /*case 0x01: // LXI B*/
            /*cpu->b = opcode[2];*/
            /*cpu->c = opcode[1];*/
            /*break;*/
        /*case 0x11: // LXI D*/
            /*cpu->d = opcode[2];*/
            /*cpu->e = opcode[1];*/
            /*break;*/
        case 0x21: // LD HL, d16
            cpu->h = opcode[2];
            cpu->l = opcode[1];
            break;
        case 0x31: // LD SP, d16
            cpu->sp = opcode[2] << 8 | opcode[1];
            break;
        case 0x32: // LD (HL-),A
            addr              = cpu->h << 8 | cpu->l;
            cpu->memory[addr] = cpu->a;
            addr              = cpu->h << 8 | cpu->l;
            addr             -= 1;
            cpu->h            = (addr >> 8 ) & 0xff;
            cpu->l            = (addr >> 0 ) & 0xff;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 12;
    cpu->pc     += 3 ;
}

void emulate_NOP ( _cpu_info *cpu ) {
    cpu->cycles += 1 ; // FIXME ?
    cpu->pc     += 1 ;
}

void emulate_JNZ ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0x20: // JZ
            addr = opcode[1];
            if ( !cpu->flags.z ) {
                cpu->pc = addr;
            } else {
                cpu->pc += 2;
                /*printf("\n00000000000000000000000000000000000000000000000\n");*/
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10;
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

void emulate_DCR ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t answer = 0;

    switch ( *opcode ) {
        case 0x05: // DCR B
            answer        = cpu->b - 1;
            cpu->b        = answer & 0xff;
            cpu->flags.h  = !((cpu->b & 0x0f) == 0x0f);
            break;
        case 0x0d: // DCR C
            answer        = cpu->c - 1;
            cpu->c        = answer & 0xff;
            cpu->flags.h  = !((cpu->c & 0x0f) == 0x0f);
            break;
        case 0x15: // DCR D
            answer        = cpu->d - 1;
            cpu->d        = answer & 0xff;
            cpu->flags.h  = !((cpu->d & 0x0f) == 0x0f);
            break;
        case 0x1d: // DCR E
            answer        = cpu->e - 1;
            cpu->e        = answer & 0xff;
            cpu->flags.h  = !((cpu->e & 0x0f) == 0x0f);
            break;
        case 0x25: // DCR H
            answer        = cpu->h - 1;
            cpu->h        = answer & 0xff;
            cpu->flags.h  = !((cpu->h & 0x0f) == 0x0f);
            break;
        case 0x2d: // DCR L
            answer        = cpu->l - 1;
            cpu->l        = answer & 0xff;
            cpu->flags.h  = !((cpu->l & 0x0f) == 0x0f);
            break;
        case 0x35: // DCR M
            answer                            = cpu->memory[cpu->h << 8 | cpu->l] - 1;
            cpu->memory[cpu->h << 8 | cpu->l] = answer & 0xff;
            cpu->flags.h                      = !((cpu->memory[cpu->h << 8 | cpu->l] & 0x0f) == 0x0f);
            cpu->cycles += 5;
            break;
        case 0x3d: // DCR A
            answer        = cpu->a - 1;
            cpu->a        = answer & 0xff;
            cpu->flags.h  = !((cpu->a & 0x0f) == 0x0f);
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0;
    cpu->flags.n    = ( answer & 0x80 ) != 0;

    cpu->cycles += 5 ;
    cpu->pc     += 1 ;
}

void emulate_INC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
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

    cpu->cycles += 5 ;
    cpu->pc     += 1 ;
}

void emulate_CP ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint8_t        answer = 0;

    switch ( *opcode ) {
        case 0xfe: // CP A
            answer        = cpu->a - opcode[1];
            cpu->flags.h  = !halfcary_sub( cpu->a, opcode[1], answer );
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0                  ; // Only the last 8 bits matters, hence the mask
    cpu->flags.n    = ( answer & 0x80 ) != 0       ; // Checks if the MSB is 1
    cpu->flags.c    = ( cpu->a < opcode[1] )       ; // Checks for the carry bit

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
}

void emulator( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

#ifdef __show_step
    disassembler ( cpu->memory, cpu->pc );
    print_registers(cpu);
#endif
    uint16_t addr;

    if ( ( *opcode >= 0x40 && *opcode <= 0x75 ) || ( *opcode >= 0x77 && *opcode <= 0x7f ) ) {
                emulate_MOV ( cpu );
    } else
    switch ( *opcode ) {
        case 0x00: emulate_NOP  ( cpu );
            break;
        case 0x0e:
            cpu->c = opcode[1];
            cpu->cycles += 8 ;
            cpu->pc     += 2 ;
            break;
        case 0x17:
            {
                uint8_t t;
                t = cpu->a;
                cpu->flags.c  = 0x80 == (t & 0x80);
                cpu->a = ( t << 1 ) | cpu->flags.c ;
                cpu->cycles += 4;
                cpu->pc     += 1;
            }
            break;
        case 0x0a:
        case 0x1a:
            emulate_LDAX ( cpu );
            break;
        case 0x20:
            emulate_JNZ  ( cpu );
            break;
        case 0x26:
            cpu->h = opcode[1];
            cpu->cycles += 8 ;
            cpu->pc     += 2 ;
            break;
        case 0x11:
            cpu->d = opcode[1];
            cpu->e = opcode[2];
            cpu->pc += 3;
            cpu->cycles += 12;
            break;
        case 0x0d:
        case 0x1d:
        case 0x2d:
        case 0x3d:
        case 0x05:
        case 0x15:
        case 0x25:
        case 0x35:
            emulate_DCR  ( cpu );
            break;
        case 0x03:
        case 0x13:
        case 0x23:
        case 0x33:
            emulate_INC  ( cpu );
            break;
        case 0xc1:
        case 0xd1:
        case 0xe1:
        case 0xf1:
            emulate_POP  ( cpu );
            break;
        case 0xc5:
        case 0xd5:
        case 0xe5:
        case 0xf5:
            emulate_PUSH ( cpu );
            break;
        case 0x21:
        case 0x31:
        case 0x32:
            emulate_LD  ( cpu );
            break;
        case 0x4f:
            cpu->c = cpu->a;
            cpu->cycles += 4 ;
            cpu->pc     += 1 ;
            break;
        case 0x06:
            cpu->b = opcode[1];
            cpu->cycles += 8 ;
            cpu->pc     += 2 ;
            break;
        case 0x3e:
            cpu->a = opcode[1];
            cpu->cycles += 8 ;
            cpu->pc     += 2 ;
            break;
        case 0xcd:
            emulate_CALL ( cpu );
            break;
        case 0xaf:
            emulate_XOR  ( cpu );
            break;
        case 0xe0:
            cpu->memory[0xff00 + opcode[1]] = cpu->a;
            cpu->cycles += 12;
            cpu->pc     += 2 ;
            break;
        case 0xe2:
            cpu->memory[0xff00 + cpu->c] = cpu->a;
            cpu->cycles += 8 ;
            cpu->pc     += 1 ;
            break;
        /*case 0xfe:*/
            /*cpu->memory[0xff00 + cpu->c] = cpu->a;*/
            /*cpu->cycles += 8 ;*/
            /*cpu->pc     += 1 ;*/
            /*break;*/
        case 0x77: // MOV M, A
            addr = cpu->h << 8 | cpu->l;
            cpu->memory[addr] = cpu->a;
            cpu->cycles += 8;
            cpu->pc     += 1 ;
            break;
        case 0xfe:
            emulate_CP ( cpu );
            break;
        case 0x0c:
        case 0x1c:
        case 0x2c:
        case 0x3c:
        case 0x04:
        case 0x14:
        case 0x24:
        case 0x34:
            emulate_INR ( cpu );
            break;
        case 0xc3:
            emulate_JMP ( cpu );
            break;
        case 0xc9:
            emulate_RET ( cpu );
            break;
        case 0xcb:
            switch ( opcode[1] ) {
                case 0x11:
                {
                    uint8_t t;
                    t = cpu->c;
                    cpu->flags.c  = 0x80 == (t & 0x80);
                    cpu->c = ( t << 1 ) | cpu->flags.c ;
                    cpu->cycles += 8;
                    cpu->pc     += 2;
                }
                    break;
                default:
                    printf(" %2x %2x is not implemented\n", opcode[0], opcode[1]);
                    exit(-1);
            }
            break;
        default:
            /*disassembler ( cpu->memory, cpu->pc );*/
            /*print_registers(cpu);*/
            printf(" %2x is not implemented\n", *opcode);
            exit(-1);
    }

    cpu->instructions_executed += 1;

#ifdef __show_step
    print_registers(cpu);
    puts("");
#endif
}

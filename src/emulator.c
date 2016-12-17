#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "memory.h"
#include "types.h"
#include "utils.h"
#include "emulator.h"
#include "disassembler.h"
#include "halfcary.h"

#include "instructions_branch.h"
#include "instructions_arithmetic.h"

void emulate_ANI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xe6: // ANI
            cpu->flags.h = !((cpu->a | opcode[1]) & 0x08) != 0;
            cpu->a       &= opcode[1];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.c = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.n  = (0x80 == (cpu->a & 0x80));

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
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
                uint8_t *dst = get_reg_ref( cpu, ( *opcode & 0x38 ) >> 3 );
                uint8_t *src = get_reg_ref( cpu, ( *opcode & 0x07 ) >> 0 );
                *dst = *src;
            }
            break;
    }

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
            cpu->flags.z  = ((cpu->memory[cpu->sp] & 0x80));
            cpu->flags.n  = ((cpu->memory[cpu->sp] & 0x40));
            cpu->flags.h  = ((cpu->memory[cpu->sp] & 0x20));
            cpu->flags.c  = ((cpu->memory[cpu->sp] & 0x10));
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
            cpu->memory[cpu->sp-2] = ( cpu->flags.z  ? 0x80 : 0x0 ) |
                                     ( cpu->flags.n  ? 0x40 : 0x0 ) |
                                     ( cpu->flags.h  ? 0x20 : 0x0 ) |
                                     ( cpu->flags.c  ? 0x10 : 0x0 ) ;
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

void emulate_XRI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xee: // XRI D8
            cpu->a ^= opcode[1];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.c = 0;
    cpu->flags.h = 0;
    cpu->flags.z = (cpu->a == 0);
    cpu->flags.n = 0;

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
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
        case 0x2a:
            addr = cpu->h << 8 | cpu->l;
            cpu->a = read_byte(cpu, addr);
            addr += 1;
            cpu->l = (addr & 0x00ff) >> 0;
            cpu->h = (addr & 0xff00) >> 8;
            cpu->cycles += 8;
            cpu->pc     += 1 ;
            break;
        case 0x00: emulate_NOP  ( cpu );
            break;
        case 0x01:
            cpu->b = opcode[2];
            cpu->c = opcode[1];
            cpu->cycles += 12;
            cpu->pc     += 3;
            break;
        case 0x02:
        case 0x12:
            emulate_STAX( cpu );
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
        case 0x18:
            cpu->pc += (int8_t) opcode[1] + 2; // Looks like there is a sign bit somewhere
            break;
        case 0x20:
            cpu->pc = cpu->flags.z ?
                        cpu->pc + 2   :
                        cpu->pc + (int8_t) opcode[1] + 2;
            break;
        case 0x22:
            addr = ( cpu->h << 8 ) | cpu->l;
            write_byte( cpu, addr, cpu->a );
            addr += 1;
            cpu->h = ( addr & 0xff00 ) >> 8;
            cpu->l = ( addr & 0x00ff ) >> 0;
            cpu->cycles += 8;
            cpu->pc += 1;
            break;
        case 0x26:
            cpu->h = opcode[1];
            cpu->cycles += 8 ;
            cpu->pc     += 2 ;
            break;
        case 0x28:
            if ( cpu->flags.z ) {
                cpu->pc += (int8_t) opcode[1] + 2;
            } else {
                cpu->pc += 2;
            }
            break;
        case 0x30:
            cpu->pc = cpu->flags.c ?
                        cpu->pc + 2   :
                        cpu->pc + (int8_t) opcode[1] + 2;
            break;
        case 0x11:
            cpu->d = opcode[2];
            cpu->e = opcode[1];
            cpu->pc += 3;
            cpu->cycles += 12;
            break;
        case 0x1f:
            {
                uint8_t t     = cpu->flags.c;
                cpu->flags.c  = cpu->a & 0x01;
                cpu->a        = (cpu->a >> 1) | ( t << 7 );
                cpu->flags.z  = 0;
                cpu->flags.h  = 0;
                cpu->flags.n  = 0;
                cpu->pc      += 1;
                cpu->cycles  += 8;
            }
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
        case 0xc4:
            emulate_CNZ  ( cpu );
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
            emulate_LD  ( cpu );
            break;
        case 0x32: // LD (HL-),A
            addr              = cpu->h << 8 | cpu->l;
            cpu->memory[addr] = cpu->a;
            addr              = cpu->h << 8 | cpu->l;
            addr             -= 1;
            cpu->h            = (addr >> 8 ) & 0xff;
            cpu->l            = (addr >> 0 ) & 0xff;
            cpu->pc += 1;
            cpu->cycles += 12;
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
        case 0xa8:
        case 0xa9:
        case 0xaa:
        case 0xab:
        case 0xac:
        case 0xad:
        case 0xae:
        case 0xaf:
            emulate_XOR  ( cpu );
            break;
        case 0xe0:
            cpu->memory[0xff00 + opcode[1]] = cpu->a;
            cpu->cycles += 12;
            cpu->pc     += 2 ;
            break;
        case 0xf0:
            cpu->a = read_byte(cpu, 0xff00 + opcode[1]);
            cpu->cycles += 12;
            cpu->pc     += 2 ;
            break;
        case 0xe2:
            cpu->memory[0xff00 + cpu->c] = cpu->a;
            cpu->cycles += 8 ;
            cpu->pc     += 1 ;
            break;
        case 0xea:
            addr = ( opcode[2] << 8 ) | opcode[1];
            write_byte ( cpu, addr, cpu->a );
            cpu->cycles += 8;
            cpu->pc     += 3;
            break;
        case 0x77: // MOV M, A
            addr = ( cpu->h << 8 ) | cpu->l;
            cpu->memory[addr] = cpu->a;
            cpu->cycles += 8;
            cpu->pc     += 1;
            break;
        case 0xc8:
            emulate_RZ ( cpu );
            break;
        case 0xfe:
            emulate_CPI ( cpu );
            break;
        case 0x09:
        case 0x19:
        case 0x29:
        case 0x39:
            emulate_DAD ( cpu );
            break;
        case 0xb0:
        case 0xb1:
        case 0xb2:
        case 0xb3:
        case 0xb4:
        case 0xb5:
        case 0xb6:
        case 0xb7:
            emulate_ORA ( cpu );
            break;
        case 0xe9:
            emulate_PCHL ( cpu );
            break;
        case 0xce:
            emulate_ACI ( cpu );
            break;
        case 0xd6:
            emulate_SUI ( cpu );
            break;
        case 0xc6:
            emulate_ADI ( cpu );
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
        case 0xf3:
            emulate_DI ( cpu );
            break;
        case 0xe6:
            emulate_ANI ( cpu );
            break;
        case 0xd0:
            emulate_RNC ( cpu );
            break;
        case 0xc9:
            emulate_RET ( cpu );
            break;
        case 0xfa:
            addr         = read_next_word ( cpu );
            cpu->a       = read_byte ( cpu, addr );
            cpu->cycles += 16;
            cpu->pc     += 3;
            break;
        case 0xee:
            emulate_XRI( cpu );
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
                case 0x19:
                {
                    uint8_t t     = cpu->flags.c;
                    cpu->flags.c  = cpu->c & 0x01;
                    cpu->c        = (cpu->c >> 1) | ( t << 7 );
                    cpu->flags.z = cpu->c == 0;
                    cpu->flags.h = 0;
                    cpu->flags.n = 0;
                    cpu->pc     += 2;
                    cpu->cycles += 8;
                }
                    break;
                case 0x1a:
                {
                    uint8_t t     = cpu->flags.c;
                    cpu->flags.c  = cpu->d & 0x01;
                    cpu->d        = (cpu->d >> 1) | ( t << 7 );
                    cpu->flags.z = cpu->d == 0;
                    cpu->flags.h = 0;
                    cpu->flags.n = 0;
                    cpu->pc     += 2;
                    cpu->cycles += 8;
                }
                    break;
                case 0x38:
                    cpu->flags.c = ( cpu->b & 0x01 );
                    cpu->b       = cpu->b >> 1;
                    cpu->flags.z = cpu->b == 0;
                    cpu->flags.h = 0;
                    cpu->flags.n = 0;
                    cpu->pc     += 2;
                    cpu->cycles += 8;
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "types.h"
#include "utils.h"
#include "emulator.h"
#include "disassembler.h"

/*void print_registers ( _cpu_info *cpu ) {*/
    /*printf(" A: %02x B: %02x C: %02x D: %02x E: %02x H: %02x L: %02x M: %04x SP: %08x PC: %08x",*/
            /*cpu->a, cpu->b, cpu->c, cpu->d, cpu->e, cpu->h, cpu->l, cpu->h << 8 | cpu-> l, cpu->sp, cpu->pc);*/
    /*printf(" F: %c%c%c%c%c CYCLES: %8d IPS: %8d\n",*/
            /*cpu->flags.z  ? 'z' : '.',*/
            /*cpu->flags.s  ? 's' : '.',*/
            /*cpu->flags.p  ? 'p' : '.',*/
            /*cpu->flags.cy ? 'c' : '.',*/
            /*cpu->flags.ac ? 'a' : '.',*/
            /*cpu->cycles  % 16667     ,*/
            /*cpu->instructions_executed);*/
/*}*/

void print_registers ( _cpu_info *cpu ) {
    uint8_t f = cpu->flags.cy << 0 |
                            0 << 1 |
                cpu->flags.p  << 2 |
                            0 << 3 |
                cpu->flags.ac << 4 |
                            0 << 5 |
                cpu->flags.z  << 6 |
                cpu->flags.s  << 7;

    printf(" AF: %02x%02x BC: %02x%02x DE: %02x%02x HL: %02x%02x PC: %08x SP: %08x",
            cpu->a, f, cpu->b, cpu->c, cpu->d, cpu->e, cpu->h, cpu->l, cpu->pc, cpu->sp);
    printf(" F: %c%c%c%c%c CYCLES: %8d IPS: %8d\n",
            cpu->flags.z  ? 'z' : '.',
            cpu->flags.s  ? 's' : '.',
            cpu->flags.p  ? 'p' : '.',
            cpu->flags.cy ? 'c' : '.',
            cpu->flags.ac ? 'a' : '.',
            cpu->cycles  % 16667     ,
            cpu->instructions_executed);
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

void emulate_XRA ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xaf: // XRA A
            cpu->a       ^= cpu->a;
            cpu->flags.cy = 0;
            cpu->flags.ac = 0;
            cpu->flags.z  = (cpu->a == 0);
            cpu->flags.s  = (0x80 == (cpu->a & 0x80));
            cpu->flags.p  = parity_bit(cpu->a);
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_ADI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t t;

    switch ( *opcode ) {
        case 0xc6: // ADI
            t             = cpu->a + (uint8_t) opcode[1];
            cpu->a        = (uint8_t) t;
            cpu->flags.cy = (t > 0xff);
            cpu->flags.z  = (cpu->a == 0);
            cpu->flags.s  = (0x80 == (cpu->a & 0x80));
            cpu->flags.p  = parity_bit(cpu->a);
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
}

void emulate_ANA ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xa7: // ANA
            cpu->a       &= cpu->a;
            cpu->flags.cy = 0;
            cpu->flags.ac = 0;
            cpu->flags.z  = (cpu->a == 0);
            cpu->flags.s  = (0x80 == (cpu->a & 0x80));
            cpu->flags.p  = parity_bit(cpu->a);
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_ANI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xe6: // ANI
            cpu->a       &= opcode[1];
            cpu->flags.cy = 0;
            cpu->flags.ac = 0;
            cpu->flags.z  = (cpu->a == 0);
            cpu->flags.s  = (0x80 == (cpu->a & 0x80));
            cpu->flags.p  = parity_bit(cpu->a);
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
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

    cpu->cycles += 11; // FIXME This is actually 10
    cpu->pc     += 1 ;
}

void emulate_RAL ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint8_t t = 0;

    switch ( *opcode ) {
        case 0x17: // RAL
            t = cpu->a;
            cpu->a = ( t << 1 ) | (( t & 0x80 ) >> 7 );
            cpu->flags.cy = 0 < ( t & 0x80 );
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
        case 0x0f: // RRC A
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

void emulate_XCHG ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint8_t t = 0;

    switch ( *opcode ) {
        case 0xeb: // CPI A
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
                    case 0x03:
                        {
                        uint16_t v = (cpu->shift1<<8) | cpu->shift0;
                        cpu->a = ((v >> (8 - cpu->shift_offset)) & 0xff);
                        }
                        break;
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
                    case 0x02:
                        cpu->shift_offset = cpu->a & 0x7;
                        break;
                    case 0x04:
                        cpu->shift0 = cpu->shift1;
                        cpu->shift1 = cpu->a;
                        break;
                    default:
                        break;
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10;
    cpu->pc     += 2 ;
}

void emulate_CPI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint8_t        answer = 0;

    switch ( *opcode ) {
        case 0xfe: // CPI A
            answer = cpu->a - opcode[1];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = answer == 0                  ; // Only the last 8 bits matters, hence the mask
    cpu->flags.s    = ( answer & 0x80 ) != 0       ; // Checks if the MSB is 1
    cpu->flags.cy   = ( cpu->a < opcode[1] )       ; // Checks for the carry bit
    cpu->flags.p    = parity_bit ( answer & 0xff ) ; // Returns 1 if the number os bits set as 1 is even

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
}

void emulate_ADC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t answer = 0;

    switch ( *opcode ) {
        case 0x8c: // ADC H
            {
            assert( ( cpu->flags.cy == 0 || cpu->flags.cy == 1 ) && "Invalid CY flag" );
            answer = cpu->a + cpu->h + cpu->flags.cy;
            cpu->a = answer & 0xff;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0       ; // Only the last 8 bits matters, hence the mask
    cpu->flags.s    = ( answer & 0x80 ) != 0       ; // Checks if the MSB is 1
    cpu->flags.cy   = ( answer > 0xff )            ; // Checks for the carry bit
    cpu->flags.p    = parity_bit ( answer & 0xff ) ; // Returns 1 if the number os bits set as 1 is even
    /*cpu->a          = answer & 0xff                ; // Stores the 8 bit result in the A register*/

    cpu->cycles += 7 ;
    cpu->pc     += 1 ;
}

void emulate_SBB ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t answer = 0;

    switch ( *opcode ) {
        case 0x9e: // SBB M
            {
            assert( ( cpu->flags.cy == 0 || cpu->flags.cy == 1 ) && "Invalid CY flag" );
            uint16_t addr = cpu->h << 8 | cpu->l;
            answer = cpu->a - cpu->memory[addr] - cpu->flags.cy;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0       ; // Only the last 8 bits matters, hence the mask
    cpu->flags.s    = ( answer & 0x80 ) != 0       ; // Checks if the MSB is 1
    cpu->flags.cy   = ( answer > 0xff )            ; // Checks for the carry bit
    cpu->flags.p    = parity_bit ( answer & 0xff ) ; // Returns 1 if the number os bits set as 1 is even
    /*cpu->a          = answer & 0xff                ; // Stores the 8 bit result in the A register*/

    cpu->cycles += 7 ;
    cpu->pc     += 1 ;
}

void emulate_DCR ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t answer = 0;

    switch ( *opcode ) {
        case 0x3d: // DCR A
            cpu->a -= 1;
            answer = cpu->a;
            break;
        case 0x05: // DCR B
            cpu->b -= 1;
            answer = cpu->b;
            break;
        case 0x0D: // DCR C
            cpu->c -= 1;
            answer = cpu->c;
            break;
        case 0x35: // DCR M
            answer = cpu->memory[cpu->h << 8 | cpu->l];
            answer -= 1;
            cpu->memory[cpu->h << 8 | cpu->l] = answer;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0       ; // Only the last 8 bits matters, hence the mask
    cpu->flags.s    = ( answer & 0x80 ) != 0       ; // Checks if the MSB is 1
    cpu->flags.cy   = ( answer > 0xff )            ; // Checks for the carry bit
    cpu->flags.p    = parity_bit ( answer & 0xff ) ; // Returns 1 if the number os bits set as 1 is even
    /*cpu->a          = answer & 0xff                ; // Stores the 8 bit result in the A register*/

    cpu->cycles += 5 ;
    cpu->pc     += 1 ;
}

void emulate_POP ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xc1: // POP B
            cpu->c = cpu->memory[cpu->sp - 0];
            cpu->b = cpu->memory[cpu->sp + 1];
            cpu->sp += 2;
            break;
        case 0xd1: // POP D
            cpu->e = cpu->memory[cpu->sp - 0];
            cpu->d = cpu->memory[cpu->sp + 1];
            cpu->sp += 2;
            break;
        case 0xe1: // POP H
            cpu->l = cpu->memory[cpu->sp - 0];
            cpu->h = cpu->memory[cpu->sp + 1];
            cpu->sp += 2;
            break;
        case 0xf1: // POP PSW
            cpu->a = cpu->memory[cpu->sp+1];
            cpu->flags.z  = (0x01 == (cpu->memory[cpu->sp] & 0x01));
            cpu->flags.s  = (0x02 == (cpu->memory[cpu->sp] & 0x02));
            cpu->flags.p  = (0x04 == (cpu->memory[cpu->sp] & 0x04));
            cpu->flags.cy = (0x08 == (cpu->memory[cpu->sp] & 0x08));
            cpu->flags.ac = (0x10 == (cpu->memory[cpu->sp] & 0x10));
            cpu->sp += 2;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10;
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
            cpu->memory[cpu->sp-2] = cpu->flags.z       |
                                     cpu->flags.s  << 1 |
                                     cpu->flags.p  << 2 |
                                     cpu->flags.cy << 3 |
                                     cpu->flags.ac << 4 ;
            cpu->sp = cpu->sp - 2;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 11;
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
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 6 ; // FIXME This is 5
    cpu->pc     += 1 ;
}

void emulate_ADD ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t answer = (uint16_t) cpu->a;

    switch ( *opcode ) {
        case 0x80:
            answer += (uint16_t) cpu->b;
            break;
        case 0x81:
            answer += (uint16_t) cpu->c;
            break;
        case 0x82:
            answer += (uint16_t) cpu->d;
            break;
        case 0x83:
            answer += (uint16_t) cpu->e;
            break;
        case 0x84:
            answer += (uint16_t) cpu->h;
            break;
        case 0x85:
            answer += (uint16_t) cpu->l;
            break;
        case 0x86:
            answer += (uint16_t) cpu->memory[(cpu->h<<8) | (cpu->l)];
            cpu->cycles += 3;
            break;
        case 0x87:
            answer += (uint16_t) cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z    = ( answer & 0xff ) == 0       ; // Only the last 8 bits matters, hence the mask
    cpu->flags.s    = ( answer & 0x80 ) != 0       ; // Checks if the MSB is 1
    cpu->flags.cy   = ( answer > 0xff )            ; // Checks for the carry bit
    cpu->flags.p    = parity_bit ( answer & 0xff ) ; // Returns 1 if the number os bits set as 1 is even
    cpu->a          = answer & 0xff                ; // Stores the 8 bit result in the A register
    cpu->pc        += 1                            ; // Moves the program counter
    cpu->cycles    += 4                            ; // Update the total of cycles spent
}

void emulate_NOP ( _cpu_info *cpu ) {
    /*printf(" %08x : NOP\n", cpu->pc);*/
    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
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

    /*if ( *opcode != 0x31 )*/
}

void emulate_MVI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0x06: // MVI B
            cpu->b = opcode[1];
            break;
        case 0x0e: // MVI C
            cpu->c = opcode[1];
            break;
        case 0x26: // MVI H
            cpu->h = opcode[1];
            break;
        case 0x36: // MVI M
            cpu->memory[cpu->h << 8 | cpu->l] = cpu->memory[opcode[1]];
            cpu->cycles += 3;
            break;
        case 0x3e: // MVI A, D*
            cpu->a = opcode[1];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 7 ;
    cpu->pc     += 2 ;
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

void emulate_MOV ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0x41: // MOV B, C
            cpu->b = cpu->c;
            break;
        case 0x4f: // MOV C, A
            cpu->c = cpu->a;
            break;
        case 0x7e: // MOV A, M
            addr = cpu->h << 8 | cpu->l;
            cpu->d = cpu->memory[addr];
            cpu->cycles += 2;
            break;
        case 0x66: // MOV H, M
            addr = cpu->h << 8 | cpu->l;
            cpu->h = cpu->memory[addr];
            cpu->cycles += 2;
            break;
        case 0x67: // MOV H, A
            cpu->h = cpu->a;
            break;
        case 0x56: // MOV D, M
            addr = cpu->h << 8 | cpu->l;
            cpu->d = cpu->memory[addr];
            cpu->cycles += 2;
            break;
        case 0x57: // MOV D, A
            cpu->d = cpu->a;
            break;
        case 0x5e: // MOV E, M
            addr = cpu->h << 8 | cpu->l;
            cpu->e = cpu->memory[addr];
            cpu->cycles += 2;
            break;
        case 0x5f: // MOV E, A
            cpu->e = cpu->a;
            break;
        case 0x77: // MOV M, A
            addr = cpu->h << 8 | cpu->l;
            cpu->memory[addr] = cpu->a;
            cpu->cycles += 2;
            break;
        case 0x7b: // MOV A, E
            cpu->a = cpu->e;
            break;
        case 0x7a: // MOV A, D
            cpu->a = cpu->d;
            break;
        case 0x7c: // MOV A, H
            cpu->a = cpu->h;
            break;
        case 0x6f: // MOV L, A
            cpu->l = cpu->a;
            break;
        default: // Too lazy to do all the MOVs now
            printf(" %2X is not implemented\n", cpu->memory[cpu->pc]);
            assert( 0 && "Code should not get here\n" );
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

void emulate_JZ ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xca: // JZ
            addr = opcode[2] << 8 | opcode[1];
            if ( cpu->flags.z ) {
                cpu->pc = addr;
                cpu->cycles += 5; // FIXME this is 3
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
                cpu->cycles += 5; // FIXME this is 3
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

void emulate_CALL ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xcd:
            {
                assert ( cpu->sp >= 2 && "Got a segfault in the 8080" );
                uint16_t ret           = cpu->pc + 3;
                cpu->memory[cpu->sp-1] = (ret >> 8) & 0xff;
                cpu->memory[cpu->sp-2] = (ret & 0xff);
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

    /*if ( cpu->instructions_executed > 16243200 ) {*/
        /*disassembler ( cpu->memory, cpu->pc );*/
        /*print_registers(cpu);*/
    /*}*/

           if ( *opcode == 0x00 ) {
        emulate_NOP ( cpu );
    } else if ( *opcode == 0x05 || *opcode == 0x0d || *opcode == 0x35 || *opcode == 0x3d ) {
        emulate_DCR ( cpu );
    } else if ( *opcode == 0x17 ) {
        emulate_RAL ( cpu );
    } else if ( *opcode == 0x0f ) {
        emulate_RRC ( cpu );
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
    } else if ( *opcode == 0x03 || *opcode == 0x13 || *opcode == 0x23 ) {
        emulate_INX ( cpu );
    } else if ( *opcode == 0x09 || *opcode == 0x19 || *opcode == 0x29 || *opcode == 0x39 ) {
        emulate_DAD ( cpu );
    } else if ( *opcode == 0x3e || *opcode == 0x36 || *opcode == 0x26 || *opcode == 0x06 || *opcode == 0x0e ) {
        emulate_MVI ( cpu );
    } else if ( ( *opcode >= 0x40 && *opcode <= 0x75 ) || ( *opcode >= 0x77 && *opcode <= 0x7f ) ) {
        emulate_MOV ( cpu );
    } else if ( *opcode >= 0x80 && *opcode <= 0x87 ) {
        emulate_ADD ( cpu );
    } else if ( *opcode == 0x8c ) {
        emulate_ADC ( cpu );
    } else if ( *opcode == 0x9e ) {
        emulate_SBB ( cpu );
    } else if ( *opcode == 0xaf ) {
        emulate_XRA ( cpu );
    } else if ( *opcode == 0xca ) {
        emulate_JZ ( cpu );
    } else if ( *opcode == 0xc2 ) {
        emulate_JNZ ( cpu );
    } else if ( *opcode == 0xc3 ) {
        emulate_JMP ( cpu );
    } else if ( *opcode == 0xd2 ) {
        emulate_JNC ( cpu );
    } else if ( *opcode == 0xda ) {
        emulate_JC ( cpu );
    } else if ( *opcode == 0xc6 ) {
        emulate_ADI ( cpu );
    } else if ( *opcode == 0xd8 ) {
        emulate_RC ( cpu );
    } else if ( *opcode == 0xc0 ) {
        emulate_RNZ ( cpu );
    } else if ( *opcode == 0xc8 ) {
        emulate_RZ ( cpu );
    } else if ( *opcode == 0xc9 ) {
        emulate_RET ( cpu );
    } else if ( *opcode == 0xcd || *opcode == 0xcd ) {
        emulate_CALL ( cpu );
    } else if ( *opcode == 0xe1 || *opcode == 0xd1 || *opcode == 0xc1 || *opcode == 0xf1 ) {
        emulate_POP ( cpu );
    } else if ( *opcode == 0xc5 || *opcode == 0xd5 || *opcode == 0xe5 || *opcode == 0xf5 ) {
        emulate_PUSH ( cpu );
    } else if ( *opcode == 0xdb ) {
        emulate_IN ( cpu );
    } else if ( *opcode == 0xd3 ) {
        emulate_OUT ( cpu );
    } else if ( *opcode == 0xa7 ) {
        emulate_ANA ( cpu );
    } else if ( *opcode == 0xe6 ) {
        emulate_ANI ( cpu );
    } else if ( *opcode == 0xeb ) {
        emulate_XCHG ( cpu );
    } else if ( *opcode == 0xfb ) {
        emulate_EI ( cpu );
    } else if ( *opcode == 0xfe ) {
        emulate_CPI ( cpu );
    } else {
        disassembler ( cpu->memory, cpu->pc );
        print_registers(cpu);
        printf(" %2X is not implemented\n", cpu->memory[cpu->pc]);
        exit(-1);
    }

    cpu->instructions_executed += 1;

    /*if ( cpu->instructions_executed > 16243200 ) {*/
        /*print_registers(cpu);*/
        /*puts("");*/
    /*}*/

    return op_size;
}

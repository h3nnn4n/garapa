#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "types.h"
#include "utils.h"
#include "emulator.h"
#include "disassembler.h"

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

    cpu->flags.cy = 0;
    cpu->flags.ac = 0;
    cpu->flags.z  = (cpu->a == 0);
    cpu->flags.s  = 0; //(0x80 == (cpu->a & 0x80));
    cpu->flags.p  = 0; //parity_bit(cpu->a);

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
    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_JNZ ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0x20: // JZ
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

void emulator( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

#ifdef __show_step
    disassembler ( cpu->memory, cpu->pc );
    print_registers(cpu);
#endif
    uint16_t addr;

    switch ( *opcode ) {
        case 0x00: emulate_NOP  ( cpu );
            break;
        case 0x0e:
            cpu->c = opcode[1];
            cpu->cycles += 8 ;
            cpu->pc     += 2 ;
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
        case 0x77: // MOV M, A
            addr = cpu->h << 8 | cpu->l;
            cpu->memory[addr] = cpu->a;
            cpu->cycles += 8;
            cpu->pc     += 1 ;
            break;
        default:
            /*disassembler ( cpu->memory, cpu->pc );*/
            /*print_registers(cpu);*/
            printf(" %2X is not implemented\n", cpu->memory[cpu->pc]);
            exit(-1);
    }

    cpu->instructions_executed += 1;

#ifdef __show_step
    print_registers(cpu);
    puts("");
#endif
}

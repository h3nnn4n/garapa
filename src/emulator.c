#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "types.h"
#include "utils.h"
#include "emulator.h"
#include "disassembler.h"

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

void emulator( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

#ifdef __show_step
    disassembler ( cpu->memory, cpu->pc );
    print_registers(cpu);
#endif

    switch ( *opcode ) {
        case 0x00: emulate_NOP  ( cpu );
            break;
        case 0x20:
            emulate_JNZ  ( cpu );
            break;
        case 0x26:
            cpu->h = opcode[1];
            cpu->cycles += 8 ;
            cpu->pc     += 2 ;
            break;
        case 0x21:
        case 0x31:
        case 0x32:
            emulate_LD  ( cpu );
            break;
        case 0xaf:
            emulate_XOR  ( cpu );
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

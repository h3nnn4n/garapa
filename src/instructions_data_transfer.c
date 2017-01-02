#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "memory.h"

#include "instructions_data_transfer.h"

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
        case 0x06: // (HL)
            return &cpu->mem_controller.memory[( cpu->h << 8 ) | cpu->l];
        case 0x07: // A
            return &(cpu->a);
        default:
            printf(" -> %02x = 0b"BYTE_TO_BINARY_PATTERN"\n", opcode, BYTE_TO_BINARY(opcode));
            assert( 0 && "Invalid OPCODE for MOV");
    }
    return NULL;
}

void emulate_LDAX ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0x0a: // LDAX B
            addr = cpu->b << 8 | cpu->c;
            cpu->a = cpu->mem_controller.memory[addr];
            break;
        case 0x1a: // LDAX D
            addr = cpu->d << 8 | cpu->e;
            cpu->a = cpu->mem_controller.memory[addr];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 2 ;
    cpu->pc     += 1 ;
}

void emulate_STAX ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];

    switch ( *opcode ) {
        case 0x02: // STAX B
            cpu->mem_controller.memory[ ( cpu->b << 8 ) | cpu->c ] = cpu->a;
            break;
        case 0x12: // STAX D
            cpu->mem_controller.memory[ ( cpu->d << 8 ) | cpu->e ] = cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 2 ;
    cpu->pc     += 1 ;
}

void emulate_SHLD ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];

    switch ( *opcode ) {
        case 0x22: // SHLD D16
            {
            uint16_t addr = opcode[2] << 8 | opcode[1];
            cpu->mem_controller.memory[addr+0] = cpu->l;
            cpu->mem_controller.memory[addr+1] = cpu->h;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 16;
    cpu->pc     += 3 ;
}

void emulate_LDA ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0x2a:
            addr = (cpu->h << 8) | cpu->l;
            cpu->a = read_byte(cpu, addr);
            addr += 1;
            cpu->l = (addr & 0x00ff) >> 0;
            cpu->h = (addr & 0xff00) >> 8;
            cpu->pc -= 2;
            break;
        case 0x3a:
            addr = (cpu->h << 8) | cpu->l;
            /*addr = (opcode[2] << 8) | opcode[1];*/
            cpu->a = read_byte(cpu, addr);
            addr -= 1;
            cpu->l = (addr & 0x00ff) >> 0;
            cpu->h = (addr & 0xff00) >> 8;
            cpu->pc -= 2;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 2;
    cpu->pc     += 3;
}

void emulate_STA ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0x32:
            addr = opcode[2] << 8 | opcode[1];
            cpu->mem_controller.memory[addr] = cpu->a;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 13;
    cpu->pc     += 3 ;
}

void emulate_LD ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];

    switch ( *opcode ) {
        case 0x01: // LXI B
            cpu->b = opcode[2];
            cpu->c = opcode[1];
            break;
        case 0x11: // LXI D
            cpu->d = opcode[2];
            cpu->e = opcode[1];
            break;
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

    cpu->cycles_machine += 3;
    cpu->pc     += 3 ;
}

void emulate_MVI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];

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
            cpu->cycles_machine += 1 ;
            break;
        case 0x2e: // MVI L, D8
            cpu->l = opcode[1];
            break;
        case 0x36: // MVI M, D8
            cpu->mem_controller.memory[cpu->h << 8 | cpu->l] = opcode[1];
            cpu->cycles_machine += 1;
            break;
        case 0x3e: // MVI A, D8
            cpu->a = opcode[1];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 2 ;
    cpu->pc     += 2 ;
}

void emulate_MOV ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0x46: // MOV B, M
            addr = cpu->h << 8 | cpu->l;
            cpu->b = cpu->mem_controller.memory[addr];
            cpu->cycles_machine += 1;
            break;
        case 0x56: // MOV D, M
            addr = cpu->h << 8 | cpu->l;
            cpu->d = cpu->mem_controller.memory[addr];
            cpu->cycles_machine += 1;
            break;
        case 0x66: // MOV H, M
            addr = cpu->h << 8 | cpu->l;
            cpu->h = cpu->mem_controller.memory[addr];
            cpu->cycles_machine += 1;
            break;

        case 0x70: // MOV M, B
            addr = cpu->h << 8 | cpu->l;
            cpu->mem_controller.memory[addr] = cpu->b;
            cpu->cycles_machine += 1;
            break;
        case 0x71: // MOV M, C
            addr = cpu->h << 8 | cpu->l;
            cpu->mem_controller.memory[addr] = cpu->c;
            cpu->cycles_machine += 1;
            break;
        case 0x72: // MOV M, D
            addr = cpu->h << 8 | cpu->l;
            cpu->mem_controller.memory[addr] = cpu->d;
            cpu->cycles_machine += 1;
            break;
        case 0x73: // MOV M, E
            addr = cpu->h << 8 | cpu->l;
            cpu->mem_controller.memory[addr] = cpu->e;
            cpu->cycles_machine += 1;
            break;
        case 0x74: // MOV M, H
            addr = cpu->h << 8 | cpu->l;
            cpu->mem_controller.memory[addr] = cpu->h;
            cpu->cycles_machine += 1;
            break;
        case 0x75: // MOV M, L
            addr = cpu->h << 8 | cpu->l;
            cpu->mem_controller.memory[addr] = cpu->l;
            cpu->cycles_machine += 1;
            break;
        case 0x77: // MOV M, A
            addr = cpu->h << 8 | cpu->l;
            cpu->mem_controller.memory[addr] = cpu->a;
            cpu->cycles_machine += 1;
            break;

        case 0x4e: // MOV C, M
            addr = cpu->h << 8 | cpu->l;
            cpu->c = cpu->mem_controller.memory[addr];
            cpu->cycles_machine += 1;
            break;
        case 0x5e: // MOV E, M
            addr = cpu->h << 8 | cpu->l;
            cpu->e = cpu->mem_controller.memory[addr];
            cpu->cycles_machine += 1;
            break;
        case 0x6e: // MOV L, M
            addr = cpu->h << 8 | cpu->l;
            cpu->l = cpu->mem_controller.memory[addr];
            cpu->cycles_machine += 1;
            break;
        case 0x7e: // MOV A, M
            addr = cpu->h << 8 | cpu->l;
            cpu->a = cpu->mem_controller.memory[addr];
            cpu->cycles_machine += 1;
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

    cpu->cycles_machine += 1 ;
    cpu->pc     += 1 ;
}

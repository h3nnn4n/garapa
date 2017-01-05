#include <stdio.h>
#include <stdlib.h>

#include "types.h"

unsigned short int out2( unsigned char *buffer, unsigned int pc ) ;
void out1 ( _cpu_info *cpu ) ;

void out_put(_cpu_info *cpu) {
    /*printf ( "TRACE:wadatsumi::gb::cpu:%11llu: ", cpu->cycles_machine ) ;*/
    printf ( "Cycles: %11llu: ", cpu->cycles_machine ) ;
    out2   ( cpu->mem_controller.memory, cpu->pc                        ) ;
    out1   ( cpu                                                        ) ;
    puts   ( ""                                                         ) ;
}

/*TRACE:wadatsumi::gb::cpu:    1787559: JR 0xFE                   PC: 0xC8B0 AF: 0x00C0 BC: 0xD826 DE: 0xD826 HL: 0xCC00 SP: 0xDFFF*/
void out1 ( _cpu_info *cpu ) {
    uint8_t f = ( (cpu->flags.z ) ? 0x80 : 0x00 ) |
                ( (cpu->flags.n ) ? 0x40 : 0x00 ) |
                ( (cpu->flags.h ) ? 0x20 : 0x00 ) |
                ( (cpu->flags.c ) ? 0x10 : 0x00 ) ;

    printf(" PC: 0x%04X AF: 0x%02X%02X BC: 0x%02X%02X DE: 0x%02X%02X HL: 0x%02X%02X SP: 0x%04X",
            cpu->pc, cpu->a, f, cpu->b, cpu->c, cpu->d, cpu->e, cpu->h, cpu->l, cpu->sp);
    /*printf(" F: %c%c%c%c CYCLES: %16llu IPS: %16llu\n",*/
            /*cpu->flags.z  ? 'z' : '.',*/
            /*cpu->flags.n  ? 'n' : '.',*/
            /*cpu->flags.h  ? 'h' : '.',*/
            /*cpu->flags.c  ? 'c' : '.',*/
            /*cpu->cycles_machine      ,*/
            /*cpu->instructions_executed);*/
    /*printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(f));*/
}

unsigned short int out2( unsigned char *buffer, unsigned int pc ) {
    unsigned short int op_size = 1;

    switch ( buffer[pc] ) {
/*TRACE:wadatsumi::gb::cpu:    1787559: JR 0xFE                   PC: 0xC8B0 AF: 0x00C0 BC: 0xD826 DE: 0xD826 HL: 0xCC00 SP: 0xDFFF*/
        case 0x00:   printf ( "NOP                      "                                  ); op_size = 1; break;
        case 0x01:   printf ( "LD BC, 0x%02X%02X            " , buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0x02:   printf ( "LD (BC), A               "                                  ); op_size = 1; break;
        case 0x03:   printf ( "INC BC                   "                                  ); op_size = 1; break;
        case 0x04:   printf ( "INC B                    "                                  ); op_size = 1; break;
        case 0x05:   printf ( "DEC B                    "                                  ); op_size = 1; break;
        case 0x06:   printf ( "LD B, 0x%02X               ",      buffer[pc+1]             ); op_size = 2; break;
        case 0x07:   printf ( "RLCA                     "                                  ); op_size = 1; break;
        case 0x08:   printf ( "LD (0x%02X%02X)    , SP      ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0x09:   printf ( "ADD HL, BC               "                                  ); op_size = 1; break;
        case 0x0A:   printf ( "LD A, (BC)               "                                  ); op_size = 1; break;
        case 0x0B:   printf ( "DEC BC                   "                                  ); op_size = 1; break;
        case 0x0C:   printf ( "INC C                    "                                  ); op_size = 1; break;
        case 0x0D:   printf ( "DEC C                    "                                  ); op_size = 1; break;
        case 0x0E:   printf ( "LD C, 0x%02X               ",  buffer[pc+1]                 ); op_size = 2; break;
        case 0x0F:   printf ( "RRCA                     "                                  ); op_size = 1; break;

        case 0x10:   printf ( "STOP                     "                                  ); op_size = 1; break;
        case 0x11:   printf ( "LD DE, 0x%02X%02X            ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0x12:   printf ( "LD (DE), A               "                                  ); op_size = 1; break;
        case 0x13:   printf ( "INC DE                   "                                  ); op_size = 1; break;
        case 0x14:   printf ( "INC D                    "                                  ); op_size = 1; break;
        case 0x15:   printf ( "DEC D                    "                                  ); op_size = 1; break;
        case 0x16:   printf ( "LD D, 0x%02X               ",               buffer[pc+1]    ); op_size = 2; break;
        case 0x17:   printf ( "RLA                      "                                  ); op_size = 1; break;
        case 0x18:   printf ( "JR 0x%02X                  ",   buffer[pc+1]                ); op_size = 2; break;
        case 0x19:   printf ( "ADD HL, DE               "                                  ); op_size = 1; break;
        case 0x1A:   printf ( "LD A, (DE)               "                                  ); op_size = 1; break;
        case 0x1B:   printf ( "DEC DE                   "                                  ); op_size = 1; break;
        case 0x1C:   printf ( "INC E                    "                                  ); op_size = 1; break;
        case 0x1D:   printf ( "DEC E                    "                                  ); op_size = 1; break;
        case 0x1E:   printf ( "LD E, 0x%02X               ",       buffer[pc+1]            ); op_size = 2; break;
        case 0x1F:   printf ( "RRA                      "                                  ); op_size = 1; break;

        case 0x20:   printf ( "JR NZ, 0x%02X              ",  buffer[pc+1]                 ); op_size = 2; break;
        case 0x21:   printf ( "LD HL, 0x%02X%02X            ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0x22:   printf ( "LDI (HL), A              "                                  ); op_size = 1; break;
        case 0x23:   printf ( "INC HL                   "                                  ); op_size = 1; break;
        case 0x24:   printf ( "INC H                    "                                  ); op_size = 1; break;
        case 0x25:   printf ( "DEC H                    "                                  ); op_size = 1; break;
        case 0x26:   printf ( "LD H, 0x%02X               ",    buffer[pc+1]               ); op_size = 2; break;
        case 0x27:   printf ( "DAA                      "                                  ); op_size = 1; break;
        case 0x28:   printf ( "JR Z, 0x%02X               ",      buffer[pc+1]             ); op_size = 2; break;
        case 0x29:   printf ( "ADD HL, HL               "                                  ); op_size = 1; break;
        case 0x2A:   printf ( "LDI A, (HL)              "                                  ); op_size = 1; break;
        case 0x2B:   printf ( "DEC HL                   "                                  ); op_size = 1; break;
        case 0x2C:   printf ( "INC L                    "                                  ); op_size = 1; break;
        case 0x2D:   printf ( "DEC L                    "                                  ); op_size = 1; break;
        case 0x2E:   printf ( "LD L, 0x%02X               ",   buffer[pc+1]                ); op_size = 2; break;
        case 0x2F:   printf ( "CPL                      "                                  ); op_size = 1; break;

        case 0x30:   printf ( "JR NC, 0x%02X              ",      buffer[pc+1]             ); op_size = 2; break;
        case 0x31:   printf ( "LD SP, (0x%02X%02X)          ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0x32:   printf ( "LDD (HL), A              "                                  ); op_size = 1; break;
        case 0x33:   printf ( "INC SP                   "                                  ); op_size = 1; break;
        case 0x34:   printf ( "INC (HL)                 "                                  ); op_size = 1; break;
        case 0x35:   printf ( "DEC (HL)                 "                                  ); op_size = 1; break;
        case 0x36:   printf ( "LD (HL), 0x%02X            ",                  buffer[pc+1] ); op_size = 2; break;
        case 0x37:   printf ( "SCF                      "                                  ); op_size = 1; break;
        case 0x38:   printf ( "JR C, 0x%02X               ",   buffer[pc+1]                ); op_size = 2; break;
        case 0x39:   printf ( "ADD HL, SP               "                                  ); op_size = 1; break;
        case 0x3A:   printf ( "LDD A, (HL)              "                                  ); op_size = 1; break;
        case 0x3B:   printf ( "DEC SP                   "                                  ); op_size = 1; break;
        case 0x3C:   printf ( "INC A                    "                                  ); op_size = 1; break;
        case 0x3D:   printf ( "DEC A                    "                                  ); op_size = 1; break;
        case 0x3E:   printf ( "LD A, 0x%02X               ",    buffer[pc+1]               ); op_size = 2; break;
        case 0x3F:   printf ( "CCF                      "                                  ); op_size = 1; break;

        case 0x40:   printf ( "LD B, B                  "                                  ); op_size = 1; break;
        case 0x41:   printf ( "LD B, C                  "                                  ); op_size = 1; break;
        case 0x42:   printf ( "LD B, D                  "                                  ); op_size = 1; break;
        case 0x43:   printf ( "LD B, E                  "                                  ); op_size = 1; break;
        case 0x44:   printf ( "LD B, H                  "                                  ); op_size = 1; break;
        case 0x45:   printf ( "LD B, L                  "                                  ); op_size = 1; break;
        case 0x46:   printf ( "LD B, (HL)               "                                  ); op_size = 1; break;
        case 0x47:   printf ( "LD B, A                  "                                  ); op_size = 1; break;
        case 0x48:   printf ( "LD C, B                  "                                  ); op_size = 1; break;
        case 0x49:   printf ( "LD C, C                  "                                  ); op_size = 1; break;
        case 0x4A:   printf ( "LD C, D                  "                                  ); op_size = 1; break;
        case 0x4B:   printf ( "LD C, E                  "                                  ); op_size = 1; break;
        case 0x4C:   printf ( "LD C, H                  "                                  ); op_size = 1; break;
        case 0x4D:   printf ( "LD C, L                  "                                  ); op_size = 1; break;
        case 0x4E:   printf ( "LD C, (HL)               "                                  ); op_size = 1; break;
        case 0x4F:   printf ( "LD C, A                  "                                  ); op_size = 1; break;

        case 0x50:   printf ( "LD D, B                  "                                  ); op_size = 1; break;
        case 0x51:   printf ( "LD D, C                  "                                  ); op_size = 1; break;
        case 0x52:   printf ( "LD D, D                  "                                  ); op_size = 1; break;
        case 0x53:   printf ( "LD D, E                  "                                  ); op_size = 1; break;
        case 0x54:   printf ( "LD D, H                  "                                  ); op_size = 1; break;
        case 0x55:   printf ( "LD D, L                  "                                  ); op_size = 1; break;
        case 0x56:   printf ( "LD D, (HL)               "                                  ); op_size = 1; break;
        case 0x57:   printf ( "LD D, A                  "                                  ); op_size = 1; break;
        case 0x58:   printf ( "LD E, B                  "                                  ); op_size = 1; break;
        case 0x59:   printf ( "LD E, C                  "                                  ); op_size = 1; break;
        case 0x5A:   printf ( "LD E, D                  "                                  ); op_size = 1; break;
        case 0x5B:   printf ( "LD E, E                  "                                  ); op_size = 1; break;
        case 0x5C:   printf ( "LD E, H                  "                                  ); op_size = 1; break;
        case 0x5D:   printf ( "LD E, L                  "                                  ); op_size = 1; break;
        case 0x5E:   printf ( "LD E, (HL)               "                                  ); op_size = 1; break;
        case 0x5F:   printf ( "LD E, A                  "                                  ); op_size = 1; break;

        case 0x60:   printf ( "LD H, B                  "                                  ); op_size = 1; break;
        case 0x61:   printf ( "LD H, C                  "                                  ); op_size = 1; break;
        case 0x62:   printf ( "LD H, D                  "                                  ); op_size = 1; break;
        case 0x63:   printf ( "LD H, E                  "                                  ); op_size = 1; break;
        case 0x64:   printf ( "LD H, H                  "                                  ); op_size = 1; break;
        case 0x65:   printf ( "LD H, L                  "                                  ); op_size = 1; break;
        case 0x66:   printf ( "LD H, (HL)               "                                  ); op_size = 1; break;
        case 0x67:   printf ( "LD H, A                  "                                  ); op_size = 1; break;
        case 0x68:   printf ( "LD L, B                  "                                  ); op_size = 1; break;
        case 0x69:   printf ( "LD L, C                  "                                  ); op_size = 1; break;
        case 0x6A:   printf ( "LD L, D                  "                                  ); op_size = 1; break;
        case 0x6B:   printf ( "LD L, E                  "                                  ); op_size = 1; break;
        case 0x6C:   printf ( "LD L, H                  "                                  ); op_size = 1; break;
        case 0x6D:   printf ( "LD L, L                  "                                  ); op_size = 1; break;
        case 0x6E:   printf ( "LD L, (HL)               "                                  ); op_size = 1; break;
        case 0x6F:   printf ( "LD L, A                  "                                  ); op_size = 1; break;

        case 0x70:   printf ( "LD (HL), B               "                                  ); op_size = 1; break;
        case 0x71:   printf ( "LD (HL), C               "                                  ); op_size = 1; break;
        case 0x72:   printf ( "LD (HL), D               "                                  ); op_size = 1; break;
        case 0x73:   printf ( "LD (HL), E               "                                  ); op_size = 1; break;
        case 0x74:   printf ( "LD (HL), H               "                                  ); op_size = 1; break;
        case 0x75:   printf ( "LD (HL), L               "                                  ); op_size = 1; break;
        case 0x76:   printf ( "HALT                     "                                  ); op_size = 1; break;
        case 0x77:   printf ( "LD (HL), A               "                                  ); op_size = 1; break;
        case 0x78:   printf ( "LD A, B                  "                                  ); op_size = 1; break;
        case 0x79:   printf ( "LD A, C                  "                                  ); op_size = 1; break;
        case 0x7A:   printf ( "LD A, D                  "                                  ); op_size = 1; break;
        case 0x7B:   printf ( "LD A, E                  "                                  ); op_size = 1; break;
        case 0x7C:   printf ( "LD A, H                  "                                  ); op_size = 1; break;
        case 0x7D:   printf ( "LD A, L                  "                                  ); op_size = 1; break;
        case 0x7E:   printf ( "LD A, (HL)               "                                  ); op_size = 1; break;
        case 0x7F:   printf ( "LD A, A                  "                                  ); op_size = 1; break;

        case 0x80:   printf ( "ADD A, B                 "                                  ); op_size = 1; break;
        case 0x81:   printf ( "ADD A, C                 "                                  ); op_size = 1; break;
        case 0x82:   printf ( "ADD A, D                 "                                  ); op_size = 1; break;
        case 0x83:   printf ( "ADD A, E                 "                                  ); op_size = 1; break;
        case 0x84:   printf ( "ADD A, H                 "                                  ); op_size = 1; break;
        case 0x85:   printf ( "ADD A, L                 "                                  ); op_size = 1; break;
        case 0x86:   printf ( "ADD A, (HL)              "                                  ); op_size = 1; break;
        case 0x87:   printf ( "ADD A, A                 "                                  ); op_size = 1; break;
        case 0x88:   printf ( "ADC A, B                 "                                  ); op_size = 1; break;
        case 0x89:   printf ( "ADC A, C                 "                                  ); op_size = 1; break;
        case 0x8A:   printf ( "ADC A, D                 "                                  ); op_size = 1; break;
        case 0x8B:   printf ( "ADC A, E                 "                                  ); op_size = 1; break;
        case 0x8C:   printf ( "ADC A, H                 "                                  ); op_size = 1; break;
        case 0x8D:   printf ( "ADC A, L                 "                                  ); op_size = 1; break;
        case 0x8E:   printf ( "ADC A, (HL)              "                                  ); op_size = 1; break;
        case 0x8F:   printf ( "ADC A, A                 "                                  ); op_size = 1; break;

        case 0x90:   printf ( "SUB A, B                 "                                  ); op_size = 1; break;
        case 0x91:   printf ( "SUB A, C                 "                                  ); op_size = 1; break;
        case 0x92:   printf ( "SUB A, D                 "                                  ); op_size = 1; break;
        case 0x93:   printf ( "SUB A, E                 "                                  ); op_size = 1; break;
        case 0x94:   printf ( "SUB A, H                 "                                  ); op_size = 1; break;
        case 0x95:   printf ( "SUB A, L                 "                                  ); op_size = 1; break;
        case 0x96:   printf ( "SUB A, (HL)              "                                  ); op_size = 1; break;
        case 0x97:   printf ( "SUB A, A                 "                                  ); op_size = 1; break;
        case 0x98:   printf ( "SBC A, B                 "                                  ); op_size = 1; break;
        case 0x99:   printf ( "SBC A, C                 "                                  ); op_size = 1; break;
        case 0x9A:   printf ( "SBC A, D                 "                                  ); op_size = 1; break;
        case 0x9B:   printf ( "SBC A, E                 "                                  ); op_size = 1; break;
        case 0x9C:   printf ( "SBC A, H                 "                                  ); op_size = 1; break;
        case 0x9D:   printf ( "SBC A, L                 "                                  ); op_size = 1; break;
        case 0x9E:   printf ( "SBC A, (HL)              "                                  ); op_size = 1; break;
        case 0x9F:   printf ( "SBC A, A                 "                                  ); op_size = 1; break;

        case 0xA0:   printf ( "AND A, B                 "                                  ); op_size = 1; break;
        case 0xA1:   printf ( "AND A, C                 "                                  ); op_size = 1; break;
        case 0xA2:   printf ( "AND A, D                 "                                  ); op_size = 1; break;
        case 0xA3:   printf ( "AND A, E                 "                                  ); op_size = 1; break;
        case 0xA4:   printf ( "AND A, H                 "                                  ); op_size = 1; break;
        case 0xA5:   printf ( "AND A, L                 "                                  ); op_size = 1; break;
        case 0xA6:   printf ( "AND A, (HL)              "                                  ); op_size = 1; break;
        case 0xA7:   printf ( "AND A, A                 "                                  ); op_size = 1; break;
        case 0xA8:   printf ( "XOR A, B                 "                                  ); op_size = 1; break;
        case 0xA9:   printf ( "XOR A, C                 "                                  ); op_size = 1; break;
        case 0xAA:   printf ( "XOR A, D                 "                                  ); op_size = 1; break;
        case 0xAB:   printf ( "XOR A, E                 "                                  ); op_size = 1; break;
        case 0xAC:   printf ( "XOR A, H                 "                                  ); op_size = 1; break;
        case 0xAD:   printf ( "XOR A, L                 "                                  ); op_size = 1; break;
        case 0xAE:   printf ( "XOR A, (HL)              "                                  ); op_size = 1; break;
        case 0xAF:   printf ( "XOR A, A                 "                                  ); op_size = 1; break;

        case 0xB0:   printf ( "OR A, B                  "                                  ); op_size = 1; break;
        case 0xB1:   printf ( "OR A, C                  "                                  ); op_size = 1; break;
        case 0xB2:   printf ( "OR A, D                  "                                  ); op_size = 1; break;
        case 0xB3:   printf ( "OR A, E                  "                                  ); op_size = 1; break;
        case 0xB4:   printf ( "OR A, H                  "                                  ); op_size = 1; break;
        case 0xB5:   printf ( "OR A, L                  "                                  ); op_size = 1; break;
        case 0xB6:   printf ( "OR A, (HL)               "                                  ); op_size = 1; break;
        case 0xB7:   printf ( "OR A, A                  "                                  ); op_size = 1; break;
        case 0xB8:   printf ( "CP A, B                  "                                  ); op_size = 1; break;
        case 0xB9:   printf ( "CP A, C                  "                                  ); op_size = 1; break;
        case 0xBA:   printf ( "CP A, D                  "                                  ); op_size = 1; break;
        case 0xBB:   printf ( "CP A, E                  "                                  ); op_size = 1; break;
        case 0xBC:   printf ( "CP A, H                  "                                  ); op_size = 1; break;
        case 0xBD:   printf ( "CP A, L                  "                                  ); op_size = 1; break;
        case 0xBE:   printf ( "CP A, (HL)               "                                  ); op_size = 1; break;
        case 0xBF:   printf ( "CP A, A                  "                                  ); op_size = 1; break;

        case 0xC0:   printf ( "RET NZ                   "                                  ); op_size = 1; break;
        case 0xC1:   printf ( "POP BC                   "                                  ); op_size = 1; break;
        case 0xC2:   printf ( "JP NZ, 0x%02X%02X            ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0xC3:   printf ( "JP 0x%02X%02X                ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0xC4:   printf ( "CALL NZ, 0x%02X%02X          ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0xC5:   printf ( "PUSH BC                  "                                  ); op_size = 1; break;
        case 0xC6:   printf ( "ADD A, 0x%02X              ",  buffer[pc+1]                 ); op_size = 2; break;
        case 0xC7:   printf ( "RST 0x00                 "                                  ); op_size = 1; break;
        case 0xC8:   printf ( "RET Z                    "                                  ); op_size = 1; break;
        case 0xC9:   printf ( "RET                      "                                  ); op_size = 1; break;
        case 0xCA:   printf ( "JP Z, 0x%02X%02X             ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0xCC:   printf ( "CALL Z, 0x%02X%02X           ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0xCD:   printf ( "CALL 0x%02X%02X              ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0xCE:   printf ( "ADC A, 0x%02X              ",  buffer[pc+1]                 ); op_size = 2; break;
        case 0xCF:   printf ( "RST 0x08                 "                                  ); op_size = 1; break;

        case 0xD0:   printf ( "RET NC                   "                                  ); op_size = 1; break;
        case 0xD1:   printf ( "POP DE                   "                                  ); op_size = 1; break;
        case 0xD2:   printf ( "JP NC, 0x%02X%02X            ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0xD3:   printf ( "-                        "                                  ); op_size = 0; break;
        case 0xD4:   printf ( "CALL NC, 0x%02X%02X          ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0xD5:   printf ( "PUSH DE                  "                                  ); op_size = 1; break;
        case 0xD6:   printf ( "SUB A, 0x%02X              ",       buffer[pc+1]            ); op_size = 2; break;
        case 0xD7:   printf ( "RST 0x10                 "                                  ); op_size = 1; break;
        case 0xD8:   printf ( "RET C                    "                                  ); op_size = 1; break;
        case 0xD9:   printf ( "RETI                     "                                  ); op_size = 1; break;
        case 0xDA:   printf ( "JP C, 0x%02X%02X             ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0xDB:   printf ( "-                        "                                  ); op_size = 0; break;
        case 0xDC:   printf ( "CALL C, 0x%02X%02X           ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0xDD:   printf ( "-                        "                                  ); op_size = 0; break;
        case 0xDE:   printf ( "SBC A, 0x%02X              ",  buffer[pc+1]                 ); op_size = 2; break;
        case 0xDF:   printf ( "RST 0x18                 "                                  ); op_size = 1; break;

        case 0xE0:   printf ( "LD (0xFF00 + 0x%02X), A    ",  buffer[pc+1]                   ); op_size = 2; break;
        case 0xE1:   printf ( "POP HL                   "                                  ); op_size = 1; break;
        case 0xE2:   printf ( "LD (C), A                "                                  ); op_size = 1; break;
        case 0xE3:   printf ( "-                        "                                  ); op_size = 0; break;
        case 0xE4:   printf ( "-                        "                                  ); op_size = 0; break;
        case 0xE5:   printf ( "PUSH HL                  "                                  ); op_size = 1; break;
        case 0xE6:   printf ( "AND A, 0x%02X              ",                 buffer[pc+1]  ); op_size = 2; break;
        case 0xE7:   printf ( "RST 0x20                 "                                  ); op_size = 1; break;
        case 0xE8:   printf ( "ADD SP, %02X               ",     buffer[pc+1]                ); op_size = 2; break;
        case 0xE9:   printf ( "JP HL                    "                                  ); op_size = 1; break;
        case 0xEA:   printf ( "LD (0x%02X%02X), A           ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0xEB:   printf ( "-                        "                                  ); op_size = 0; break;
        case 0xEC:   printf ( "-                        "                                  ); op_size = 0; break;
        case 0xED:   printf ( "-                        "                                  ); op_size = 0; break;
        case 0xEE:   printf ( "XOR A, 0x%02X              ",   buffer[pc+1]                ); op_size = 2; break;
        case 0xEF:   printf ( "RST 0x28                 "                                  ); op_size = 1; break;

        case 0xF0:   printf ( "LD A, (0xFF00 + 0x%02X)    ",   buffer[pc+1]                  ); op_size = 2; break;
        case 0xF1:   printf ( "POP AF                   "                                  ); op_size = 1; break;
        case 0xF2:   printf ( "LD A, (C)                "                                  ); op_size = 3; break;
        case 0xF3:   printf ( "DI                       "                                  ); op_size = 1; break;
        case 0xF4:   printf ( "-                        "                                  ); op_size = 0; break;
        case 0xF5:   printf ( "PUSH AF                  "                                  ); op_size = 1; break;
        case 0xF6:   printf ( "OR A, 0x%02X               ",    buffer[pc+1]               ); op_size = 2; break;
        case 0xF7:   printf ( "RST 0x30                 "                                  ); op_size = 1; break;
        case 0xF8:   printf ( "LD HL, SP + 0x%02X         ",                buffer[pc+1]   ); op_size = 2; break;
        case 0xF9:   printf ( "LD SP, HL                "                                  ); op_size = 1; break;
        case 0xFA:   printf ( "LD A, (0x%02X%02X)           ",  buffer[pc+2], buffer[pc+1] ); op_size = 3; break;
        case 0xFB:   printf ( "EI                       "                                  ); op_size = 1; break;
        case 0xFC:   printf ( "-                        "                                  ); op_size = 0; break;
        case 0xFD:   printf ( "-                        "                                  ); op_size = 0; break;
        case 0xFE:   printf ( "CP A, 0x%02X               ",        buffer[pc+1]           ); op_size = 2; break;
        case 0xFF:   printf ( "RST 0x38                 "                                  ); op_size = 1; break;

        case 0xCB:
            switch ( buffer[pc+1] ) {
                case 0x00: printf ( "RLC B                    " ); op_size = 1; break;
                case 0x01: printf ( "RLC C                    " ); op_size = 1; break;
                case 0x02: printf ( "RLC D                    " ); op_size = 1; break;
                case 0x03: printf ( "RLC E                    " ); op_size = 1; break;
                case 0x04: printf ( "RLC H                    " ); op_size = 1; break;
                case 0x05: printf ( "RLC L                    " ); op_size = 1; break;
                case 0x06: printf ( "RLC (HL)                 " ); op_size = 1; break;
                case 0x07: printf ( "RLC A                    " ); op_size = 1; break;
                case 0x08: printf ( "RRC B                    " ); op_size = 1; break;
                case 0x09: printf ( "RRC C                    " ); op_size = 1; break;
                case 0x0A: printf ( "RRC D                    " ); op_size = 1; break;
                case 0x0B: printf ( "RRC E                    " ); op_size = 1; break;
                case 0x0C: printf ( "RRC H                    " ); op_size = 1; break;
                case 0x0D: printf ( "RRC L                    " ); op_size = 1; break;
                case 0x0E: printf ( "RRC (HL)                 " ); op_size = 1; break;
                case 0x0F: printf ( "RRC A                    " ); op_size = 1; break;

                case 0x10: printf ( "RL B                     " ); op_size = 1; break;
                case 0x11: printf ( "RL C                     " ); op_size = 1; break;
                case 0x12: printf ( "RL D                     " ); op_size = 1; break;
                case 0x13: printf ( "RL E                     " ); op_size = 1; break;
                case 0x14: printf ( "RL H                     " ); op_size = 1; break;
                case 0x15: printf ( "RL L                     " ); op_size = 1; break;
                case 0x16: printf ( "RL (HL)                  " ); op_size = 1; break;
                case 0x17: printf ( "RL A                     " ); op_size = 1; break;
                case 0x18: printf ( "RR B                     " ); op_size = 1; break;
                case 0x19: printf ( "RR C                     " ); op_size = 1; break;
                case 0x1A: printf ( "RR D                     " ); op_size = 1; break;
                case 0x1B: printf ( "RR E                     " ); op_size = 1; break;
                case 0x1C: printf ( "RR H                     " ); op_size = 1; break;
                case 0x1D: printf ( "RR L                     " ); op_size = 1; break;
                case 0x1E: printf ( "RR (HL)                  " ); op_size = 1; break;
                case 0x1F: printf ( "RR A                     " ); op_size = 1; break;

                case 0x20: printf ( "SLA B                    " ); op_size = 1; break;
                case 0x21: printf ( "SLA C                    " ); op_size = 1; break;
                case 0x22: printf ( "SLA D                    " ); op_size = 1; break;
                case 0x23: printf ( "SLA E                    " ); op_size = 1; break;
                case 0x24: printf ( "SLA H                    " ); op_size = 1; break;
                case 0x25: printf ( "SLA L                    " ); op_size = 1; break;
                case 0x26: printf ( "SLA (HL)                 " ); op_size = 1; break;
                case 0x27: printf ( "SLA A                    " ); op_size = 1; break;
                case 0x28: printf ( "SRA B                    " ); op_size = 1; break;
                case 0x29: printf ( "SRA C                    " ); op_size = 1; break;
                case 0x2A: printf ( "SRA D                    " ); op_size = 1; break;
                case 0x2B: printf ( "SRA E                    " ); op_size = 1; break;
                case 0x2C: printf ( "SRA H                    " ); op_size = 1; break;
                case 0x2D: printf ( "SRA L                    " ); op_size = 1; break;
                case 0x2E: printf ( "SRA (HL)                 " ); op_size = 1; break;
                case 0x2F: printf ( "SRA A                    " ); op_size = 1; break;

                case 0x30: printf ( "SWAP B                   " ); op_size = 1; break;
                case 0x31: printf ( "SWAP C                   " ); op_size = 1; break;
                case 0x32: printf ( "SWAP D                   " ); op_size = 1; break;
                case 0x33: printf ( "SWAP E                   " ); op_size = 1; break;
                case 0x34: printf ( "SWAP H                   " ); op_size = 1; break;
                case 0x35: printf ( "SWAP L                   " ); op_size = 1; break;
                case 0x36: printf ( "SWAP (HL)                " ); op_size = 1; break;
                case 0x37: printf ( "SWAP A                   " ); op_size = 1; break;
                case 0x38: printf ( "SRL B                    " ); op_size = 1; break;
                case 0x39: printf ( "SRL C                    " ); op_size = 1; break;
                case 0x3A: printf ( "SRL D                    " ); op_size = 1; break;
                case 0x3B: printf ( "SRL E                    " ); op_size = 1; break;
                case 0x3C: printf ( "SRL H                    " ); op_size = 1; break;
                case 0x3D: printf ( "SRL L                    " ); op_size = 1; break;
                case 0x3E: printf ( "SRL (HL)                 " ); op_size = 1; break;
                case 0x3F: printf ( "SRL A                    " ); op_size = 1; break;

                case 0x40: printf ( "BIT 0, B                 " ); op_size = 1; break;
                case 0x41: printf ( "BIT 0, C                 " ); op_size = 1; break;
                case 0x42: printf ( "BIT 0, D                 " ); op_size = 1; break;
                case 0x43: printf ( "BIT 0, E                 " ); op_size = 1; break;
                case 0x44: printf ( "BIT 0, H                 " ); op_size = 1; break;
                case 0x45: printf ( "BIT 0, L                 " ); op_size = 1; break;
                case 0x46: printf ( "BIT 0, (HL)              " ); op_size = 1; break;
                case 0x47: printf ( "BIT 0, A                 " ); op_size = 1; break;
                case 0x48: printf ( "BIT 1, B                 " ); op_size = 1; break;
                case 0x49: printf ( "BIT 1, C                 " ); op_size = 1; break;
                case 0x4A: printf ( "BIT 1, D                 " ); op_size = 1; break;
                case 0x4B: printf ( "BIT 1, E                 " ); op_size = 1; break;
                case 0x4C: printf ( "BIT 1, H                 " ); op_size = 1; break;
                case 0x4D: printf ( "BIT 1, L                 " ); op_size = 1; break;
                case 0x4E: printf ( "BIT 1, (HL)              " ); op_size = 1; break;
                case 0x4F: printf ( "BIT 1, A                 " ); op_size = 1; break;

                case 0x50: printf ( "BIT 2, B                 " ); op_size = 1; break;
                case 0x51: printf ( "BIT 2, C                 " ); op_size = 1; break;
                case 0x52: printf ( "BIT 2, D                 " ); op_size = 1; break;
                case 0x53: printf ( "BIT 2, E                 " ); op_size = 1; break;
                case 0x54: printf ( "BIT 2, H                 " ); op_size = 1; break;
                case 0x55: printf ( "BIT 2, L                 " ); op_size = 1; break;
                case 0x56: printf ( "BIT 2, (HL)              " ); op_size = 1; break;
                case 0x57: printf ( "BIT 2, A                 " ); op_size = 1; break;
                case 0x58: printf ( "BIT 3, B                 " ); op_size = 1; break;
                case 0x59: printf ( "BIT 3, C                 " ); op_size = 1; break;
                case 0x5A: printf ( "BIT 3, D                 " ); op_size = 1; break;
                case 0x5B: printf ( "BIT 3, E                 " ); op_size = 1; break;
                case 0x5C: printf ( "BIT 3, H                 " ); op_size = 1; break;
                case 0x5D: printf ( "BIT 3, L                 " ); op_size = 1; break;
                case 0x5E: printf ( "BIT 3, (HL)              " ); op_size = 1; break;
                case 0x5F: printf ( "BIT 3, A                 " ); op_size = 1; break;

                case 0x60: printf ( "BIT 4, B                 " ); op_size = 1; break;
                case 0x61: printf ( "BIT 4, C                 " ); op_size = 1; break;
                case 0x62: printf ( "BIT 4, D                 " ); op_size = 1; break;
                case 0x63: printf ( "BIT 4, E                 " ); op_size = 1; break;
                case 0x64: printf ( "BIT 4, H                 " ); op_size = 1; break;
                case 0x65: printf ( "BIT 4, L                 " ); op_size = 1; break;
                case 0x66: printf ( "BIT 4, (HL)              " ); op_size = 1; break;
                case 0x67: printf ( "BIT 4, A                 " ); op_size = 1; break;
                case 0x68: printf ( "BIT 5, B                 " ); op_size = 1; break;
                case 0x69: printf ( "BIT 5, C                 " ); op_size = 1; break;
                case 0x6A: printf ( "BIT 5, D                 " ); op_size = 1; break;
                case 0x6B: printf ( "BIT 5, E                 " ); op_size = 1; break;
                case 0x6C: printf ( "BIT 5, H                 " ); op_size = 1; break;
                case 0x6D: printf ( "BIT 5, L                 " ); op_size = 1; break;
                case 0x6E: printf ( "BIT 5, (HL)              " ); op_size = 1; break;
                case 0x6F: printf ( "BIT 5, A                 " ); op_size = 1; break;

                case 0x70: printf ( "BIT 6, B                 " ); op_size = 1; break;
                case 0x71: printf ( "BIT 6, C                 " ); op_size = 1; break;
                case 0x72: printf ( "BIT 6, D                 " ); op_size = 1; break;
                case 0x73: printf ( "BIT 6, E                 " ); op_size = 1; break;
                case 0x74: printf ( "BIT 6, H                 " ); op_size = 1; break;
                case 0x75: printf ( "BIT 6, L                 " ); op_size = 1; break;
                case 0x76: printf ( "BIT 6, (HL)              " ); op_size = 1; break;
                case 0x77: printf ( "BIT 6, A                 " ); op_size = 1; break;
                case 0x78: printf ( "BIT 7, B                 " ); op_size = 1; break;
                case 0x79: printf ( "BIT 7, C                 " ); op_size = 1; break;
                case 0x7A: printf ( "BIT 7, D                 " ); op_size = 1; break;
                case 0x7B: printf ( "BIT 7, E                 " ); op_size = 1; break;
                case 0x7C: printf ( "BIT 7, H                 " ); op_size = 1; break;
                case 0x7D: printf ( "BIT 7, L                 " ); op_size = 1; break;
                case 0x7E: printf ( "BIT 7, (HL)              " ); op_size = 1; break;
                case 0x7F: printf ( "BIT 7, A                 " ); op_size = 1; break;

                case 0x80: printf ( "RES 0, B                 " ); op_size = 1; break;
                case 0x81: printf ( "RES 0, C                 " ); op_size = 1; break;
                case 0x82: printf ( "RES 0, D                 " ); op_size = 1; break;
                case 0x83: printf ( "RES 0, E                 " ); op_size = 1; break;
                case 0x84: printf ( "RES 0, H                 " ); op_size = 1; break;
                case 0x85: printf ( "RES 0, L                 " ); op_size = 1; break;
                case 0x86: printf ( "RES 0, (HL)              " ); op_size = 1; break;
                case 0x87: printf ( "RES 0, A                 " ); op_size = 1; break;
                case 0x88: printf ( "RES 1, B                 " ); op_size = 1; break;
                case 0x89: printf ( "RES 1, C                 " ); op_size = 1; break;
                case 0x8A: printf ( "RES 1, D                 " ); op_size = 1; break;
                case 0x8B: printf ( "RES 1, E                 " ); op_size = 1; break;
                case 0x8C: printf ( "RES 1, H                 " ); op_size = 1; break;
                case 0x8D: printf ( "RES 1, L                 " ); op_size = 1; break;
                case 0x8E: printf ( "RES 1, (HL)              " ); op_size = 1; break;
                case 0x8F: printf ( "RES 1, A                 " ); op_size = 1; break;

                case 0x90: printf ( "RES 2, B                 " ); op_size = 1; break;
                case 0x91: printf ( "RES 2, C                 " ); op_size = 1; break;
                case 0x92: printf ( "RES 2, D                 " ); op_size = 1; break;
                case 0x93: printf ( "RES 2, E                 " ); op_size = 1; break;
                case 0x94: printf ( "RES 2, H                 " ); op_size = 1; break;
                case 0x95: printf ( "RES 2, L                 " ); op_size = 1; break;
                case 0x96: printf ( "RES 2, (HL)              " ); op_size = 1; break;
                case 0x97: printf ( "RES 2, A                 " ); op_size = 1; break;
                case 0x98: printf ( "RES 3, B                 " ); op_size = 1; break;
                case 0x99: printf ( "RES 3, C                 " ); op_size = 1; break;
                case 0x9A: printf ( "RES 3, D                 " ); op_size = 1; break;
                case 0x9B: printf ( "RES 3, E                 " ); op_size = 1; break;
                case 0x9C: printf ( "RES 3, H                 " ); op_size = 1; break;
                case 0x9D: printf ( "RES 3, L                 " ); op_size = 1; break;
                case 0x9E: printf ( "RES 3, (HL)              " ); op_size = 1; break;
                case 0x9F: printf ( "RES 3, A                 " ); op_size = 1; break;

                case 0xA0: printf ( "RES 4, B                 " ); op_size = 1; break;
                case 0xA1: printf ( "RES 4, C                 " ); op_size = 1; break;
                case 0xA2: printf ( "RES 4, D                 " ); op_size = 1; break;
                case 0xA3: printf ( "RES 4, E                 " ); op_size = 1; break;
                case 0xA4: printf ( "RES 4, H                 " ); op_size = 1; break;
                case 0xA5: printf ( "RES 4, L                 " ); op_size = 1; break;
                case 0xA6: printf ( "RES 4, (HL)              " ); op_size = 1; break;
                case 0xA7: printf ( "RES 4, A                 " ); op_size = 1; break;
                case 0xA8: printf ( "RES 5, B                 " ); op_size = 1; break;
                case 0xA9: printf ( "RES 5, C                 " ); op_size = 1; break;
                case 0xAA: printf ( "RES 5, D                 " ); op_size = 1; break;
                case 0xAB: printf ( "RES 5, E                 " ); op_size = 1; break;
                case 0xAC: printf ( "RES 5, H                 " ); op_size = 1; break;
                case 0xAD: printf ( "RES 5, L                 " ); op_size = 1; break;
                case 0xAE: printf ( "RES 5, (HL)              " ); op_size = 1; break;
                case 0xAF: printf ( "RES 5, A                 " ); op_size = 1; break;

                case 0xB0: printf ( "RES 6, B                 " ); op_size = 1; break;
                case 0xB1: printf ( "RES 6, C                 " ); op_size = 1; break;
                case 0xB2: printf ( "RES 6, D                 " ); op_size = 1; break;
                case 0xB3: printf ( "RES 6, E                 " ); op_size = 1; break;
                case 0xB4: printf ( "RES 6, H                 " ); op_size = 1; break;
                case 0xB5: printf ( "RES 6, L                 " ); op_size = 1; break;
                case 0xB6: printf ( "RES 6, (HL)              " ); op_size = 1; break;
                case 0xB7: printf ( "RES 6, A                 " ); op_size = 1; break;
                case 0xB8: printf ( "RES 7, B                 " ); op_size = 1; break;
                case 0xB9: printf ( "RES 7, C                 " ); op_size = 1; break;
                case 0xBA: printf ( "RES 7, D                 " ); op_size = 1; break;
                case 0xBB: printf ( "RES 7, E                 " ); op_size = 1; break;
                case 0xBC: printf ( "RES 7, H                 " ); op_size = 1; break;
                case 0xBD: printf ( "RES 7, L                 " ); op_size = 1; break;
                case 0xBE: printf ( "RES 7, (HL)              " ); op_size = 1; break;
                case 0xBF: printf ( "RES 7, A                 " ); op_size = 1; break;

                case 0xC0: printf ( "SET 0, B                 " ); op_size = 1; break;
                case 0xC1: printf ( "SET 0, C                 " ); op_size = 1; break;
                case 0xC2: printf ( "SET 0, D                 " ); op_size = 1; break;
                case 0xC3: printf ( "SET 0, E                 " ); op_size = 1; break;
                case 0xC4: printf ( "SET 0, H                 " ); op_size = 1; break;
                case 0xC5: printf ( "SET 0, L                 " ); op_size = 1; break;
                case 0xC6: printf ( "SET 0, (HL)              " ); op_size = 1; break;
                case 0xC7: printf ( "SET 0, A                 " ); op_size = 1; break;
                case 0xC8: printf ( "SET 1, B                 " ); op_size = 1; break;
                case 0xC9: printf ( "SET 1, C                 " ); op_size = 1; break;
                case 0xCA: printf ( "SET 1, D                 " ); op_size = 1; break;
                case 0xCB: printf ( "SET 1, E                 " ); op_size = 1; break;
                case 0xCC: printf ( "SET 1, H                 " ); op_size = 1; break;
                case 0xCD: printf ( "SET 1, L                 " ); op_size = 1; break;
                case 0xCE: printf ( "SET 1, (HL)              " ); op_size = 1; break;
                case 0xCF: printf ( "SET 1, A                 " ); op_size = 1; break;

                case 0xD0: printf ( "SET 2, B                 " ); op_size = 1; break;
                case 0xD1: printf ( "SET 2, C                 " ); op_size = 1; break;
                case 0xD2: printf ( "SET 2, D                 " ); op_size = 1; break;
                case 0xD3: printf ( "SET 2, E                 " ); op_size = 1; break;
                case 0xD4: printf ( "SET 2, H                 " ); op_size = 1; break;
                case 0xD5: printf ( "SET 2, L                 " ); op_size = 1; break;
                case 0xD6: printf ( "SET 2, (HL)              " ); op_size = 1; break;
                case 0xD7: printf ( "SET 2, A                 " ); op_size = 1; break;
                case 0xD8: printf ( "SET 3, B                 " ); op_size = 1; break;
                case 0xD9: printf ( "SET 3, C                 " ); op_size = 1; break;
                case 0xDA: printf ( "SET 3, D                 " ); op_size = 1; break;
                case 0xDB: printf ( "SET 3, E                 " ); op_size = 1; break;
                case 0xDC: printf ( "SET 3, H                 " ); op_size = 1; break;
                case 0xDD: printf ( "SET 3, L                 " ); op_size = 1; break;
                case 0xDE: printf ( "SET 3, (HL)              " ); op_size = 1; break;
                case 0xDF: printf ( "SET 3, A                 " ); op_size = 1; break;

                case 0xE0: printf ( "SET 4, B                 " ); op_size = 1; break;
                case 0xE1: printf ( "SET 4, C                 " ); op_size = 1; break;
                case 0xE2: printf ( "SET 4, D                 " ); op_size = 1; break;
                case 0xE3: printf ( "SET 4, E                 " ); op_size = 1; break;
                case 0xE4: printf ( "SET 4, H                 " ); op_size = 1; break;
                case 0xE5: printf ( "SET 4, L                 " ); op_size = 1; break;
                case 0xE6: printf ( "SET 4, (HL)              " ); op_size = 1; break;
                case 0xE7: printf ( "SET 4, A                 " ); op_size = 1; break;
                case 0xE8: printf ( "SET 5, B                 " ); op_size = 1; break;
                case 0xE9: printf ( "SET 5, C                 " ); op_size = 1; break;
                case 0xEA: printf ( "SET 5, D                 " ); op_size = 1; break;
                case 0xEB: printf ( "SET 5, E                 " ); op_size = 1; break;
                case 0xEC: printf ( "SET 5, H                 " ); op_size = 1; break;
                case 0xED: printf ( "SET 5, L                 " ); op_size = 1; break;
                case 0xEE: printf ( "SET 5, (HL)              " ); op_size = 1; break;
                case 0xEF: printf ( "SET 5, A                 " ); op_size = 1; break;

                case 0xF0: printf ( "SET 6, B                 " ); op_size = 1; break;
                case 0xF1: printf ( "SET 6, C                 " ); op_size = 1; break;
                case 0xF2: printf ( "SET 6, D                 " ); op_size = 1; break;
                case 0xF3: printf ( "SET 6, E                 " ); op_size = 1; break;
                case 0xF4: printf ( "SET 6, H                 " ); op_size = 1; break;
                case 0xF5: printf ( "SET 6, L                 " ); op_size = 1; break;
                case 0xF6: printf ( "SET 6, (HL)              " ); op_size = 1; break;
                case 0xF7: printf ( "SET 6, A                 " ); op_size = 1; break;
                case 0xF8: printf ( "SET 7, B                 " ); op_size = 1; break;
                case 0xF9: printf ( "SET 7, C                 " ); op_size = 1; break;
                case 0xFA: printf ( "SET 7, D                 " ); op_size = 1; break;
                case 0xFB: printf ( "SET 7, E                 " ); op_size = 1; break;
                case 0xFC: printf ( "SET 7, H                 " ); op_size = 1; break;
                case 0xFD: printf ( "SET 7, L                 " ); op_size = 1; break;
                case 0xFE: printf ( "SET 7, (HL)              " ); op_size = 1; break;
                case 0xFF: printf ( "SET 7, A                 " ); op_size = 1; break;
            }
            break;

        default:
            printf ( " %04x is not implemented " , buffer[pc] );
            break;
    }

    return op_size;
}

#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "disassembler.h"

unsigned short int disassembler( unsigned char *buffer, unsigned int pc ) {
    unsigned short int op_size = 1;

    switch ( buffer[pc] ) {
        case 0x00: printf( " %08X : %02X \t NOP\n"             , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x01: printf( " %08X : %02X \t LXI %X %X\n"       , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x02: printf( " %08X : %02X \t STAX B\n"          , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x03: printf( " %08X : %02X \t INX B\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x04: printf( " %08X : %02X \t INR B\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x05: printf( " %08X : %02X \t DCR B\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x06: printf( " %08X : %02X \t MVI B, %X\n"       , pc, buffer[pc], buffer[pc+1]               ) ; op_size = 2; break;
        case 0x07: printf( " %08X : %02X \t RLC\n"             , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x08: printf( " %08X : %02X \t -\n"               , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x09: printf( " %08X : %02X \t DAD B\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;

        case 0x0a: printf( " %08X : %02X \t LDAX B\n"          , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x0b: printf( " %08X : %02X \t DCX B\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x0c: printf( " %08X : %02X \t INR C\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x0d: printf( " %08X : %02X \t DCR C\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x0e: printf( " %08X : %02X \t MVI C, %X\n"       , pc, buffer[pc], buffer[pc+1]               ) ; op_size = 1; break;
        case 0x0f: printf( " %08X : %02X \t RRC B\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;

        case 0x10: printf( " %08X : %02X \t -\n"               , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x11: printf( " %08X : %02X \t LXI D,  %X %X\n"   , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x12: printf( " %08X : %02X \t STAX D\n"          , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x13: printf( " %08X : %02X \t INX D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x14: printf( " %08X : %02X \t INR D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x15: printf( " %08X : %02X \t DCR D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x16: printf( " %08X : %02X \t MVI D, %X\n"       , pc, buffer[pc], buffer[pc+1]               ) ; op_size = 2; break;
        case 0x17: printf( " %08X : %02X \t RAL\n"             , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x18: printf( " %08X : %02X \t -\n"               , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x19: printf( " %08X : %02X \t DAD D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;

        case 0x1a: printf( " %08X : %02X \t LDAX D\n"          , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x1b: printf( " %08X : %02X \t DCX D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x1c: printf( " %08X : %02X \t INR D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x1d: printf( " %08X : %02X \t DCR D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x1e: printf( " %08X : %02X \t MVI D, %X\n"       , pc, buffer[pc], buffer[pc+1]               ) ; op_size = 1; break;
        case 0x1f: printf( " %08X : %02X \t RAR\n"             , pc, buffer[pc]                             ) ; op_size = 1; break;

        case 0x20: printf( " %08X : %02X \t RIM\n"             , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x21: printf( " %08X : %02X \t LXI H,  %X %X\n"   , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x22: printf( " %08X : %02X \t SHLD %X %X\n"      , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x23: printf( " %08X : %02X \t INX H\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x24: printf( " %08X : %02X \t INR H\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x25: printf( " %08X : %02X \t DCR H\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x26: printf( " %08X : %02X \t MVI H, %X\n"       , pc, buffer[pc], buffer[pc+1]               ) ; op_size = 2; break;
        case 0x27: printf( " %08X : %02X \t DAA\n"             , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x28: printf( " %08X : %02X \t -\n"               , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x29: printf( " %08X : %02X \t DAD H\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;

        case 0x2a: printf( " %08X : %02X \t LHLD %X %X\n"      , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x2b: printf( " %08X : %02X \t DCX H\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x2c: printf( " %08X : %02X \t INR L\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x2d: printf( " %08X : %02X \t DCR L\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x2e: printf( " %08X : %02X \t MVI L, %X\n"       , pc, buffer[pc], buffer[pc+1]               ) ; op_size = 1; break;
        case 0x2f: printf( " %08X : %02X \t CMA\n"             , pc, buffer[pc]                             ) ; op_size = 1; break;

        case 0x30: printf( " %08X : %02X \t SIM\n"             , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x31: printf( " %08X : %02X \t LXI SP,  %X %X\n"  , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x32: printf( " %08X : %02X \t STA %X %X\n"       , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x33: printf( " %08X : %02X \t INX SP\n"          , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x34: printf( " %08X : %02X \t INR M\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x35: printf( " %08X : %02X \t DCR M\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x36: printf( " %08X : %02X \t MVI M, %X\n"       , pc, buffer[pc], buffer[pc+1]               ) ; op_size = 2; break;
        case 0x37: printf( " %08X : %02X \t STC\n"             , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x38: printf( " %08X : %02X \t -\n"               , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x39: printf( " %08X : %02X \t DAD SP\n"          , pc, buffer[pc]                             ) ; op_size = 1; break;

        case 0x3a: printf( " %08X : %02X \t LDA %X %X\n"       , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x3b: printf( " %08X : %02X \t DCX SP\n"          , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x3c: printf( " %08X : %02X \t INR A\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x3d: printf( " %08X : %02X \t DCR A\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0x3e: printf( " %08X : %02X \t MVI A, %X\n"       , pc, buffer[pc], buffer[pc+1]               ) ; op_size = 1; break;
        case 0x3f: printf( " %08X : %02X \t STC\n"             , pc, buffer[pc]                             ) ; op_size = 1; break;

        case 0x40: printf( " %08X : %02X \t MOV B, B\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x41: printf( " %08X : %02X \t MOV B, C\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x42: printf( " %08X : %02X \t MOV B, D\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x43: printf( " %08X : %02X \t MOV B, E\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x44: printf( " %08X : %02X \t MOV B, H\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x45: printf( " %08X : %02X \t MOV B, L\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x46: printf( " %08X : %02X \t MOV B, M\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x47: printf( " %08X : %02X \t MOV B, A\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x48: printf( " %08X : %02X \t MOV C, B\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x49: printf( " %08X : %02X \t MOV C, C\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x4a: printf( " %08X : %02X \t MOV C, D\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x4b: printf( " %08X : %02X \t MOV C, E\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x4c: printf( " %08X : %02X \t MOV C, H\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x4d: printf( " %08X : %02X \t MOV C, L\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x4e: printf( " %08X : %02X \t MOV C, M\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x4f: printf( " %08X : %02X \t MOV C, A\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;

        case 0x50: printf( " %08X : %02X \t MOV D, B\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x51: printf( " %08X : %02X \t MOV D, C\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x52: printf( " %08X : %02X \t MOV D, D\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x53: printf( " %08X : %02X \t MOV D, E\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x54: printf( " %08X : %02X \t MOV D, H\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x55: printf( " %08X : %02X \t MOV D, L\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x56: printf( " %08X : %02X \t MOV D, M\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x57: printf( " %08X : %02X \t MOV D, A\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x58: printf( " %08X : %02X \t MOV E, B\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x59: printf( " %08X : %02X \t MOV E, C\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x5a: printf( " %08X : %02X \t MOV E, D\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x5b: printf( " %08X : %02X \t MOV E, E\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x5c: printf( " %08X : %02X \t MOV E, H\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x5d: printf( " %08X : %02X \t MOV E, L\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x5e: printf( " %08X : %02X \t MOV E, M\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x5f: printf( " %08X : %02X \t MOV E, A\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;

        case 0x60: printf( " %08X : %02X \t MOV H, B\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x61: printf( " %08X : %02X \t MOV H, C\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x62: printf( " %08X : %02X \t MOV H, D\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x63: printf( " %08X : %02X \t MOV H, E\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x64: printf( " %08X : %02X \t MOV H, H\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x65: printf( " %08X : %02X \t MOV H, L\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x66: printf( " %08X : %02X \t MOV H, M\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x67: printf( " %08X : %02X \t MOV H, A\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x68: printf( " %08X : %02X \t MOV L, B\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x69: printf( " %08X : %02X \t MOV L, C\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x6a: printf( " %08X : %02X \t MOV L, D\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x6b: printf( " %08X : %02X \t MOV L, E\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x6c: printf( " %08X : %02X \t MOV L, H\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x6d: printf( " %08X : %02X \t MOV L, L\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x6e: printf( " %08X : %02X \t MOV L, M\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x6f: printf( " %08X : %02X \t MOV L, A\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;

        case 0x70: printf( " %08X : %02X \t MOV M, B\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x71: printf( " %08X : %02X \t MOV M, C\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x72: printf( " %08X : %02X \t MOV M, D\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x73: printf( " %08X : %02X \t MOV M, E\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x74: printf( " %08X : %02X \t MOV M, H\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x75: printf( " %08X : %02X \t MOV M, L\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x76: printf( " %08X : %02X \t HTL\n"             , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x77: printf( " %08X : %02X \t MOV M, A\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x78: printf( " %08X : %02X \t MOV A, B\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x79: printf( " %08X : %02X \t MOV A, C\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x7a: printf( " %08X : %02X \t MOV A, D\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x7b: printf( " %08X : %02X \t MOV A, E\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x7c: printf( " %08X : %02X \t MOV A, H\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x7d: printf( " %08X : %02X \t MOV A, L\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x7e: printf( " %08X : %02X \t MOV A, M\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x7f: printf( " %08X : %02X \t MOV A, A\n"        , pc, buffer[pc]                             ) ; op_size = 1; break ;

        case 0x80: printf( " %08X : %02X \t ADD B\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x81: printf( " %08X : %02X \t ADD C\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x82: printf( " %08X : %02X \t ADD D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x83: printf( " %08X : %02X \t ADD E\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x84: printf( " %08X : %02X \t ADD H\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x85: printf( " %08X : %02X \t ADD L\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x86: printf( " %08X : %02X \t ADD M\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x87: printf( " %08X : %02X \t ADD A\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x88: printf( " %08X : %02X \t ADC B\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x89: printf( " %08X : %02X \t ADC C\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x8a: printf( " %08X : %02X \t ADC D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x8b: printf( " %08X : %02X \t ADC E\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x8c: printf( " %08X : %02X \t ADC H\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x8d: printf( " %08X : %02X \t ADC L\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x8e: printf( " %08X : %02X \t ADC M\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x8f: printf( " %08X : %02X \t ADC A\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;

        case 0x90: printf( " %08X : %02X \t SUB B\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x91: printf( " %08X : %02X \t SUB C\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x92: printf( " %08X : %02X \t SUB D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x93: printf( " %08X : %02X \t SUB E\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x94: printf( " %08X : %02X \t SUB H\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x95: printf( " %08X : %02X \t SUB L\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x96: printf( " %08X : %02X \t SUB M\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x97: printf( " %08X : %02X \t SUB A\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x98: printf( " %08X : %02X \t SBB B\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x99: printf( " %08X : %02X \t SBB C\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x9a: printf( " %08X : %02X \t SBB D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x9b: printf( " %08X : %02X \t SBB E\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x9c: printf( " %08X : %02X \t SBB H\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x9d: printf( " %08X : %02X \t SBB L\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x9e: printf( " %08X : %02X \t SBB M\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0x9f: printf( " %08X : %02X \t SBB A\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;

        case 0xa0: printf( " %08X : %02X \t ANA B\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xa1: printf( " %08X : %02X \t ANA C\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xa2: printf( " %08X : %02X \t ANA D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xa3: printf( " %08X : %02X \t ANA E\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xa4: printf( " %08X : %02X \t ANA H\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xa5: printf( " %08X : %02X \t ANA L\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xa6: printf( " %08X : %02X \t ANA M\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xa7: printf( " %08X : %02X \t ANA A\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xa8: printf( " %08X : %02X \t XRA B\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xa9: printf( " %08X : %02X \t XRA C\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xaa: printf( " %08X : %02X \t XRA D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xab: printf( " %08X : %02X \t XRA E\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xac: printf( " %08X : %02X \t XRA H\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xad: printf( " %08X : %02X \t XRA L\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xae: printf( " %08X : %02X \t XRA M\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xaf: printf( " %08X : %02X \t XRA A\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;

        case 0xb0: printf( " %08X : %02X \t ORA B\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xb1: printf( " %08X : %02X \t ORA C\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xb2: printf( " %08X : %02X \t ORA D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xb3: printf( " %08X : %02X \t ORA E\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xb4: printf( " %08X : %02X \t ORA H\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xb5: printf( " %08X : %02X \t ORA L\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xb6: printf( " %08X : %02X \t ORA M\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xb7: printf( " %08X : %02X \t ORA A\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xb8: printf( " %08X : %02X \t CMP B\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xb9: printf( " %08X : %02X \t CMP C\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xba: printf( " %08X : %02X \t CMP D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xbb: printf( " %08X : %02X \t CMP E\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xbc: printf( " %08X : %02X \t CMP H\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xbd: printf( " %08X : %02X \t CMP L\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xbe: printf( " %08X : %02X \t CMP M\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;
        case 0xbf: printf( " %08X : %02X \t CMP A\n"           , pc, buffer[pc]                             ) ; op_size = 1; break ;

        case 0xc0: printf( " %08X : %02X \t RNZ\n"             , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xc1: printf( " %08X : %02X \t POP B\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xc2: printf( " %08X : %02X \t JNZ %X %X\n"       , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xc3: printf( " %08X : %02X \t JMP %X %X\n"       , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xc4: printf( " %08X : %02X \t CNZ %X %X\n"       , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xc5: printf( " %08X : %02X \t PUSH B\n"          , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xc6: printf( " %08X : %02X \t ADI D8\n"          , pc, buffer[pc]                             ) ; op_size = 2; break;
        case 0xc7: printf( " %08X : %02X \t RST 0\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xc8: printf( " %08X : %02X \t RZ\n"              , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xc9: printf( " %08X : %02X \t RET\n"             , pc, buffer[pc]                             ) ; op_size = 1; break;

        case 0xca: printf( " %08X : %02X \t JZ %X %X\n"        , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xcb: printf( " %08X : %02X \t -\n"               , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xcc: printf( " %08X : %02X \t CZ %X %X\n"        , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xcd: printf( " %08X : %02X \t CALL %X %X\n"      , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xce: printf( " %08X : %02X \t ACI D8\n"          , pc, buffer[pc]                             ) ; op_size = 2; break;
        case 0xcf: printf( " %08X : %02X \t RST 1\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;

        case 0xd0: printf( " %08X : %02X \t RNC\n"             , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xd1: printf( " %08X : %02X \t POP D\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xd2: printf( " %08X : %02X \t JNC %X %X\n"       , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xd3: printf( " %08X : %02X \t OUT D8\n"          , pc, buffer[pc]                             ) ; op_size = 2; break;
        case 0xd4: printf( " %08X : %02X \t CNC %X %X\n"       , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xd5: printf( " %08X : %02X \t PUSH D\n"          , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xd6: printf( " %08X : %02X \t SUI D8\n"          , pc, buffer[pc]                             ) ; op_size = 2; break;
        case 0xd7: printf( " %08X : %02X \t RST 2\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xd8: printf( " %08X : %02X \t RC\n"              , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xd9: printf( " %08X : %02X \t -\n"               , pc, buffer[pc]                             ) ; op_size = 1; break;

        case 0xda: printf( " %08X : %02X \t JC %X %X\n"        , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xdb: printf( " %08X : %02X \t IN D8\n"           , pc, buffer[pc]                             ) ; op_size = 2; break;
        case 0xdc: printf( " %08X : %02X \t CC %X %X\n"        , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xdd: printf( " %08X : %02X \t -\n"               , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xde: printf( " %08X : %02X \t SBI D8\n"          , pc, buffer[pc]                             ) ; op_size = 2; break;
        case 0xdf: printf( " %08X : %02X \t RST 3\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;

        case 0xe0: printf( " %08X : %02X \t RPO\n"             , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xe1: printf( " %08X : %02X \t POP H\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xe2: printf( " %08X : %02X \t JPO %X %X\n"       , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xe3: printf( " %08X : %02X \t XTHL\n"            , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xe4: printf( " %08X : %02X \t CPO %X %X\n"       , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xe5: printf( " %08X : %02X \t PUSH H\n"          , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xe6: printf( " %08X : %02X \t ANI D8\n"          , pc, buffer[pc]                             ) ; op_size = 2; break;
        case 0xe7: printf( " %08X : %02X \t RST 4\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xe8: printf( " %08X : %02X \t RPE\n"             , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xe9: printf( " %08X : %02X \t PCHL\n"            , pc, buffer[pc]                             ) ; op_size = 1; break;

        case 0xea: printf( " %08X : %02X \t JPE %X %X\n"       , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xeb: printf( " %08X : %02X \t XCHG\n"            , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xec: printf( " %08X : %02X \t CPE %X %X\n"       , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xed: printf( " %08X : %02X \t -\n"               , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xee: printf( " %08X : %02X \t XRI D8\n"          , pc, buffer[pc]                             ) ; op_size = 2; break;
        case 0xef: printf( " %08X : %02X \t RST 5\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;

        case 0xf0: printf( " %08X : %02X \t RP\n"              , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xf1: printf( " %08X : %02X \t POP PSW\n"         , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xf2: printf( " %08X : %02X \t JP %X %X\n"        , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xf3: printf( " %08X : %02X \t DI\n"              , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xf4: printf( " %08X : %02X \t CP %X %X\n"        , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xf5: printf( " %08X : %02X \t PUSH PSW\n"        , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xf6: printf( " %08X : %02X \t ORI D8\n"          , pc, buffer[pc]                             ) ; op_size = 2; break;
        case 0xf7: printf( " %08X : %02X \t RST 6\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xf8: printf( " %08X : %02X \t RM\n"              , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xf9: printf( " %08X : %02X \t SPHL\n"            , pc, buffer[pc]                             ) ; op_size = 1; break;

        case 0xfa: printf( " %08X : %02X \t JM %X %X\n"        , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xfb: printf( " %08X : %02X \t EI\n"              , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xfc: printf( " %08X : %02X \t CM %X %X\n"        , pc, buffer[pc], buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xfd: printf( " %08X : %02X \t -\n"               , pc, buffer[pc]                             ) ; op_size = 1; break;
        case 0xfe: printf( " %08X : %02X \t CPI D8\n"          , pc, buffer[pc]                             ) ; op_size = 2; break;
        case 0xff: printf( " %08X : %02X \t RST 7\n"           , pc, buffer[pc]                             ) ; op_size = 1; break;

        default:
            printf(" %2X is not implemented\n", buffer[pc]);
            break;
    }

    return op_size;
}

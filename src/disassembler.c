#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "disassembler.h"

unsigned short int disassembler( unsigned char *buffer, unsigned int pc ) {
    unsigned short int op_size = 1;

    switch ( buffer[pc] ) {
        case 0x00: printf( " 0x%04x : %02x "  "       \t NOP\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x01: printf( " 0x%04x : %02x %02x %02x  \t LXI %02x %02x\n"      , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x02: printf( " 0x%04x : %02x "  "       \t STAX B\n"             , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x03: printf( " 0x%04x : %02x "  "       \t INX B\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x04: printf( " 0x%04x : %02x "  "       \t INR B\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x05: printf( " 0x%04x : %02x "  "       \t DCR B\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x06: printf( " 0x%04x : %02x %02x       \t MVI B, %02x\n"        , pc , buffer[pc], buffer[pc+1] , buffer[pc+1]                             ) ; op_size = 2; break;
        case 0x07: printf( " 0x%04x : %02x "  "       \t RLC\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x08: printf( " 0x%04x : %02x "  "       \t -\n"                  , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x09: printf( " 0x%04x : %02x "  "       \t DAD B\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        case 0x0a: printf( " 0x%04x : %02x "  "       \t LDAX B\n"             , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x0b: printf( " 0x%04x : %02x "  "       \t DCX B\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x0c: printf( " 0x%04x : %02x "  "       \t INR C\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x0d: printf( " 0x%04x : %02x "  "       \t DCR C\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x0e: printf( " 0x%04x : %02x %02x       \t MVI C, %02x\n"        , pc , buffer[pc], buffer[pc+1] , buffer[pc+1]                             ) ; op_size = 1; break;
        case 0x0f: printf( " 0x%04x : %02x "  "       \t RRC B\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        case 0x10: printf( " 0x%04x : %02x "  "       \t -\n"                  , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x11: printf( " 0x%04x : %02x %02x %02x  \t LXI D, %02x %02x\n"   , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x12: printf( " 0x%04x : %02x "  "       \t STAX D\n"             , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x13: printf( " 0x%04x : %02x "  "       \t INX D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x14: printf( " 0x%04x : %02x "  "       \t INR D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x15: printf( " 0x%04x : %02x "  "       \t DCR D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x16: printf( " 0x%04x : %02x %02x       \t MVI D, %02x\n"        , pc , buffer[pc], buffer[pc+1] , buffer[pc+1]                             ) ; op_size = 2; break;
        case 0x17: printf( " 0x%04x : %02x "  "       \t RAL\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x18: printf( " 0x%04x : %02x "  "       \t -\n"                  , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x19: printf( " 0x%04x : %02x "  "       \t DAD D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        case 0x1a: printf( " 0x%04x : %02x "  "       \t LDAX D\n"             , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x1b: printf( " 0x%04x : %02x "  "       \t DCX D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x1c: printf( " 0x%04x : %02x "  "       \t INR D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x1d: printf( " 0x%04x : %02x "  "       \t DCR D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x1e: printf( " 0x%04x : %02x %02x       \t MVI D, %02x\n"        , pc , buffer[pc], buffer[pc+1] , buffer[pc+1]                             ) ; op_size = 1; break;
        case 0x1f: printf( " 0x%04x : %02x "  "       \t RAR\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        case 0x20: printf( " 0x%04x : %02x "  "       \t RIM\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x21: printf( " 0x%04x : %02x %02x %02x  \t LXI H,  %02x %02x\n"  , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x22: printf( " 0x%04x : %02x %02x %02x  \t SHLD %02x %02x\n"     , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x23: printf( " 0x%04x : %02x "  "       \t INX H\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x24: printf( " 0x%04x : %02x "  "       \t INR H\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x25: printf( " 0x%04x : %02x "  "       \t DCR H\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x26: printf( " 0x%04x : %02x %02x       \t MVI H, %02x\n"        , pc , buffer[pc], buffer[pc+1] , buffer[pc+1]                             ) ; op_size = 2; break;
        case 0x27: printf( " 0x%04x : %02x "  "       \t DAA\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x28: printf( " 0x%04x : %02x "  "       \t -\n"                  , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x29: printf( " 0x%04x : %02x "  "       \t DAD H\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        case 0x2a: printf( " 0x%04x : %02x %02x %02x  \t LHLD %02x %02x\n"     , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x2b: printf( " 0x%04x : %02x "  "       \t DCX H\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x2c: printf( " 0x%04x : %02x "  "       \t INR L\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x2d: printf( " 0x%04x : %02x "  "       \t DCR L\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x2e: printf( " 0x%04x : %02x %02x       \t MVI L, %02x\n"        , pc , buffer[pc], buffer[pc+1] , buffer[pc+1]                             ) ; op_size = 1; break;
        case 0x2f: printf( " 0x%04x : %02x "  "       \t CMA\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        case 0x30: printf( " 0x%04x : %02x "  "       \t SIM\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x31: printf( " 0x%04x : %02x %02x %02x  \t LXI SP,  %02x %02x\n" , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x32: printf( " 0x%04x : %02x %02x %02x  \t STA %02x %02x\n"      , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x33: printf( " 0x%04x : %02x "  "       \t INX SP\n"             , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x34: printf( " 0x%04x : %02x "  "       \t INR M\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x35: printf( " 0x%04x : %02x "  "       \t DCR M\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x36: printf( " 0x%04x : %02x %02x       \t MVI M, %02x\n"        , pc , buffer[pc], buffer[pc+1] , buffer[pc+1]                             ) ; op_size = 2; break;
        case 0x37: printf( " 0x%04x : %02x "  "       \t STC\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x38: printf( " 0x%04x : %02x "  "       \t -\n"                  , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x39: printf( " 0x%04x : %02x "  "       \t DAD SP\n"             , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        case 0x3a: printf( " 0x%04x : %02x %02x %02x  \t LDA %02x %02x\n"      , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0x3b: printf( " 0x%04x : %02x "  "       \t DCX SP\n"             , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x3c: printf( " 0x%04x : %02x "  "       \t INR A\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x3d: printf( " 0x%04x : %02x "  "       \t DCR A\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0x3e: printf( " 0x%04x : %02x %02x       \t MVI A, %02x\n"        , pc , buffer[pc], buffer[pc+1] , buffer[pc+1]                             ) ; op_size = 1; break;
        case 0x3f: printf( " 0x%04x : %02x "  "       \t STC\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        case 0x40: printf( " 0x%04x : %02x "  "       \t MOV B, B\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x41: printf( " 0x%04x : %02x "  "       \t MOV B, C\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x42: printf( " 0x%04x : %02x "  "       \t MOV B, D\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x43: printf( " 0x%04x : %02x "  "       \t MOV B, E\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x44: printf( " 0x%04x : %02x "  "       \t MOV B, H\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x45: printf( " 0x%04x : %02x "  "       \t MOV B, L\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x46: printf( " 0x%04x : %02x "  "       \t MOV B, M\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x47: printf( " 0x%04x : %02x "  "       \t MOV B, A\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x48: printf( " 0x%04x : %02x "  "       \t MOV C, B\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x49: printf( " 0x%04x : %02x "  "       \t MOV C, C\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x4a: printf( " 0x%04x : %02x "  "       \t MOV C, D\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x4b: printf( " 0x%04x : %02x "  "       \t MOV C, E\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x4c: printf( " 0x%04x : %02x "  "       \t MOV C, H\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x4d: printf( " 0x%04x : %02x "  "       \t MOV C, L\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x4e: printf( " 0x%04x : %02x "  "       \t MOV C, M\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x4f: printf( " 0x%04x : %02x "  "       \t MOV C, A\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;

        case 0x50: printf( " 0x%04x : %02x "  "       \t MOV D, B\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x51: printf( " 0x%04x : %02x "  "       \t MOV D, C\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x52: printf( " 0x%04x : %02x "  "       \t MOV D, D\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x53: printf( " 0x%04x : %02x "  "       \t MOV D, E\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x54: printf( " 0x%04x : %02x "  "       \t MOV D, H\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x55: printf( " 0x%04x : %02x "  "       \t MOV D, L\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x56: printf( " 0x%04x : %02x "  "       \t MOV D, M\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x57: printf( " 0x%04x : %02x "  "       \t MOV D, A\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x58: printf( " 0x%04x : %02x "  "       \t MOV E, B\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x59: printf( " 0x%04x : %02x "  "       \t MOV E, C\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x5a: printf( " 0x%04x : %02x "  "       \t MOV E, D\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x5b: printf( " 0x%04x : %02x "  "       \t MOV E, E\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x5c: printf( " 0x%04x : %02x "  "       \t MOV E, H\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x5d: printf( " 0x%04x : %02x "  "       \t MOV E, L\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x5e: printf( " 0x%04x : %02x "  "       \t MOV E, M\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x5f: printf( " 0x%04x : %02x "  "       \t MOV E, A\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;

        case 0x60: printf( " 0x%04x : %02x "  "       \t MOV H, B\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x61: printf( " 0x%04x : %02x "  "       \t MOV H, C\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x62: printf( " 0x%04x : %02x "  "       \t MOV H, D\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x63: printf( " 0x%04x : %02x "  "       \t MOV H, E\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x64: printf( " 0x%04x : %02x "  "       \t MOV H, H\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x65: printf( " 0x%04x : %02x "  "       \t MOV H, L\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x66: printf( " 0x%04x : %02x "  "       \t MOV H, M\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x67: printf( " 0x%04x : %02x "  "       \t MOV H, A\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x68: printf( " 0x%04x : %02x "  "       \t MOV L, B\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x69: printf( " 0x%04x : %02x "  "       \t MOV L, C\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x6a: printf( " 0x%04x : %02x "  "       \t MOV L, D\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x6b: printf( " 0x%04x : %02x "  "       \t MOV L, E\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x6c: printf( " 0x%04x : %02x "  "       \t MOV L, H\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x6d: printf( " 0x%04x : %02x "  "       \t MOV L, L\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x6e: printf( " 0x%04x : %02x "  "       \t MOV L, M\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x6f: printf( " 0x%04x : %02x "  "       \t MOV L, A\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;

        case 0x70: printf( " 0x%04x : %02x "  "       \t MOV M, B\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x71: printf( " 0x%04x : %02x "  "       \t MOV M, C\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x72: printf( " 0x%04x : %02x "  "       \t MOV M, D\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x73: printf( " 0x%04x : %02x "  "       \t MOV M, E\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x74: printf( " 0x%04x : %02x "  "       \t MOV M, H\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x75: printf( " 0x%04x : %02x "  "       \t MOV M, L\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x76: printf( " 0x%04x : %02x "  "       \t HTL\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x77: printf( " 0x%04x : %02x "  "       \t MOV M, A\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x78: printf( " 0x%04x : %02x "  "       \t MOV A, B\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x79: printf( " 0x%04x : %02x "  "       \t MOV A, C\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x7a: printf( " 0x%04x : %02x "  "       \t MOV A, D\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x7b: printf( " 0x%04x : %02x "  "       \t MOV A, E\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x7c: printf( " 0x%04x : %02x "  "       \t MOV A, H\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x7d: printf( " 0x%04x : %02x "  "       \t MOV A, L\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x7e: printf( " 0x%04x : %02x "  "       \t MOV A, M\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x7f: printf( " 0x%04x : %02x "  "       \t MOV A, A\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break ;

        case 0x80: printf( " 0x%04x : %02x "  "       \t ADD B\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x81: printf( " 0x%04x : %02x "  "       \t ADD C\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x82: printf( " 0x%04x : %02x "  "       \t ADD D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x83: printf( " 0x%04x : %02x "  "       \t ADD E\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x84: printf( " 0x%04x : %02x "  "       \t ADD H\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x85: printf( " 0x%04x : %02x "  "       \t ADD L\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x86: printf( " 0x%04x : %02x "  "       \t ADD M\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x87: printf( " 0x%04x : %02x "  "       \t ADD A\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x88: printf( " 0x%04x : %02x "  "       \t ADC B\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x89: printf( " 0x%04x : %02x "  "       \t ADC C\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x8a: printf( " 0x%04x : %02x "  "       \t ADC D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x8b: printf( " 0x%04x : %02x "  "       \t ADC E\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x8c: printf( " 0x%04x : %02x "  "       \t ADC H\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x8d: printf( " 0x%04x : %02x "  "       \t ADC L\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x8e: printf( " 0x%04x : %02x "  "       \t ADC M\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x8f: printf( " 0x%04x : %02x "  "       \t ADC A\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;

        case 0x90: printf( " 0x%04x : %02x "  "       \t SUB B\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x91: printf( " 0x%04x : %02x "  "       \t SUB C\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x92: printf( " 0x%04x : %02x "  "       \t SUB D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x93: printf( " 0x%04x : %02x "  "       \t SUB E\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x94: printf( " 0x%04x : %02x "  "       \t SUB H\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x95: printf( " 0x%04x : %02x "  "       \t SUB L\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x96: printf( " 0x%04x : %02x "  "       \t SUB M\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x97: printf( " 0x%04x : %02x "  "       \t SUB A\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x98: printf( " 0x%04x : %02x "  "       \t SBB B\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x99: printf( " 0x%04x : %02x "  "       \t SBB C\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x9a: printf( " 0x%04x : %02x "  "       \t SBB D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x9b: printf( " 0x%04x : %02x "  "       \t SBB E\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x9c: printf( " 0x%04x : %02x "  "       \t SBB H\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x9d: printf( " 0x%04x : %02x "  "       \t SBB L\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x9e: printf( " 0x%04x : %02x "  "       \t SBB M\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0x9f: printf( " 0x%04x : %02x "  "       \t SBB A\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;

        case 0xa0: printf( " 0x%04x : %02x "  "       \t ANA B\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xa1: printf( " 0x%04x : %02x "  "       \t ANA C\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xa2: printf( " 0x%04x : %02x "  "       \t ANA D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xa3: printf( " 0x%04x : %02x "  "       \t ANA E\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xa4: printf( " 0x%04x : %02x "  "       \t ANA H\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xa5: printf( " 0x%04x : %02x "  "       \t ANA L\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xa6: printf( " 0x%04x : %02x "  "       \t ANA M\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xa7: printf( " 0x%04x : %02x "  "       \t ANA A\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xa8: printf( " 0x%04x : %02x "  "       \t XRA B\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xa9: printf( " 0x%04x : %02x "  "       \t XRA C\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xaa: printf( " 0x%04x : %02x "  "       \t XRA D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xab: printf( " 0x%04x : %02x "  "       \t XRA E\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xac: printf( " 0x%04x : %02x "  "       \t XRA H\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xad: printf( " 0x%04x : %02x "  "       \t XRA L\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xae: printf( " 0x%04x : %02x "  "       \t XRA M\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xaf: printf( " 0x%04x : %02x "  "       \t XRA A\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;

        case 0xb0: printf( " 0x%04x : %02x "  "       \t ORA B\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xb1: printf( " 0x%04x : %02x "  "       \t ORA C\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xb2: printf( " 0x%04x : %02x "  "       \t ORA D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xb3: printf( " 0x%04x : %02x "  "       \t ORA E\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xb4: printf( " 0x%04x : %02x "  "       \t ORA H\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xb5: printf( " 0x%04x : %02x "  "       \t ORA L\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xb6: printf( " 0x%04x : %02x "  "       \t ORA M\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xb7: printf( " 0x%04x : %02x "  "       \t ORA A\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xb8: printf( " 0x%04x : %02x "  "       \t CMP B\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xb9: printf( " 0x%04x : %02x "  "       \t CMP C\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xba: printf( " 0x%04x : %02x "  "       \t CMP D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xbb: printf( " 0x%04x : %02x "  "       \t CMP E\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xbc: printf( " 0x%04x : %02x "  "       \t CMP H\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xbd: printf( " 0x%04x : %02x "  "       \t CMP L\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xbe: printf( " 0x%04x : %02x "  "       \t CMP M\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;
        case 0xbf: printf( " 0x%04x : %02x "  "       \t CMP A\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break ;

        case 0xc0: printf( " 0x%04x : %02x "  "       \t RNZ\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xc1: printf( " 0x%04x : %02x "  "       \t POP B\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xc2: printf( " 0x%04x : %02x %02x %02x  \t JNZ %02x %02x\n"      , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xc3: printf( " 0x%04x : %02x %02x %02x  \t JMP %02x %02x\n"      , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xc4: printf( " 0x%04x : %02x %02x %02x  \t CNZ %02x %02x\n"      , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xc5: printf( " 0x%04x : %02x "  "       \t PUSH B\n"             , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xc6: printf( " 0x%04x : %02x %02x       \t ADI %02x\n"           , pc , buffer[pc], buffer[pc+1], buffer[pc+1]                              ) ; op_size = 2; break;
        case 0xc7: printf( " 0x%04x : %02x "  "       \t RST 0\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xc8: printf( " 0x%04x : %02x "  "       \t RZ\n"                 , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xc9: printf( " 0x%04x : %02x "  "       \t RET\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        case 0xca: printf( " 0x%04x : %02x %02x %02x  \t JZ %02x %02x\n"       , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xcb: printf( " 0x%04x : %02x "  "       \t -\n"                  , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xcc: printf( " 0x%04x : %02x %02x %02x  \t CZ %02x %02x\n"       , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xcd: printf( " 0x%04x : %02x %02x %02x  \t CALL %02x %02x\n"     , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xce: printf( " 0x%04x : %02x %02x       \t ACI %02x\n"           , pc , buffer[pc], buffer[pc+1], buffer[pc+1]                              ) ; op_size = 2; break;
        case 0xcf: printf( " 0x%04x : %02x "  "       \t RST 1\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        case 0xd0: printf( " 0x%04x : %02x "  "       \t RNC\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xd1: printf( " 0x%04x : %02x "  "       \t POP D\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xd2: printf( " 0x%04x : %02x %02x %02x  \t JNC %02x %02x\n"      , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xd3: printf( " 0x%04x : %02x %02x       \t OUT %2X\n"            , pc , buffer[pc], buffer[pc+1] , buffer[pc+1]                             ) ; op_size = 2; break;
        case 0xd4: printf( " 0x%04x : %02x %02x %02x  \t CNC %02x %02x\n"      , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xd5: printf( " 0x%04x : %02x "  "       \t PUSH D\n"             , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xd6: printf( " 0x%04x : %02x %02x       \t SUI %02x\n"           , pc , buffer[pc], buffer[pc+1], buffer[pc+1]                              ) ; op_size = 2; break;
        case 0xd7: printf( " 0x%04x : %02x "  "       \t RST 2\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xd8: printf( " 0x%04x : %02x "  "       \t RC\n"                 , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xd9: printf( " 0x%04x : %02x "  "       \t -\n"                  , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        case 0xda: printf( " 0x%04x : %02x %02x %02x  \t JC %02x %02x\n"       , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xdb: printf( " 0x%04x : %02x %02x       \t IN %2X\n"             , pc , buffer[pc], buffer[pc+1], buffer[pc+1]                              ) ; op_size = 2; break;
        case 0xdc: printf( " 0x%04x : %02x %02x %02x  \t CC %02x %02x\n"       , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xdd: printf( " 0x%04x : %02x "  "       \t -\n"                  , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xde: printf( " 0x%04x : %02x %02x       \t SBI %02x\n"           , pc , buffer[pc], buffer[pc+1], buffer[pc+1]                              ) ; op_size = 2; break;
        case 0xdf: printf( " 0x%04x : %02x "  "       \t RST 3\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        case 0xe0: printf( " 0x%04x : %02x "  "       \t RPO\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xe1: printf( " 0x%04x : %02x "  "       \t POP H\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xe2: printf( " 0x%04x : %02x %02x %02x  \t JPO %02x %02x\n"      , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xe3: printf( " 0x%04x : %02x "  "       \t XTHL\n"               , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xe4: printf( " 0x%04x : %02x %02x %02x  \t CPO %02x %02x\n"      , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xe5: printf( " 0x%04x : %02x "  "       \t PUSH H\n"             , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xe6: printf( " 0x%04x : %02x %02x       \t ANI %02x\n"           , pc , buffer[pc], buffer[pc+1], buffer[pc+1]                              ) ; op_size = 2; break;
        case 0xe7: printf( " 0x%04x : %02x "  "       \t RST 4\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xe8: printf( " 0x%04x : %02x "  "       \t RPE\n"                , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xe9: printf( " 0x%04x : %02x "  "       \t PCHL\n"               , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        case 0xea: printf( " 0x%04x : %02x %02x %02x  \t JPE %02x %02x\n"      , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xeb: printf( " 0x%04x : %02x "  "       \t XCHG\n"               , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xec: printf( " 0x%04x : %02x %02x %02x  \t CPE %02x %02x\n"      , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xed: printf( " 0x%04x : %02x "  "       \t -\n"                  , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xee: printf( " 0x%04x : %02x %02x       \t XRI %02x\n"           , pc , buffer[pc], buffer[pc+1], buffer[pc+1]                              ) ; op_size = 2; break;
        case 0xef: printf( " 0x%04x : %02x "  "       \t RST 5\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        case 0xf0: printf( " 0x%04x : %02x "  "       \t RP\n"                 , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xf1: printf( " 0x%04x : %02x "  "       \t POP PSW\n"            , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xf2: printf( " 0x%04x : %02x %02x %02x  \t JP %02x %02x\n"       , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xf3: printf( " 0x%04x : %02x "  "       \t DI\n"                 , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xf4: printf( " 0x%04x : %02x %02x %02x  \t CP %02x %02x\n"       , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xf5: printf( " 0x%04x : %02x "  "       \t PUSH PSW\n"           , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xf6: printf( " 0x%04x : %02x %02x       \t ORI %02x\n"           , pc , buffer[pc], buffer[pc+1], buffer[pc+1]                              ) ; op_size = 2; break;
        case 0xf7: printf( " 0x%04x : %02x "  "       \t RST 6\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xf8: printf( " 0x%04x : %02x "  "       \t RM\n"                 , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xf9: printf( " 0x%04x : %02x "  "       \t SPHL\n"               , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        case 0xfa: printf( " 0x%04x : %02x %02x %02x  \t JM %02x %02x\n"       , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xfb: printf( " 0x%04x : %02x "  "       \t EI\n"                 , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xfc: printf( " 0x%04x : %02x %02x %02x  \t CM %02x %02x\n"       , pc , buffer[pc], buffer[pc+1], buffer[pc+2] , buffer[pc+1], buffer[pc+2] ) ; op_size = 3; break;
        case 0xfd: printf( " 0x%04x : %02x "  "       \t -\n"                  , pc , buffer[pc]                                                          ) ; op_size = 1; break;
        case 0xfe: printf( " 0x%04x : %02x %02x       \t CPI %02x\n"           , pc , buffer[pc], buffer[pc+1], buffer[pc+1]                              ) ; op_size = 2; break;
        case 0xff: printf( " 0x%04x : %02x "  "       \t RST 7\n"              , pc , buffer[pc]                                                          ) ; op_size = 1; break;

        default:
            printf(" %2X is not implemented\n", buffer[pc]);
            break;
    }

    return op_size;
}

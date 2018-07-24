/******************************************************************************
 * Copyright (C) 2016-2018  Renan S. Silva                                    *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgment in the product documentation would be   *
 *    appreciated but is not required.                                        *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "disassembler.h"
#include "types.h"
#include "memory.h"

unsigned short int disassembler( _cpu_info *cpu ) {
    unsigned short int op_size = 1;
    uint16_t pc = cpu->pc;

    switch ( read_byte(cpu, pc) ) {
        case 0x00:   printf ( " %04x : %02x      """"    NOP                     "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x01:   printf ( " %04x : %02x %02x %02x    LD BC, 0x%02x%02x           ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2) , read_byte(cpu, pc+1), read_byte(cpu, pc+2) ); op_size = 3; break;
        case 0x02:   printf ( " %04x : %02x      """"    LD (BC), A              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x03:   printf ( " %04x : %02x      """"    INC BC                  "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x04:   printf ( " %04x : %02x      """"    INC B                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x05:   printf ( " %04x : %02x      """"    DEC B                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x06:   printf ( " %04x : %02x %02x   ""    LD B, 0x%02x              ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0x07:   printf ( " %04x : %02x      """"    RLCA                    "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x08:   printf ( " %04x : %02x %02x %02x    LD (0x%02x%02x), SP         ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0x09:   printf ( " %04x : %02x      """"    ADD HL, BC              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x0A:   printf ( " %04x : %02x      """"    LD A, (BC)              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x0B:   printf ( " %04x : %02x      """"    DEC BC                  "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x0C:   printf ( " %04x : %02x      """"    INC C                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x0D:   printf ( " %04x : %02x      """"    DEC C                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x0E:   printf ( " %04x : %02x %02x   ""    LD C, 0x%02x              ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0x0F:   printf ( " %04x : %02x      """"    RRCA                    "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0x10:   printf ( " %04x : %02x      """"    STOP                    "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x11:   printf ( " %04x : %02x %02x %02x    LD DE, 0x%02x%02x           ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0x12:   printf ( " %04x : %02x      """"    LD (DE), A              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x13:   printf ( " %04x : %02x      """"    INC DE                  "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x14:   printf ( " %04x : %02x      """"    INC D                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x15:   printf ( " %04x : %02x      """"    DEC D                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x16:   printf ( " %04x : %02x %02x   ""    LD D, 0x%02x              ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0x17:   printf ( " %04x : %02x      """"    RLA                     "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x18:   printf ( " %04x : %02x %02x   ""    JR 0x%02x                 ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0x19:   printf ( " %04x : %02x      """"    ADD HL, DE              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x1A:   printf ( " %04x : %02x      """"    LD A, (DE)              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x1B:   printf ( " %04x : %02x      """"    DEC DE                  "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x1C:   printf ( " %04x : %02x      """"    INC E                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x1D:   printf ( " %04x : %02x      """"    DEC E                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x1E:   printf ( " %04x : %02x %02x   ""    LD E, 0x%02x              ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0x1F:   printf ( " %04x : %02x      """"    RRA                     "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0x20:   printf ( " %04x : %02x %02x   ""    JR NZ, 0x%02x             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0x21:   printf ( " %04x : %02x %02x %02x    LD HL, 0x%02x%02x           ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0x22:   printf ( " %04x : %02x      """"    LDI (HL), A             "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x23:   printf ( " %04x : %02x      """"    INC HL                  "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x24:   printf ( " %04x : %02x      """"    INC H                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x25:   printf ( " %04x : %02x      """"    DEC H                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x26:   printf ( " %04x : %02x %02x   ""    LD H, 0x%02x              ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0x27:   printf ( " %04x : %02x      """"    DAA                     "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x28:   printf ( " %04x : %02x %02x   ""    JR Z, 0x%02x              ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0x29:   printf ( " %04x : %02x      """"    ADD HL, HL              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x2A:   printf ( " %04x : %02x      """"    LDI A, (HL)             "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x2B:   printf ( " %04x : %02x      """"    DEC HL                  "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x2C:   printf ( " %04x : %02x      """"    INC L                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x2D:   printf ( " %04x : %02x      """"    DEC L                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x2E:   printf ( " %04x : %02x %02x   ""    LD L, 0x%02x              ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0x2F:   printf ( " %04x : %02x      """"    CPL                     "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0x30:   printf ( " %04x : %02x %02x   ""    JR NC, 0x%02x             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0x31:   printf ( " %04x : %02x %02x %02x    LD SP, (0x%02x%02x)         ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0x32:   printf ( " %04x : %02x      """"    LDD (HL), A             "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x33:   printf ( " %04x : %02x      """"    INC SP                  "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x34:   printf ( " %04x : %02x      """"    INC (HL)                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x35:   printf ( " %04x : %02x      """"    DEC (HL)                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x36:   printf ( " %04x : %02x %02x   ""    LD (HL), 0x%02x           ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0x37:   printf ( " %04x : %02x      """"    SCF                     "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x38:   printf ( " %04x : %02x %02x   ""    JR C, 0x%02x              ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0x39:   printf ( " %04x : %02x      """"    ADD HL, SP              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x3A:   printf ( " %04x : %02x      """"    LDD A, (HL)             "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x3B:   printf ( " %04x : %02x      """"    DEC SP                  "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x3C:   printf ( " %04x : %02x      """"    INC A                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x3D:   printf ( " %04x : %02x      """"    DEC A                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x3E:   printf ( " %04x : %02x %02x   ""    LD A, 0x%02x              ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0x3F:   printf ( " %04x : %02x      """"    CCF                     "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0x40:   printf ( " %04x : %02x      """"    LD B, B                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x41:   printf ( " %04x : %02x      """"    LD B, C                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x42:   printf ( " %04x : %02x      """"    LD B, D                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x43:   printf ( " %04x : %02x      """"    LD B, E                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x44:   printf ( " %04x : %02x      """"    LD B, H                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x45:   printf ( " %04x : %02x      """"    LD B, L                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x46:   printf ( " %04x : %02x      """"    LD B, (HL)              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x47:   printf ( " %04x : %02x      """"    LD B, A                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x48:   printf ( " %04x : %02x      """"    LD C, B                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x49:   printf ( " %04x : %02x      """"    LD C, C                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x4A:   printf ( " %04x : %02x      """"    LD C, D                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x4B:   printf ( " %04x : %02x      """"    LD C, E                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x4C:   printf ( " %04x : %02x      """"    LD C, H                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x4D:   printf ( " %04x : %02x      """"    LD C, L                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x4E:   printf ( " %04x : %02x      """"    LD C, (HL)              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x4F:   printf ( " %04x : %02x      """"    LD C, A                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0x50:   printf ( " %04x : %02x      """"    LD D, B                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x51:   printf ( " %04x : %02x      """"    LD D, C                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x52:   printf ( " %04x : %02x      """"    LD D, D                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x53:   printf ( " %04x : %02x      """"    LD D, E                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x54:   printf ( " %04x : %02x      """"    LD D, H                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x55:   printf ( " %04x : %02x      """"    LD D, L                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x56:   printf ( " %04x : %02x      """"    LD D, (HL)              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x57:   printf ( " %04x : %02x      """"    LD D, A                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x58:   printf ( " %04x : %02x      """"    LD E, B                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x59:   printf ( " %04x : %02x      """"    LD E, C                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x5A:   printf ( " %04x : %02x      """"    LD E, D                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x5B:   printf ( " %04x : %02x      """"    LD E, E                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x5C:   printf ( " %04x : %02x      """"    LD E, H                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x5D:   printf ( " %04x : %02x      """"    LD E, L                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x5E:   printf ( " %04x : %02x      """"    LD E, (HL)              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x5F:   printf ( " %04x : %02x      """"    LD E, A                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0x60:   printf ( " %04x : %02x      """"    LD H, B                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x61:   printf ( " %04x : %02x      """"    LD H, C                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x62:   printf ( " %04x : %02x      """"    LD H, D                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x63:   printf ( " %04x : %02x      """"    LD H, E                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x64:   printf ( " %04x : %02x      """"    LD H, H                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x65:   printf ( " %04x : %02x      """"    LD H, L                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x66:   printf ( " %04x : %02x      """"    LD H, (HL)              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x67:   printf ( " %04x : %02x      """"    LD H, A                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x68:   printf ( " %04x : %02x      """"    LD L, B                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x69:   printf ( " %04x : %02x      """"    LD L, C                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x6A:   printf ( " %04x : %02x      """"    LD L, D                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x6B:   printf ( " %04x : %02x      """"    LD L, E                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x6C:   printf ( " %04x : %02x      """"    LD L, H                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x6D:   printf ( " %04x : %02x      """"    LD L, L                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x6E:   printf ( " %04x : %02x      """"    LD L, (HL)              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x6F:   printf ( " %04x : %02x      """"    LD L, A                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0x70:   printf ( " %04x : %02x      """"    LD (HL), B              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x71:   printf ( " %04x : %02x      """"    LD (HL), C              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x72:   printf ( " %04x : %02x      """"    LD (HL), D              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x73:   printf ( " %04x : %02x      """"    LD (HL), E              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x74:   printf ( " %04x : %02x      """"    LD (HL), H              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x75:   printf ( " %04x : %02x      """"    LD (HL), L              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x76:   printf ( " %04x : %02x      """"    HALT                    "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x77:   printf ( " %04x : %02x      """"    LD (HL), A              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x78:   printf ( " %04x : %02x      """"    LD A, B                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x79:   printf ( " %04x : %02x      """"    LD A, C                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x7A:   printf ( " %04x : %02x      """"    LD A, D                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x7B:   printf ( " %04x : %02x      """"    LD A, E                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x7C:   printf ( " %04x : %02x      """"    LD A, H                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x7D:   printf ( " %04x : %02x      """"    LD A, L                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x7E:   printf ( " %04x : %02x      """"    LD A, (HL)              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x7F:   printf ( " %04x : %02x      """"    LD A, A                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0x80:   printf ( " %04x : %02x      """"    ADD A, B                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x81:   printf ( " %04x : %02x      """"    ADD A, C                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x82:   printf ( " %04x : %02x      """"    ADD A, D                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x83:   printf ( " %04x : %02x      """"    ADD A, E                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x84:   printf ( " %04x : %02x      """"    ADD A, H                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x85:   printf ( " %04x : %02x      """"    ADD A, L                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x86:   printf ( " %04x : %02x      """"    ADD A, (HL)             "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x87:   printf ( " %04x : %02x      """"    ADD A, A                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x88:   printf ( " %04x : %02x      """"    ADC A, B                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x89:   printf ( " %04x : %02x      """"    ADC A, C                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x8A:   printf ( " %04x : %02x      """"    ADC A, D                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x8B:   printf ( " %04x : %02x      """"    ADC A, E                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x8C:   printf ( " %04x : %02x      """"    ADC A, H                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x8D:   printf ( " %04x : %02x      """"    ADC A, L                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x8E:   printf ( " %04x : %02x      """"    ADC A, (HL)             "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x8F:   printf ( " %04x : %02x      """"    ADC A, A                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0x90:   printf ( " %04x : %02x      """"    SUB A, B                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x91:   printf ( " %04x : %02x      """"    SUB A, C                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x92:   printf ( " %04x : %02x      """"    SUB A, D                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x93:   printf ( " %04x : %02x      """"    SUB A, E                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x94:   printf ( " %04x : %02x      """"    SUB A, H                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x95:   printf ( " %04x : %02x      """"    SUB A, L                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x96:   printf ( " %04x : %02x      """"    SUB A, (HL)             "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x97:   printf ( " %04x : %02x      """"    SUB A, A                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x98:   printf ( " %04x : %02x      """"    SBC A, B                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x99:   printf ( " %04x : %02x      """"    SBC A, C                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x9A:   printf ( " %04x : %02x      """"    SBC A, D                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x9B:   printf ( " %04x : %02x      """"    SBC A, E                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x9C:   printf ( " %04x : %02x      """"    SBC A, H                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x9D:   printf ( " %04x : %02x      """"    SBC A, L                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x9E:   printf ( " %04x : %02x      """"    SBC A, (HL)             "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0x9F:   printf ( " %04x : %02x      """"    SBC A, A                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0xA0:   printf ( " %04x : %02x      """"    AND A, B                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xA1:   printf ( " %04x : %02x      """"    AND A, C                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xA2:   printf ( " %04x : %02x      """"    AND A, D                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xA3:   printf ( " %04x : %02x      """"    AND A, E                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xA4:   printf ( " %04x : %02x      """"    AND A, H                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xA5:   printf ( " %04x : %02x      """"    AND A, L                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xA6:   printf ( " %04x : %02x      """"    AND A, (HL)             "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xA7:   printf ( " %04x : %02x      """"    AND A, A                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xA8:   printf ( " %04x : %02x      """"    XOR A, B                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xA9:   printf ( " %04x : %02x      """"    XOR A, C                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xAA:   printf ( " %04x : %02x      """"    XOR A, D                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xAB:   printf ( " %04x : %02x      """"    XOR A, E                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xAC:   printf ( " %04x : %02x      """"    XOR A, H                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xAD:   printf ( " %04x : %02x      """"    XOR A, L                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xAE:   printf ( " %04x : %02x      """"    XOR A, (HL)             "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xAF:   printf ( " %04x : %02x      """"    XOR A, A                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0xB0:   printf ( " %04x : %02x      """"    OR A, B                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xB1:   printf ( " %04x : %02x      """"    OR A, C                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xB2:   printf ( " %04x : %02x      """"    OR A, D                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xB3:   printf ( " %04x : %02x      """"    OR A, E                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xB4:   printf ( " %04x : %02x      """"    OR A, H                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xB5:   printf ( " %04x : %02x      """"    OR A, L                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xB6:   printf ( " %04x : %02x      """"    OR A, (HL)              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xB7:   printf ( " %04x : %02x      """"    OR A, A                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xB8:   printf ( " %04x : %02x      """"    CP A, B                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xB9:   printf ( " %04x : %02x      """"    CP A, C                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xBA:   printf ( " %04x : %02x      """"    CP A, D                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xBB:   printf ( " %04x : %02x      """"    CP A, E                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xBC:   printf ( " %04x : %02x      """"    CP A, H                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xBD:   printf ( " %04x : %02x      """"    CP A, L                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xBE:   printf ( " %04x : %02x      """"    CP A, (HL)              "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xBF:   printf ( " %04x : %02x      """"    CP A, A                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0xC0:   printf ( " %04x : %02x      """"    RET NZ                  "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xC1:   printf ( " %04x : %02x      """"    POP BC                  "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xC2:   printf ( " %04x : %02x %02x %02x    JP NZ, 0x%02x%02x           ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0xC3:   printf ( " %04x : %02x %02x %02x    JP 0x%02x%02x               ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0xC4:   printf ( " %04x : %02x %02x %02x    CALL NZ, 0x%02x%02x         ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0xC5:   printf ( " %04x : %02x      """"    PUSH BC                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xC6:   printf ( " %04x : %02x %02x   ""    ADD A, 0x%02x             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0xC7:   printf ( " %04x : %02x      """"    RST 0x00                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xC8:   printf ( " %04x : %02x      """"    RET Z                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xC9:   printf ( " %04x : %02x      """"    RET                     "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xCA:   printf ( " %04x : %02x %02x %02x    JP Z, 0x%02x%02x            ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0xCC:   printf ( " %04x : %02x %02x %02x    CALL Z, 0x%02x%02x          ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0xCD:   printf ( " %04x : %02x %02x %02x    CALL 0x%02x%02x             ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0xCE:   printf ( " %04x : %02x %02x   ""    ADC A, 0x%02x             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0xCF:   printf ( " %04x : %02x      """"    RST 0x08                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0xD0:   printf ( " %04x : %02x      """"    RET NC                  "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xD1:   printf ( " %04x : %02x      """"    POP DE                  "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xD2:   printf ( " %04x : %02x %02x %02x    JP NC, 0x%02x%02x           ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0xD3:   printf ( " %04x : %02x      """"    -                       "  , pc, read_byte(cpu, pc)                                                          ); op_size = 0; break;
        case 0xD4:   printf ( " %04x : %02x %02x %02x    CALL NC, 0x%02x%02x         ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0xD5:   printf ( " %04x : %02x      """"    PUSH DE                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xD6:   printf ( " %04x : %02x %02x   ""    SUB A, 0x%02x             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0xD7:   printf ( " %04x : %02x      """"    RST 0x10                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xD8:   printf ( " %04x : %02x      """"    RET C                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xD9:   printf ( " %04x : %02x      """"    RETI                    "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xDA:   printf ( " %04x : %02x %02x %02x    JP C, 0x%02x%02x            ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0xDB:   printf ( " %04x : %02x      """"    -                       "  , pc, read_byte(cpu, pc)                                                          ); op_size = 0; break;
        case 0xDC:   printf ( " %04x : %02x %02x %02x    CALL C, 0x%02x%02x          ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0xDD:   printf ( " %04x : %02x      """"    -                       "  , pc, read_byte(cpu, pc)                                                          ); op_size = 0; break;
        case 0xDE:   printf ( " %04x : %02x %02x   ""    SBC A, 0x%02x             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0xDF:   printf ( " %04x : %02x      """"    RST 0x18                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0xE0:   printf ( " %04x : %02x %02x   ""    LD (0xFF00 + 0x%02x), A   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0xE1:   printf ( " %04x : %02x      """"    POP HL                  "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xE2:   printf ( " %04x : %02x      """"    LD (C), A               "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xE3:   printf ( " %04x : %02x      """"    -                       "  , pc, read_byte(cpu, pc)                                                          ); op_size = 0; break;
        case 0xE4:   printf ( " %04x : %02x      """"    -                       "  , pc, read_byte(cpu, pc)                                                          ); op_size = 0; break;
        case 0xE5:   printf ( " %04x : %02x      """"    PUSH HL                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xE6:   printf ( " %04x : %02x %02x   ""    AND A, 0x%02x             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0xE7:   printf ( " %04x : %02x      """"    RST 0x20                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xE8:   printf ( " %04x : %02x %02x   ""    ADD SP, %02x              ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0xE9:   printf ( " %04x : %02x      """"    JP HL                   "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xEA:   printf ( " %04x : %02x %02x %02x    LD (0x%02x%02x), A          ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0xEB:   printf ( " %04x : %02x      """"    -                       "  , pc, read_byte(cpu, pc)                                                          ); op_size = 0; break;
        case 0xEC:   printf ( " %04x : %02x      """"    -                       "  , pc, read_byte(cpu, pc)                                                          ); op_size = 0; break;
        case 0xED:   printf ( " %04x : %02x      """"    -                       "  , pc, read_byte(cpu, pc)                                                          ); op_size = 0; break;
        case 0xEE:   printf ( " %04x : %02x %02x   ""    XOR A, 0x%02x             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0xEF:   printf ( " %04x : %02x      """"    RST 0x28                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0xF0:   printf ( " %04x : %02x %02x   ""    LD A, (0xFF00 + 0x%02x)   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0xF1:   printf ( " %04x : %02x      """"    POP AF                  "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xF2:   printf ( " %04x : %02x      """"    LD A, (C)               "  , pc, read_byte(cpu, pc)                                                          ); op_size = 3; break;
        case 0xF3:   printf ( " %04x : %02x      """"    DI                      "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xF4:   printf ( " %04x : %02x      """"    -                       "  , pc, read_byte(cpu, pc)                                                          ); op_size = 0; break;
        case 0xF5:   printf ( " %04x : %02x      """"    PUSH AF                 "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xF6:   printf ( " %04x : %02x %02x   ""    OR A, 0x%02x              ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0xF7:   printf ( " %04x : %02x      """"    RST 0x30                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xF8:   printf ( " %04x : %02x %02x   ""    LD HL, SP + 0x%02x        ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0xF9:   printf ( " %04x : %02x      """"    LD SP, HL               "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xFA:   printf ( " %04x : %02x %02x %02x    LD A, (0x%02x%02x)          ",pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+2), read_byte(cpu, pc+1), read_byte(cpu, pc+2)  ); op_size = 3; break;
        case 0xFB:   printf ( " %04x : %02x      """"    EI                      "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;
        case 0xFC:   printf ( " %04x : %02x      """"    -                       "  , pc, read_byte(cpu, pc)                                                          ); op_size = 0; break;
        case 0xFD:   printf ( " %04x : %02x      """"    -                       "  , pc, read_byte(cpu, pc)                                                          ); op_size = 0; break;
        case 0xFE:   printf ( " %04x : %02x %02x   ""    CP A, 0x%02x              ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1), read_byte(cpu, pc+1)                              ); op_size = 2; break;
        case 0xFF:   printf ( " %04x : %02x      """"    RST 0x38                "  , pc, read_byte(cpu, pc)                                                          ); op_size = 1; break;

        case 0xCB:
            switch ( read_byte(cpu, pc+1) ) {
                case 0x00: printf ( " %04x : %02x %02x   ""    RLC B                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x01: printf ( " %04x : %02x %02x   ""    RLC C                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x02: printf ( " %04x : %02x %02x   ""    RLC D                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x03: printf ( " %04x : %02x %02x   ""    RLC E                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x04: printf ( " %04x : %02x %02x   ""    RLC H                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x05: printf ( " %04x : %02x %02x   ""    RLC L                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x06: printf ( " %04x : %02x %02x   ""    RLC (HL)                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x07: printf ( " %04x : %02x %02x   ""    RLC A                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x08: printf ( " %04x : %02x %02x   ""    RRC B                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x09: printf ( " %04x : %02x %02x   ""    RRC C                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x0A: printf ( " %04x : %02x %02x   ""    RRC D                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x0B: printf ( " %04x : %02x %02x   ""    RRC E                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x0C: printf ( " %04x : %02x %02x   ""    RRC H                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x0D: printf ( " %04x : %02x %02x   ""    RRC L                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x0E: printf ( " %04x : %02x %02x   ""    RRC (HL)                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x0F: printf ( " %04x : %02x %02x   ""    RRC A                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;

                case 0x10: printf ( " %04x : %02x %02x   ""    RL B                    ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x11: printf ( " %04x : %02x %02x   ""    RL C                    ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x12: printf ( " %04x : %02x %02x   ""    RL D                    ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x13: printf ( " %04x : %02x %02x   ""    RL E                    ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x14: printf ( " %04x : %02x %02x   ""    RL H                    ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x15: printf ( " %04x : %02x %02x   ""    RL L                    ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x16: printf ( " %04x : %02x %02x   ""    RL (HL)                 ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x17: printf ( " %04x : %02x %02x   ""    RL A                    ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x18: printf ( " %04x : %02x %02x   ""    RR B                    ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x19: printf ( " %04x : %02x %02x   ""    RR C                    ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x1A: printf ( " %04x : %02x %02x   ""    RR D                    ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x1B: printf ( " %04x : %02x %02x   ""    RR E                    ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x1C: printf ( " %04x : %02x %02x   ""    RR H                    ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x1D: printf ( " %04x : %02x %02x   ""    RR L                    ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x1E: printf ( " %04x : %02x %02x   ""    RR (HL)                 ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x1F: printf ( " %04x : %02x %02x   ""    RR A                    ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;

                case 0x20: printf ( " %04x : %02x %02x   ""    SLA B                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x21: printf ( " %04x : %02x %02x   ""    SLA C                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x22: printf ( " %04x : %02x %02x   ""    SLA D                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x23: printf ( " %04x : %02x %02x   ""    SLA E                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x24: printf ( " %04x : %02x %02x   ""    SLA H                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x25: printf ( " %04x : %02x %02x   ""    SLA L                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x26: printf ( " %04x : %02x %02x   ""    SLA (HL)                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x27: printf ( " %04x : %02x %02x   ""    SLA A                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x28: printf ( " %04x : %02x %02x   ""    SRA B                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x29: printf ( " %04x : %02x %02x   ""    SRA C                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x2A: printf ( " %04x : %02x %02x   ""    SRA D                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x2B: printf ( " %04x : %02x %02x   ""    SRA E                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x2C: printf ( " %04x : %02x %02x   ""    SRA H                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x2D: printf ( " %04x : %02x %02x   ""    SRA L                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x2E: printf ( " %04x : %02x %02x   ""    SRA (HL)                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x2F: printf ( " %04x : %02x %02x   ""    SRA A                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;

                case 0x30: printf ( " %04x : %02x %02x   ""    SWAP B                  ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x31: printf ( " %04x : %02x %02x   ""    SWAP C                  ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x32: printf ( " %04x : %02x %02x   ""    SWAP D                  ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x33: printf ( " %04x : %02x %02x   ""    SWAP E                  ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x34: printf ( " %04x : %02x %02x   ""    SWAP H                  ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x35: printf ( " %04x : %02x %02x   ""    SWAP L                  ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x36: printf ( " %04x : %02x %02x   ""    SWAP (HL)               ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x37: printf ( " %04x : %02x %02x   ""    SWAP A                  ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x38: printf ( " %04x : %02x %02x   ""    SRL B                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x39: printf ( " %04x : %02x %02x   ""    SRL C                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x3A: printf ( " %04x : %02x %02x   ""    SRL D                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x3B: printf ( " %04x : %02x %02x   ""    SRL E                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x3C: printf ( " %04x : %02x %02x   ""    SRL H                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x3D: printf ( " %04x : %02x %02x   ""    SRL L                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x3E: printf ( " %04x : %02x %02x   ""    SRL (HL)                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x3F: printf ( " %04x : %02x %02x   ""    SRL A                   ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;

                case 0x40: printf ( " %04x : %02x %02x   ""    BIT 0, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x41: printf ( " %04x : %02x %02x   ""    BIT 0, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x42: printf ( " %04x : %02x %02x   ""    BIT 0, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x43: printf ( " %04x : %02x %02x   ""    BIT 0, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x44: printf ( " %04x : %02x %02x   ""    BIT 0, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x45: printf ( " %04x : %02x %02x   ""    BIT 0, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x46: printf ( " %04x : %02x %02x   ""    BIT 0, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x47: printf ( " %04x : %02x %02x   ""    BIT 0, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x48: printf ( " %04x : %02x %02x   ""    BIT 1, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x49: printf ( " %04x : %02x %02x   ""    BIT 1, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x4A: printf ( " %04x : %02x %02x   ""    BIT 1, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x4B: printf ( " %04x : %02x %02x   ""    BIT 1, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x4C: printf ( " %04x : %02x %02x   ""    BIT 1, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x4D: printf ( " %04x : %02x %02x   ""    BIT 1, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x4E: printf ( " %04x : %02x %02x   ""    BIT 1, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x4F: printf ( " %04x : %02x %02x   ""    BIT 1, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;

                case 0x50: printf ( " %04x : %02x %02x   ""    BIT 2, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x51: printf ( " %04x : %02x %02x   ""    BIT 2, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x52: printf ( " %04x : %02x %02x   ""    BIT 2, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x53: printf ( " %04x : %02x %02x   ""    BIT 2, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x54: printf ( " %04x : %02x %02x   ""    BIT 2, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x55: printf ( " %04x : %02x %02x   ""    BIT 2, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x56: printf ( " %04x : %02x %02x   ""    BIT 2, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x57: printf ( " %04x : %02x %02x   ""    BIT 2, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x58: printf ( " %04x : %02x %02x   ""    BIT 3, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x59: printf ( " %04x : %02x %02x   ""    BIT 3, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x5A: printf ( " %04x : %02x %02x   ""    BIT 3, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x5B: printf ( " %04x : %02x %02x   ""    BIT 3, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x5C: printf ( " %04x : %02x %02x   ""    BIT 3, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x5D: printf ( " %04x : %02x %02x   ""    BIT 3, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x5E: printf ( " %04x : %02x %02x   ""    BIT 3, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x5F: printf ( " %04x : %02x %02x   ""    BIT 3, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;

                case 0x60: printf ( " %04x : %02x %02x   ""    BIT 4, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x61: printf ( " %04x : %02x %02x   ""    BIT 4, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x62: printf ( " %04x : %02x %02x   ""    BIT 4, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x63: printf ( " %04x : %02x %02x   ""    BIT 4, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x64: printf ( " %04x : %02x %02x   ""    BIT 4, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x65: printf ( " %04x : %02x %02x   ""    BIT 4, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x66: printf ( " %04x : %02x %02x   ""    BIT 4, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x67: printf ( " %04x : %02x %02x   ""    BIT 4, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x68: printf ( " %04x : %02x %02x   ""    BIT 5, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x69: printf ( " %04x : %02x %02x   ""    BIT 5, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x6A: printf ( " %04x : %02x %02x   ""    BIT 5, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x6B: printf ( " %04x : %02x %02x   ""    BIT 5, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x6C: printf ( " %04x : %02x %02x   ""    BIT 5, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x6D: printf ( " %04x : %02x %02x   ""    BIT 5, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x6E: printf ( " %04x : %02x %02x   ""    BIT 5, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x6F: printf ( " %04x : %02x %02x   ""    BIT 5, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;

                case 0x70: printf ( " %04x : %02x %02x   ""    BIT 6, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x71: printf ( " %04x : %02x %02x   ""    BIT 6, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x72: printf ( " %04x : %02x %02x   ""    BIT 6, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x73: printf ( " %04x : %02x %02x   ""    BIT 6, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x74: printf ( " %04x : %02x %02x   ""    BIT 6, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x75: printf ( " %04x : %02x %02x   ""    BIT 6, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x76: printf ( " %04x : %02x %02x   ""    BIT 6, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x77: printf ( " %04x : %02x %02x   ""    BIT 6, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x78: printf ( " %04x : %02x %02x   ""    BIT 7, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x79: printf ( " %04x : %02x %02x   ""    BIT 7, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x7A: printf ( " %04x : %02x %02x   ""    BIT 7, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x7B: printf ( " %04x : %02x %02x   ""    BIT 7, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x7C: printf ( " %04x : %02x %02x   ""    BIT 7, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x7D: printf ( " %04x : %02x %02x   ""    BIT 7, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x7E: printf ( " %04x : %02x %02x   ""    BIT 7, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x7F: printf ( " %04x : %02x %02x   ""    BIT 7, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;

                case 0x80: printf ( " %04x : %02x %02x   ""    RES 0, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x81: printf ( " %04x : %02x %02x   ""    RES 0, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x82: printf ( " %04x : %02x %02x   ""    RES 0, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x83: printf ( " %04x : %02x %02x   ""    RES 0, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x84: printf ( " %04x : %02x %02x   ""    RES 0, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x85: printf ( " %04x : %02x %02x   ""    RES 0, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x86: printf ( " %04x : %02x %02x   ""    RES 0, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x87: printf ( " %04x : %02x %02x   ""    RES 0, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x88: printf ( " %04x : %02x %02x   ""    RES 1, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x89: printf ( " %04x : %02x %02x   ""    RES 1, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x8A: printf ( " %04x : %02x %02x   ""    RES 1, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x8B: printf ( " %04x : %02x %02x   ""    RES 1, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x8C: printf ( " %04x : %02x %02x   ""    RES 1, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x8D: printf ( " %04x : %02x %02x   ""    RES 1, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x8E: printf ( " %04x : %02x %02x   ""    RES 1, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x8F: printf ( " %04x : %02x %02x   ""    RES 1, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;

                case 0x90: printf ( " %04x : %02x %02x   ""    RES 2, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x91: printf ( " %04x : %02x %02x   ""    RES 2, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x92: printf ( " %04x : %02x %02x   ""    RES 2, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x93: printf ( " %04x : %02x %02x   ""    RES 2, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x94: printf ( " %04x : %02x %02x   ""    RES 2, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x95: printf ( " %04x : %02x %02x   ""    RES 2, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x96: printf ( " %04x : %02x %02x   ""    RES 2, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x97: printf ( " %04x : %02x %02x   ""    RES 2, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x98: printf ( " %04x : %02x %02x   ""    RES 3, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x99: printf ( " %04x : %02x %02x   ""    RES 3, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x9A: printf ( " %04x : %02x %02x   ""    RES 3, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x9B: printf ( " %04x : %02x %02x   ""    RES 3, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x9C: printf ( " %04x : %02x %02x   ""    RES 3, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x9D: printf ( " %04x : %02x %02x   ""    RES 3, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x9E: printf ( " %04x : %02x %02x   ""    RES 3, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0x9F: printf ( " %04x : %02x %02x   ""    RES 3, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;

                case 0xA0: printf ( " %04x : %02x %02x   ""    RES 4, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xA1: printf ( " %04x : %02x %02x   ""    RES 4, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xA2: printf ( " %04x : %02x %02x   ""    RES 4, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xA3: printf ( " %04x : %02x %02x   ""    RES 4, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xA4: printf ( " %04x : %02x %02x   ""    RES 4, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xA5: printf ( " %04x : %02x %02x   ""    RES 4, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xA6: printf ( " %04x : %02x %02x   ""    RES 4, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xA7: printf ( " %04x : %02x %02x   ""    RES 4, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xA8: printf ( " %04x : %02x %02x   ""    RES 5, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xA9: printf ( " %04x : %02x %02x   ""    RES 5, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xAA: printf ( " %04x : %02x %02x   ""    RES 5, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xAB: printf ( " %04x : %02x %02x   ""    RES 5, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xAC: printf ( " %04x : %02x %02x   ""    RES 5, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xAD: printf ( " %04x : %02x %02x   ""    RES 5, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xAE: printf ( " %04x : %02x %02x   ""    RES 5, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xAF: printf ( " %04x : %02x %02x   ""    RES 5, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;

                case 0xB0: printf ( " %04x : %02x %02x   ""    RES 6, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xB1: printf ( " %04x : %02x %02x   ""    RES 6, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xB2: printf ( " %04x : %02x %02x   ""    RES 6, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xB3: printf ( " %04x : %02x %02x   ""    RES 6, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xB4: printf ( " %04x : %02x %02x   ""    RES 6, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xB5: printf ( " %04x : %02x %02x   ""    RES 6, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xB6: printf ( " %04x : %02x %02x   ""    RES 6, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xB7: printf ( " %04x : %02x %02x   ""    RES 6, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xB8: printf ( " %04x : %02x %02x   ""    RES 7, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xB9: printf ( " %04x : %02x %02x   ""    RES 7, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xBA: printf ( " %04x : %02x %02x   ""    RES 7, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xBB: printf ( " %04x : %02x %02x   ""    RES 7, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xBC: printf ( " %04x : %02x %02x   ""    RES 7, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xBD: printf ( " %04x : %02x %02x   ""    RES 7, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xBE: printf ( " %04x : %02x %02x   ""    RES 7, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xBF: printf ( " %04x : %02x %02x   ""    RES 7, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;

                case 0xC0: printf ( " %04x : %02x %02x   ""    SET 0, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xC1: printf ( " %04x : %02x %02x   ""    SET 0, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xC2: printf ( " %04x : %02x %02x   ""    SET 0, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xC3: printf ( " %04x : %02x %02x   ""    SET 0, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xC4: printf ( " %04x : %02x %02x   ""    SET 0, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xC5: printf ( " %04x : %02x %02x   ""    SET 0, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xC6: printf ( " %04x : %02x %02x   ""    SET 0, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xC7: printf ( " %04x : %02x %02x   ""    SET 0, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xC8: printf ( " %04x : %02x %02x   ""    SET 1, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xC9: printf ( " %04x : %02x %02x   ""    SET 1, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xCA: printf ( " %04x : %02x %02x   ""    SET 1, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xCB: printf ( " %04x : %02x %02x   ""    SET 1, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xCC: printf ( " %04x : %02x %02x   ""    SET 1, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xCD: printf ( " %04x : %02x %02x   ""    SET 1, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xCE: printf ( " %04x : %02x %02x   ""    SET 1, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xCF: printf ( " %04x : %02x %02x   ""    SET 1, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;

                case 0xD0: printf ( " %04x : %02x %02x   ""    SET 2, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xD1: printf ( " %04x : %02x %02x   ""    SET 2, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xD2: printf ( " %04x : %02x %02x   ""    SET 2, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xD3: printf ( " %04x : %02x %02x   ""    SET 2, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xD4: printf ( " %04x : %02x %02x   ""    SET 2, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xD5: printf ( " %04x : %02x %02x   ""    SET 2, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xD6: printf ( " %04x : %02x %02x   ""    SET 2, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xD7: printf ( " %04x : %02x %02x   ""    SET 2, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xD8: printf ( " %04x : %02x %02x   ""    SET 3, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xD9: printf ( " %04x : %02x %02x   ""    SET 3, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xDA: printf ( " %04x : %02x %02x   ""    SET 3, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xDB: printf ( " %04x : %02x %02x   ""    SET 3, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xDC: printf ( " %04x : %02x %02x   ""    SET 3, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xDD: printf ( " %04x : %02x %02x   ""    SET 3, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xDE: printf ( " %04x : %02x %02x   ""    SET 3, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xDF: printf ( " %04x : %02x %02x   ""    SET 3, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;

                case 0xE0: printf ( " %04x : %02x %02x   ""    SET 4, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xE1: printf ( " %04x : %02x %02x   ""    SET 4, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xE2: printf ( " %04x : %02x %02x   ""    SET 4, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xE3: printf ( " %04x : %02x %02x   ""    SET 4, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xE4: printf ( " %04x : %02x %02x   ""    SET 4, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xE5: printf ( " %04x : %02x %02x   ""    SET 4, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xE6: printf ( " %04x : %02x %02x   ""    SET 4, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xE7: printf ( " %04x : %02x %02x   ""    SET 4, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xE8: printf ( " %04x : %02x %02x   ""    SET 5, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xE9: printf ( " %04x : %02x %02x   ""    SET 5, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xEA: printf ( " %04x : %02x %02x   ""    SET 5, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xEB: printf ( " %04x : %02x %02x   ""    SET 5, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xEC: printf ( " %04x : %02x %02x   ""    SET 5, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xED: printf ( " %04x : %02x %02x   ""    SET 5, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xEE: printf ( " %04x : %02x %02x   ""    SET 5, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xEF: printf ( " %04x : %02x %02x   ""    SET 5, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;

                case 0xF0: printf ( " %04x : %02x %02x   ""    SET 6, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xF1: printf ( " %04x : %02x %02x   ""    SET 6, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xF2: printf ( " %04x : %02x %02x   ""    SET 6, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xF3: printf ( " %04x : %02x %02x   ""    SET 6, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xF4: printf ( " %04x : %02x %02x   ""    SET 6, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xF5: printf ( " %04x : %02x %02x   ""    SET 6, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xF6: printf ( " %04x : %02x %02x   ""    SET 6, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xF7: printf ( " %04x : %02x %02x   ""    SET 6, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xF8: printf ( " %04x : %02x %02x   ""    SET 7, B                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xF9: printf ( " %04x : %02x %02x   ""    SET 7, C                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xFA: printf ( " %04x : %02x %02x   ""    SET 7, D                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xFB: printf ( " %04x : %02x %02x   ""    SET 7, E                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xFC: printf ( " %04x : %02x %02x   ""    SET 7, H                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xFD: printf ( " %04x : %02x %02x   ""    SET 7, L                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xFE: printf ( " %04x : %02x %02x   ""    SET 7, (HL)             ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
                case 0xFF: printf ( " %04x : %02x %02x   ""    SET 7, A                ", pc, read_byte(cpu, pc), read_byte(cpu, pc+1)                                                          ); op_size = 1; break;
            }
            break;

        default:
            printf ( " %04x is not implemented " , read_byte(cpu, pc) );
            break;
    }

    return op_size;
}

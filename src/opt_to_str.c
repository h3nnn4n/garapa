/******************************************************************************
 * Copyright (C) 2016  Renan S. Silva                                         *
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

void get_opt_str( _cpu_info *cpu, char *str ) {
    uint16_t pc = cpu->pc;

    switch ( _read_byte(cpu, pc) ) {
        case 0x00:   sprintf (str,  " %04x : %02x      """"    NOP                     "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x01:   sprintf (str,  " %04x : %02x %02x %02x    LD BC, 0x%02x%02x           ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2) , _read_byte(cpu, pc+1), _read_byte(cpu, pc+2) );  break;
        case 0x02:   sprintf (str,  " %04x : %02x      """"    LD (BC), A              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x03:   sprintf (str,  " %04x : %02x      """"    INC BC                  "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x04:   sprintf (str,  " %04x : %02x      """"    INC B                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x05:   sprintf (str,  " %04x : %02x      """"    DEC B                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x06:   sprintf (str,  " %04x : %02x %02x   ""    LD B, 0x%02x              ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0x07:   sprintf (str,  " %04x : %02x      """"    RLCA                    "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x08:   sprintf (str,  " %04x : %02x %02x %02x    LD (0x%02x%02x), SP         ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0x09:   sprintf (str,  " %04x : %02x      """"    ADD HL, BC              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x0A:   sprintf (str,  " %04x : %02x      """"    LD A, (BC)              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x0B:   sprintf (str,  " %04x : %02x      """"    DEC BC                  "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x0C:   sprintf (str,  " %04x : %02x      """"    INC C                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x0D:   sprintf (str,  " %04x : %02x      """"    DEC C                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x0E:   sprintf (str,  " %04x : %02x %02x   ""    LD C, 0x%02x              ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0x0F:   sprintf (str,  " %04x : %02x      """"    RRCA                    "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0x10:   sprintf (str,  " %04x : %02x      """"    STOP                    "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x11:   sprintf (str,  " %04x : %02x %02x %02x    LD DE, 0x%02x%02x           ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0x12:   sprintf (str,  " %04x : %02x      """"    LD (DE), A              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x13:   sprintf (str,  " %04x : %02x      """"    INC DE                  "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x14:   sprintf (str,  " %04x : %02x      """"    INC D                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x15:   sprintf (str,  " %04x : %02x      """"    DEC D                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x16:   sprintf (str,  " %04x : %02x %02x   ""    LD D, 0x%02x              ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0x17:   sprintf (str,  " %04x : %02x      """"    RLA                     "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x18:   sprintf (str,  " %04x : %02x %02x   ""    JR 0x%02x                 ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0x19:   sprintf (str,  " %04x : %02x      """"    ADD HL, DE              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x1A:   sprintf (str,  " %04x : %02x      """"    LD A, (DE)              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x1B:   sprintf (str,  " %04x : %02x      """"    DEC DE                  "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x1C:   sprintf (str,  " %04x : %02x      """"    INC E                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x1D:   sprintf (str,  " %04x : %02x      """"    DEC E                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x1E:   sprintf (str,  " %04x : %02x %02x   ""    LD E, 0x%02x              ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0x1F:   sprintf (str,  " %04x : %02x      """"    RRA                     "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0x20:   sprintf (str,  " %04x : %02x %02x   ""    JR NZ, 0x%02x             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0x21:   sprintf (str,  " %04x : %02x %02x %02x    LD HL, 0x%02x%02x           ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0x22:   sprintf (str,  " %04x : %02x      """"    LDI (HL), A             "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x23:   sprintf (str,  " %04x : %02x      """"    INC HL                  "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x24:   sprintf (str,  " %04x : %02x      """"    INC H                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x25:   sprintf (str,  " %04x : %02x      """"    DEC H                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x26:   sprintf (str,  " %04x : %02x %02x   ""    LD H, 0x%02x              ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0x27:   sprintf (str,  " %04x : %02x      """"    DAA                     "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x28:   sprintf (str,  " %04x : %02x %02x   ""    JR Z, 0x%02x              ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0x29:   sprintf (str,  " %04x : %02x      """"    ADD HL, HL              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x2A:   sprintf (str,  " %04x : %02x      """"    LDI A, (HL)             "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x2B:   sprintf (str,  " %04x : %02x      """"    DEC HL                  "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x2C:   sprintf (str,  " %04x : %02x      """"    INC L                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x2D:   sprintf (str,  " %04x : %02x      """"    DEC L                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x2E:   sprintf (str,  " %04x : %02x %02x   ""    LD L, 0x%02x              ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0x2F:   sprintf (str,  " %04x : %02x      """"    CPL                     "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0x30:   sprintf (str,  " %04x : %02x %02x   ""    JR NC, 0x%02x             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0x31:   sprintf (str,  " %04x : %02x %02x %02x    LD SP, (0x%02x%02x)         ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0x32:   sprintf (str,  " %04x : %02x      """"    LDD (HL), A             "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x33:   sprintf (str,  " %04x : %02x      """"    INC SP                  "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x34:   sprintf (str,  " %04x : %02x      """"    INC (HL)                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x35:   sprintf (str,  " %04x : %02x      """"    DEC (HL)                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x36:   sprintf (str,  " %04x : %02x %02x   ""    LD (HL), 0x%02x           ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0x37:   sprintf (str,  " %04x : %02x      """"    SCF                     "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x38:   sprintf (str,  " %04x : %02x %02x   ""    JR C, 0x%02x              ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0x39:   sprintf (str,  " %04x : %02x      """"    ADD HL, SP              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x3A:   sprintf (str,  " %04x : %02x      """"    LDD A, (HL)             "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x3B:   sprintf (str,  " %04x : %02x      """"    DEC SP                  "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x3C:   sprintf (str,  " %04x : %02x      """"    INC A                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x3D:   sprintf (str,  " %04x : %02x      """"    DEC A                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x3E:   sprintf (str,  " %04x : %02x %02x   ""    LD A, 0x%02x              ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0x3F:   sprintf (str,  " %04x : %02x      """"    CCF                     "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0x40:   sprintf (str,  " %04x : %02x      """"    LD B, B                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x41:   sprintf (str,  " %04x : %02x      """"    LD B, C                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x42:   sprintf (str,  " %04x : %02x      """"    LD B, D                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x43:   sprintf (str,  " %04x : %02x      """"    LD B, E                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x44:   sprintf (str,  " %04x : %02x      """"    LD B, H                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x45:   sprintf (str,  " %04x : %02x      """"    LD B, L                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x46:   sprintf (str,  " %04x : %02x      """"    LD B, (HL)              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x47:   sprintf (str,  " %04x : %02x      """"    LD B, A                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x48:   sprintf (str,  " %04x : %02x      """"    LD C, B                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x49:   sprintf (str,  " %04x : %02x      """"    LD C, C                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x4A:   sprintf (str,  " %04x : %02x      """"    LD C, D                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x4B:   sprintf (str,  " %04x : %02x      """"    LD C, E                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x4C:   sprintf (str,  " %04x : %02x      """"    LD C, H                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x4D:   sprintf (str,  " %04x : %02x      """"    LD C, L                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x4E:   sprintf (str,  " %04x : %02x      """"    LD C, (HL)              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x4F:   sprintf (str,  " %04x : %02x      """"    LD C, A                 "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0x50:   sprintf (str,  " %04x : %02x      """"    LD D, B                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x51:   sprintf (str,  " %04x : %02x      """"    LD D, C                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x52:   sprintf (str,  " %04x : %02x      """"    LD D, D                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x53:   sprintf (str,  " %04x : %02x      """"    LD D, E                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x54:   sprintf (str,  " %04x : %02x      """"    LD D, H                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x55:   sprintf (str,  " %04x : %02x      """"    LD D, L                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x56:   sprintf (str,  " %04x : %02x      """"    LD D, (HL)              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x57:   sprintf (str,  " %04x : %02x      """"    LD D, A                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x58:   sprintf (str,  " %04x : %02x      """"    LD E, B                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x59:   sprintf (str,  " %04x : %02x      """"    LD E, C                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x5A:   sprintf (str,  " %04x : %02x      """"    LD E, D                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x5B:   sprintf (str,  " %04x : %02x      """"    LD E, E                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x5C:   sprintf (str,  " %04x : %02x      """"    LD E, H                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x5D:   sprintf (str,  " %04x : %02x      """"    LD E, L                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x5E:   sprintf (str,  " %04x : %02x      """"    LD E, (HL)              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x5F:   sprintf (str,  " %04x : %02x      """"    LD E, A                 "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0x60:   sprintf (str,  " %04x : %02x      """"    LD H, B                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x61:   sprintf (str,  " %04x : %02x      """"    LD H, C                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x62:   sprintf (str,  " %04x : %02x      """"    LD H, D                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x63:   sprintf (str,  " %04x : %02x      """"    LD H, E                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x64:   sprintf (str,  " %04x : %02x      """"    LD H, H                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x65:   sprintf (str,  " %04x : %02x      """"    LD H, L                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x66:   sprintf (str,  " %04x : %02x      """"    LD H, (HL)              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x67:   sprintf (str,  " %04x : %02x      """"    LD H, A                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x68:   sprintf (str,  " %04x : %02x      """"    LD L, B                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x69:   sprintf (str,  " %04x : %02x      """"    LD L, C                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x6A:   sprintf (str,  " %04x : %02x      """"    LD L, D                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x6B:   sprintf (str,  " %04x : %02x      """"    LD L, E                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x6C:   sprintf (str,  " %04x : %02x      """"    LD L, H                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x6D:   sprintf (str,  " %04x : %02x      """"    LD L, L                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x6E:   sprintf (str,  " %04x : %02x      """"    LD L, (HL)              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x6F:   sprintf (str,  " %04x : %02x      """"    LD L, A                 "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0x70:   sprintf (str,  " %04x : %02x      """"    LD (HL), B              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x71:   sprintf (str,  " %04x : %02x      """"    LD (HL), C              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x72:   sprintf (str,  " %04x : %02x      """"    LD (HL), D              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x73:   sprintf (str,  " %04x : %02x      """"    LD (HL), E              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x74:   sprintf (str,  " %04x : %02x      """"    LD (HL), H              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x75:   sprintf (str,  " %04x : %02x      """"    LD (HL), L              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x76:   sprintf (str,  " %04x : %02x      """"    HALT                    "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x77:   sprintf (str,  " %04x : %02x      """"    LD (HL), A              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x78:   sprintf (str,  " %04x : %02x      """"    LD A, B                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x79:   sprintf (str,  " %04x : %02x      """"    LD A, C                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x7A:   sprintf (str,  " %04x : %02x      """"    LD A, D                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x7B:   sprintf (str,  " %04x : %02x      """"    LD A, E                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x7C:   sprintf (str,  " %04x : %02x      """"    LD A, H                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x7D:   sprintf (str,  " %04x : %02x      """"    LD A, L                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x7E:   sprintf (str,  " %04x : %02x      """"    LD A, (HL)              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x7F:   sprintf (str,  " %04x : %02x      """"    LD A, A                 "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0x80:   sprintf (str,  " %04x : %02x      """"    ADD A, B                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x81:   sprintf (str,  " %04x : %02x      """"    ADD A, C                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x82:   sprintf (str,  " %04x : %02x      """"    ADD A, D                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x83:   sprintf (str,  " %04x : %02x      """"    ADD A, E                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x84:   sprintf (str,  " %04x : %02x      """"    ADD A, H                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x85:   sprintf (str,  " %04x : %02x      """"    ADD A, L                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x86:   sprintf (str,  " %04x : %02x      """"    ADD A, (HL)             "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x87:   sprintf (str,  " %04x : %02x      """"    ADD A, A                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x88:   sprintf (str,  " %04x : %02x      """"    ADC A, B                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x89:   sprintf (str,  " %04x : %02x      """"    ADC A, C                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x8A:   sprintf (str,  " %04x : %02x      """"    ADC A, D                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x8B:   sprintf (str,  " %04x : %02x      """"    ADC A, E                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x8C:   sprintf (str,  " %04x : %02x      """"    ADC A, H                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x8D:   sprintf (str,  " %04x : %02x      """"    ADC A, L                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x8E:   sprintf (str,  " %04x : %02x      """"    ADC A, (HL)             "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x8F:   sprintf (str,  " %04x : %02x      """"    ADC A, A                "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0x90:   sprintf (str,  " %04x : %02x      """"    SUB A, B                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x91:   sprintf (str,  " %04x : %02x      """"    SUB A, C                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x92:   sprintf (str,  " %04x : %02x      """"    SUB A, D                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x93:   sprintf (str,  " %04x : %02x      """"    SUB A, E                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x94:   sprintf (str,  " %04x : %02x      """"    SUB A, H                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x95:   sprintf (str,  " %04x : %02x      """"    SUB A, L                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x96:   sprintf (str,  " %04x : %02x      """"    SUB A, (HL)             "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x97:   sprintf (str,  " %04x : %02x      """"    SUB A, A                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x98:   sprintf (str,  " %04x : %02x      """"    SBC A, B                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x99:   sprintf (str,  " %04x : %02x      """"    SBC A, C                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x9A:   sprintf (str,  " %04x : %02x      """"    SBC A, D                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x9B:   sprintf (str,  " %04x : %02x      """"    SBC A, E                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x9C:   sprintf (str,  " %04x : %02x      """"    SBC A, H                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x9D:   sprintf (str,  " %04x : %02x      """"    SBC A, L                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x9E:   sprintf (str,  " %04x : %02x      """"    SBC A, (HL)             "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0x9F:   sprintf (str,  " %04x : %02x      """"    SBC A, A                "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0xA0:   sprintf (str,  " %04x : %02x      """"    AND A, B                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xA1:   sprintf (str,  " %04x : %02x      """"    AND A, C                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xA2:   sprintf (str,  " %04x : %02x      """"    AND A, D                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xA3:   sprintf (str,  " %04x : %02x      """"    AND A, E                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xA4:   sprintf (str,  " %04x : %02x      """"    AND A, H                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xA5:   sprintf (str,  " %04x : %02x      """"    AND A, L                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xA6:   sprintf (str,  " %04x : %02x      """"    AND A, (HL)             "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xA7:   sprintf (str,  " %04x : %02x      """"    AND A, A                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xA8:   sprintf (str,  " %04x : %02x      """"    XOR A, B                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xA9:   sprintf (str,  " %04x : %02x      """"    XOR A, C                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xAA:   sprintf (str,  " %04x : %02x      """"    XOR A, D                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xAB:   sprintf (str,  " %04x : %02x      """"    XOR A, E                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xAC:   sprintf (str,  " %04x : %02x      """"    XOR A, H                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xAD:   sprintf (str,  " %04x : %02x      """"    XOR A, L                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xAE:   sprintf (str,  " %04x : %02x      """"    XOR A, (HL)             "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xAF:   sprintf (str,  " %04x : %02x      """"    XOR A, A                "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0xB0:   sprintf (str,  " %04x : %02x      """"    OR A, B                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xB1:   sprintf (str,  " %04x : %02x      """"    OR A, C                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xB2:   sprintf (str,  " %04x : %02x      """"    OR A, D                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xB3:   sprintf (str,  " %04x : %02x      """"    OR A, E                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xB4:   sprintf (str,  " %04x : %02x      """"    OR A, H                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xB5:   sprintf (str,  " %04x : %02x      """"    OR A, L                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xB6:   sprintf (str,  " %04x : %02x      """"    OR A, (HL)              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xB7:   sprintf (str,  " %04x : %02x      """"    OR A, A                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xB8:   sprintf (str,  " %04x : %02x      """"    CP A, B                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xB9:   sprintf (str,  " %04x : %02x      """"    CP A, C                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xBA:   sprintf (str,  " %04x : %02x      """"    CP A, D                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xBB:   sprintf (str,  " %04x : %02x      """"    CP A, E                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xBC:   sprintf (str,  " %04x : %02x      """"    CP A, H                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xBD:   sprintf (str,  " %04x : %02x      """"    CP A, L                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xBE:   sprintf (str,  " %04x : %02x      """"    CP A, (HL)              "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xBF:   sprintf (str,  " %04x : %02x      """"    CP A, A                 "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0xC0:   sprintf (str,  " %04x : %02x      """"    RET NZ                  "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xC1:   sprintf (str,  " %04x : %02x      """"    POP BC                  "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xC2:   sprintf (str,  " %04x : %02x %02x %02x    JP NZ, 0x%02x%02x           ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0xC3:   sprintf (str,  " %04x : %02x %02x %02x    JP 0x%02x%02x               ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0xC4:   sprintf (str,  " %04x : %02x %02x %02x    CALL NZ, 0x%02x%02x         ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0xC5:   sprintf (str,  " %04x : %02x      """"    PUSH BC                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xC6:   sprintf (str,  " %04x : %02x %02x   ""    ADD A, 0x%02x             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0xC7:   sprintf (str,  " %04x : %02x      """"    RST 0x00                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xC8:   sprintf (str,  " %04x : %02x      """"    RET Z                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xC9:   sprintf (str,  " %04x : %02x      """"    RET                     "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xCA:   sprintf (str,  " %04x : %02x %02x %02x    JP Z, 0x%02x%02x            ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0xCC:   sprintf (str,  " %04x : %02x %02x %02x    CALL Z, 0x%02x%02x          ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0xCD:   sprintf (str,  " %04x : %02x %02x %02x    CALL 0x%02x%02x             ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0xCE:   sprintf (str,  " %04x : %02x %02x   ""    ADC A, 0x%02x             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0xCF:   sprintf (str,  " %04x : %02x      """"    RST 0x08                "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0xD0:   sprintf (str,  " %04x : %02x      """"    RET NC                  "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xD1:   sprintf (str,  " %04x : %02x      """"    POP DE                  "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xD2:   sprintf (str,  " %04x : %02x %02x %02x    JP NC, 0x%02x%02x           ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0xD3:   sprintf (str,  " %04x : %02x      """"    -                       "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xD4:   sprintf (str,  " %04x : %02x %02x %02x    CALL NC, 0x%02x%02x         ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0xD5:   sprintf (str,  " %04x : %02x      """"    PUSH DE                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xD6:   sprintf (str,  " %04x : %02x %02x   ""    SUB A, 0x%02x             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0xD7:   sprintf (str,  " %04x : %02x      """"    RST 0x10                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xD8:   sprintf (str,  " %04x : %02x      """"    RET C                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xD9:   sprintf (str,  " %04x : %02x      """"    RETI                    "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xDA:   sprintf (str,  " %04x : %02x %02x %02x    JP C, 0x%02x%02x            ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0xDB:   sprintf (str,  " %04x : %02x      """"    -                       "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xDC:   sprintf (str,  " %04x : %02x %02x %02x    CALL C, 0x%02x%02x          ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0xDD:   sprintf (str,  " %04x : %02x      """"    -                       "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xDE:   sprintf (str,  " %04x : %02x %02x   ""    SBC A, 0x%02x             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0xDF:   sprintf (str,  " %04x : %02x      """"    RST 0x18                "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0xE0:   sprintf (str,  " %04x : %02x %02x   ""    LD (0xFF00 + 0x%02x), A   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0xE1:   sprintf (str,  " %04x : %02x      """"    POP HL                  "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xE2:   sprintf (str,  " %04x : %02x      """"    LD (C), A               "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xE3:   sprintf (str,  " %04x : %02x      """"    -                       "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xE4:   sprintf (str,  " %04x : %02x      """"    -                       "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xE5:   sprintf (str,  " %04x : %02x      """"    PUSH HL                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xE6:   sprintf (str,  " %04x : %02x %02x   ""    AND A, 0x%02x             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0xE7:   sprintf (str,  " %04x : %02x      """"    RST 0x20                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xE8:   sprintf (str,  " %04x : %02x %02x   ""    ADD SP, %02x              ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0xE9:   sprintf (str,  " %04x : %02x      """"    JP HL                   "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xEA:   sprintf (str,  " %04x : %02x %02x %02x    LD (0x%02x%02x), A          ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0xEB:   sprintf (str,  " %04x : %02x      """"    -                       "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xEC:   sprintf (str,  " %04x : %02x      """"    -                       "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xED:   sprintf (str,  " %04x : %02x      """"    -                       "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xEE:   sprintf (str,  " %04x : %02x %02x   ""    XOR A, 0x%02x             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0xEF:   sprintf (str,  " %04x : %02x      """"    RST 0x28                "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0xF0:   sprintf (str,  " %04x : %02x %02x   ""    LD A, (0xFF00 + 0x%02x)   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0xF1:   sprintf (str,  " %04x : %02x      """"    POP AF                  "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xF2:   sprintf (str,  " %04x : %02x      """"    LD A, (C)               "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xF3:   sprintf (str,  " %04x : %02x      """"    DI                      "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xF4:   sprintf (str,  " %04x : %02x      """"    -                       "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xF5:   sprintf (str,  " %04x : %02x      """"    PUSH AF                 "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xF6:   sprintf (str,  " %04x : %02x %02x   ""    OR A, 0x%02x              ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0xF7:   sprintf (str,  " %04x : %02x      """"    RST 0x30                "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xF8:   sprintf (str,  " %04x : %02x %02x   ""    LD HL, SP + 0x%02x        ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0xF9:   sprintf (str,  " %04x : %02x      """"    LD SP, HL               "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xFA:   sprintf (str,  " %04x : %02x %02x %02x    LD A, (0x%02x%02x)          ",pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2), _read_byte(cpu, pc+1), _read_byte(cpu, pc+2)  );  break;
        case 0xFB:   sprintf (str,  " %04x : %02x      """"    EI                      "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xFC:   sprintf (str,  " %04x : %02x      """"    -                       "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xFD:   sprintf (str,  " %04x : %02x      """"    -                       "  , pc, _read_byte(cpu, pc)                                                          );  break;
        case 0xFE:   sprintf (str,  " %04x : %02x %02x   ""    CP A, 0x%02x              ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1), _read_byte(cpu, pc+1)                              );  break;
        case 0xFF:   sprintf (str,  " %04x : %02x      """"    RST 0x38                "  , pc, _read_byte(cpu, pc)                                                          );  break;

        case 0xCB:
            switch ( _read_byte(cpu, pc+1) ) {
                case 0x00: sprintf (str,  " %04x : %02x %02x   ""    RLC B                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x01: sprintf (str,  " %04x : %02x %02x   ""    RLC C                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x02: sprintf (str,  " %04x : %02x %02x   ""    RLC D                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x03: sprintf (str,  " %04x : %02x %02x   ""    RLC E                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x04: sprintf (str,  " %04x : %02x %02x   ""    RLC H                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x05: sprintf (str,  " %04x : %02x %02x   ""    RLC L                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x06: sprintf (str,  " %04x : %02x %02x   ""    RLC (HL)                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x07: sprintf (str,  " %04x : %02x %02x   ""    RLC A                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x08: sprintf (str,  " %04x : %02x %02x   ""    RRC B                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x09: sprintf (str,  " %04x : %02x %02x   ""    RRC C                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x0A: sprintf (str,  " %04x : %02x %02x   ""    RRC D                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x0B: sprintf (str,  " %04x : %02x %02x   ""    RRC E                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x0C: sprintf (str,  " %04x : %02x %02x   ""    RRC H                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x0D: sprintf (str,  " %04x : %02x %02x   ""    RRC L                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x0E: sprintf (str,  " %04x : %02x %02x   ""    RRC (HL)                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x0F: sprintf (str,  " %04x : %02x %02x   ""    RRC A                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;

                case 0x10: sprintf (str,  " %04x : %02x %02x   ""    RL B                    ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x11: sprintf (str,  " %04x : %02x %02x   ""    RL C                    ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x12: sprintf (str,  " %04x : %02x %02x   ""    RL D                    ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x13: sprintf (str,  " %04x : %02x %02x   ""    RL E                    ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x14: sprintf (str,  " %04x : %02x %02x   ""    RL H                    ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x15: sprintf (str,  " %04x : %02x %02x   ""    RL L                    ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x16: sprintf (str,  " %04x : %02x %02x   ""    RL (HL)                 ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x17: sprintf (str,  " %04x : %02x %02x   ""    RL A                    ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x18: sprintf (str,  " %04x : %02x %02x   ""    RR B                    ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x19: sprintf (str,  " %04x : %02x %02x   ""    RR C                    ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x1A: sprintf (str,  " %04x : %02x %02x   ""    RR D                    ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x1B: sprintf (str,  " %04x : %02x %02x   ""    RR E                    ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x1C: sprintf (str,  " %04x : %02x %02x   ""    RR H                    ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x1D: sprintf (str,  " %04x : %02x %02x   ""    RR L                    ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x1E: sprintf (str,  " %04x : %02x %02x   ""    RR (HL)                 ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x1F: sprintf (str,  " %04x : %02x %02x   ""    RR A                    ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;

                case 0x20: sprintf (str,  " %04x : %02x %02x   ""    SLA B                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x21: sprintf (str,  " %04x : %02x %02x   ""    SLA C                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x22: sprintf (str,  " %04x : %02x %02x   ""    SLA D                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x23: sprintf (str,  " %04x : %02x %02x   ""    SLA E                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x24: sprintf (str,  " %04x : %02x %02x   ""    SLA H                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x25: sprintf (str,  " %04x : %02x %02x   ""    SLA L                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x26: sprintf (str,  " %04x : %02x %02x   ""    SLA (HL)                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x27: sprintf (str,  " %04x : %02x %02x   ""    SLA A                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x28: sprintf (str,  " %04x : %02x %02x   ""    SRA B                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x29: sprintf (str,  " %04x : %02x %02x   ""    SRA C                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x2A: sprintf (str,  " %04x : %02x %02x   ""    SRA D                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x2B: sprintf (str,  " %04x : %02x %02x   ""    SRA E                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x2C: sprintf (str,  " %04x : %02x %02x   ""    SRA H                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x2D: sprintf (str,  " %04x : %02x %02x   ""    SRA L                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x2E: sprintf (str,  " %04x : %02x %02x   ""    SRA (HL)                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x2F: sprintf (str,  " %04x : %02x %02x   ""    SRA A                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;

                case 0x30: sprintf (str,  " %04x : %02x %02x   ""    SWAP B                  ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x31: sprintf (str,  " %04x : %02x %02x   ""    SWAP C                  ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x32: sprintf (str,  " %04x : %02x %02x   ""    SWAP D                  ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x33: sprintf (str,  " %04x : %02x %02x   ""    SWAP E                  ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x34: sprintf (str,  " %04x : %02x %02x   ""    SWAP H                  ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x35: sprintf (str,  " %04x : %02x %02x   ""    SWAP L                  ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x36: sprintf (str,  " %04x : %02x %02x   ""    SWAP (HL)               ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x37: sprintf (str,  " %04x : %02x %02x   ""    SWAP A                  ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x38: sprintf (str,  " %04x : %02x %02x   ""    SRL B                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x39: sprintf (str,  " %04x : %02x %02x   ""    SRL C                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x3A: sprintf (str,  " %04x : %02x %02x   ""    SRL D                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x3B: sprintf (str,  " %04x : %02x %02x   ""    SRL E                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x3C: sprintf (str,  " %04x : %02x %02x   ""    SRL H                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x3D: sprintf (str,  " %04x : %02x %02x   ""    SRL L                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x3E: sprintf (str,  " %04x : %02x %02x   ""    SRL (HL)                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x3F: sprintf (str,  " %04x : %02x %02x   ""    SRL A                   ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;

                case 0x40: sprintf (str,  " %04x : %02x %02x   ""    BIT 0, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x41: sprintf (str,  " %04x : %02x %02x   ""    BIT 0, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x42: sprintf (str,  " %04x : %02x %02x   ""    BIT 0, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x43: sprintf (str,  " %04x : %02x %02x   ""    BIT 0, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x44: sprintf (str,  " %04x : %02x %02x   ""    BIT 0, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x45: sprintf (str,  " %04x : %02x %02x   ""    BIT 0, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x46: sprintf (str,  " %04x : %02x %02x   ""    BIT 0, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x47: sprintf (str,  " %04x : %02x %02x   ""    BIT 0, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x48: sprintf (str,  " %04x : %02x %02x   ""    BIT 1, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x49: sprintf (str,  " %04x : %02x %02x   ""    BIT 1, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x4A: sprintf (str,  " %04x : %02x %02x   ""    BIT 1, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x4B: sprintf (str,  " %04x : %02x %02x   ""    BIT 1, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x4C: sprintf (str,  " %04x : %02x %02x   ""    BIT 1, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x4D: sprintf (str,  " %04x : %02x %02x   ""    BIT 1, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x4E: sprintf (str,  " %04x : %02x %02x   ""    BIT 1, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x4F: sprintf (str,  " %04x : %02x %02x   ""    BIT 1, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;

                case 0x50: sprintf (str,  " %04x : %02x %02x   ""    BIT 2, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x51: sprintf (str,  " %04x : %02x %02x   ""    BIT 2, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x52: sprintf (str,  " %04x : %02x %02x   ""    BIT 2, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x53: sprintf (str,  " %04x : %02x %02x   ""    BIT 2, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x54: sprintf (str,  " %04x : %02x %02x   ""    BIT 2, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x55: sprintf (str,  " %04x : %02x %02x   ""    BIT 2, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x56: sprintf (str,  " %04x : %02x %02x   ""    BIT 2, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x57: sprintf (str,  " %04x : %02x %02x   ""    BIT 2, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x58: sprintf (str,  " %04x : %02x %02x   ""    BIT 3, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x59: sprintf (str,  " %04x : %02x %02x   ""    BIT 3, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x5A: sprintf (str,  " %04x : %02x %02x   ""    BIT 3, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x5B: sprintf (str,  " %04x : %02x %02x   ""    BIT 3, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x5C: sprintf (str,  " %04x : %02x %02x   ""    BIT 3, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x5D: sprintf (str,  " %04x : %02x %02x   ""    BIT 3, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x5E: sprintf (str,  " %04x : %02x %02x   ""    BIT 3, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x5F: sprintf (str,  " %04x : %02x %02x   ""    BIT 3, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;

                case 0x60: sprintf (str,  " %04x : %02x %02x   ""    BIT 4, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x61: sprintf (str,  " %04x : %02x %02x   ""    BIT 4, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x62: sprintf (str,  " %04x : %02x %02x   ""    BIT 4, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x63: sprintf (str,  " %04x : %02x %02x   ""    BIT 4, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x64: sprintf (str,  " %04x : %02x %02x   ""    BIT 4, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x65: sprintf (str,  " %04x : %02x %02x   ""    BIT 4, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x66: sprintf (str,  " %04x : %02x %02x   ""    BIT 4, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x67: sprintf (str,  " %04x : %02x %02x   ""    BIT 4, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x68: sprintf (str,  " %04x : %02x %02x   ""    BIT 5, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x69: sprintf (str,  " %04x : %02x %02x   ""    BIT 5, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x6A: sprintf (str,  " %04x : %02x %02x   ""    BIT 5, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x6B: sprintf (str,  " %04x : %02x %02x   ""    BIT 5, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x6C: sprintf (str,  " %04x : %02x %02x   ""    BIT 5, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x6D: sprintf (str,  " %04x : %02x %02x   ""    BIT 5, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x6E: sprintf (str,  " %04x : %02x %02x   ""    BIT 5, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x6F: sprintf (str,  " %04x : %02x %02x   ""    BIT 5, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;

                case 0x70: sprintf (str,  " %04x : %02x %02x   ""    BIT 6, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x71: sprintf (str,  " %04x : %02x %02x   ""    BIT 6, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x72: sprintf (str,  " %04x : %02x %02x   ""    BIT 6, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x73: sprintf (str,  " %04x : %02x %02x   ""    BIT 6, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x74: sprintf (str,  " %04x : %02x %02x   ""    BIT 6, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x75: sprintf (str,  " %04x : %02x %02x   ""    BIT 6, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x76: sprintf (str,  " %04x : %02x %02x   ""    BIT 6, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x77: sprintf (str,  " %04x : %02x %02x   ""    BIT 6, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x78: sprintf (str,  " %04x : %02x %02x   ""    BIT 7, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x79: sprintf (str,  " %04x : %02x %02x   ""    BIT 7, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x7A: sprintf (str,  " %04x : %02x %02x   ""    BIT 7, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x7B: sprintf (str,  " %04x : %02x %02x   ""    BIT 7, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x7C: sprintf (str,  " %04x : %02x %02x   ""    BIT 7, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x7D: sprintf (str,  " %04x : %02x %02x   ""    BIT 7, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x7E: sprintf (str,  " %04x : %02x %02x   ""    BIT 7, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x7F: sprintf (str,  " %04x : %02x %02x   ""    BIT 7, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;

                case 0x80: sprintf (str,  " %04x : %02x %02x   ""    RES 0, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x81: sprintf (str,  " %04x : %02x %02x   ""    RES 0, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x82: sprintf (str,  " %04x : %02x %02x   ""    RES 0, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x83: sprintf (str,  " %04x : %02x %02x   ""    RES 0, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x84: sprintf (str,  " %04x : %02x %02x   ""    RES 0, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x85: sprintf (str,  " %04x : %02x %02x   ""    RES 0, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x86: sprintf (str,  " %04x : %02x %02x   ""    RES 0, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x87: sprintf (str,  " %04x : %02x %02x   ""    RES 0, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x88: sprintf (str,  " %04x : %02x %02x   ""    RES 1, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x89: sprintf (str,  " %04x : %02x %02x   ""    RES 1, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x8A: sprintf (str,  " %04x : %02x %02x   ""    RES 1, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x8B: sprintf (str,  " %04x : %02x %02x   ""    RES 1, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x8C: sprintf (str,  " %04x : %02x %02x   ""    RES 1, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x8D: sprintf (str,  " %04x : %02x %02x   ""    RES 1, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x8E: sprintf (str,  " %04x : %02x %02x   ""    RES 1, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x8F: sprintf (str,  " %04x : %02x %02x   ""    RES 1, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;

                case 0x90: sprintf (str,  " %04x : %02x %02x   ""    RES 2, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x91: sprintf (str,  " %04x : %02x %02x   ""    RES 2, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x92: sprintf (str,  " %04x : %02x %02x   ""    RES 2, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x93: sprintf (str,  " %04x : %02x %02x   ""    RES 2, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x94: sprintf (str,  " %04x : %02x %02x   ""    RES 2, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x95: sprintf (str,  " %04x : %02x %02x   ""    RES 2, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x96: sprintf (str,  " %04x : %02x %02x   ""    RES 2, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x97: sprintf (str,  " %04x : %02x %02x   ""    RES 2, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x98: sprintf (str,  " %04x : %02x %02x   ""    RES 3, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x99: sprintf (str,  " %04x : %02x %02x   ""    RES 3, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x9A: sprintf (str,  " %04x : %02x %02x   ""    RES 3, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x9B: sprintf (str,  " %04x : %02x %02x   ""    RES 3, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x9C: sprintf (str,  " %04x : %02x %02x   ""    RES 3, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x9D: sprintf (str,  " %04x : %02x %02x   ""    RES 3, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x9E: sprintf (str,  " %04x : %02x %02x   ""    RES 3, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0x9F: sprintf (str,  " %04x : %02x %02x   ""    RES 3, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;

                case 0xA0: sprintf (str,  " %04x : %02x %02x   ""    RES 4, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xA1: sprintf (str,  " %04x : %02x %02x   ""    RES 4, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xA2: sprintf (str,  " %04x : %02x %02x   ""    RES 4, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xA3: sprintf (str,  " %04x : %02x %02x   ""    RES 4, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xA4: sprintf (str,  " %04x : %02x %02x   ""    RES 4, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xA5: sprintf (str,  " %04x : %02x %02x   ""    RES 4, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xA6: sprintf (str,  " %04x : %02x %02x   ""    RES 4, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xA7: sprintf (str,  " %04x : %02x %02x   ""    RES 4, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xA8: sprintf (str,  " %04x : %02x %02x   ""    RES 5, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xA9: sprintf (str,  " %04x : %02x %02x   ""    RES 5, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xAA: sprintf (str,  " %04x : %02x %02x   ""    RES 5, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xAB: sprintf (str,  " %04x : %02x %02x   ""    RES 5, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xAC: sprintf (str,  " %04x : %02x %02x   ""    RES 5, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xAD: sprintf (str,  " %04x : %02x %02x   ""    RES 5, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xAE: sprintf (str,  " %04x : %02x %02x   ""    RES 5, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xAF: sprintf (str,  " %04x : %02x %02x   ""    RES 5, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;

                case 0xB0: sprintf (str,  " %04x : %02x %02x   ""    RES 6, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xB1: sprintf (str,  " %04x : %02x %02x   ""    RES 6, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xB2: sprintf (str,  " %04x : %02x %02x   ""    RES 6, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xB3: sprintf (str,  " %04x : %02x %02x   ""    RES 6, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xB4: sprintf (str,  " %04x : %02x %02x   ""    RES 6, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xB5: sprintf (str,  " %04x : %02x %02x   ""    RES 6, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xB6: sprintf (str,  " %04x : %02x %02x   ""    RES 6, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xB7: sprintf (str,  " %04x : %02x %02x   ""    RES 6, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xB8: sprintf (str,  " %04x : %02x %02x   ""    RES 7, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xB9: sprintf (str,  " %04x : %02x %02x   ""    RES 7, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xBA: sprintf (str,  " %04x : %02x %02x   ""    RES 7, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xBB: sprintf (str,  " %04x : %02x %02x   ""    RES 7, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xBC: sprintf (str,  " %04x : %02x %02x   ""    RES 7, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xBD: sprintf (str,  " %04x : %02x %02x   ""    RES 7, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xBE: sprintf (str,  " %04x : %02x %02x   ""    RES 7, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xBF: sprintf (str,  " %04x : %02x %02x   ""    RES 7, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;

                case 0xC0: sprintf (str,  " %04x : %02x %02x   ""    SET 0, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xC1: sprintf (str,  " %04x : %02x %02x   ""    SET 0, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xC2: sprintf (str,  " %04x : %02x %02x   ""    SET 0, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xC3: sprintf (str,  " %04x : %02x %02x   ""    SET 0, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xC4: sprintf (str,  " %04x : %02x %02x   ""    SET 0, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xC5: sprintf (str,  " %04x : %02x %02x   ""    SET 0, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xC6: sprintf (str,  " %04x : %02x %02x   ""    SET 0, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xC7: sprintf (str,  " %04x : %02x %02x   ""    SET 0, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xC8: sprintf (str,  " %04x : %02x %02x   ""    SET 1, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xC9: sprintf (str,  " %04x : %02x %02x   ""    SET 1, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xCA: sprintf (str,  " %04x : %02x %02x   ""    SET 1, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xCB: sprintf (str,  " %04x : %02x %02x   ""    SET 1, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xCC: sprintf (str,  " %04x : %02x %02x   ""    SET 1, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xCD: sprintf (str,  " %04x : %02x %02x   ""    SET 1, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xCE: sprintf (str,  " %04x : %02x %02x   ""    SET 1, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xCF: sprintf (str,  " %04x : %02x %02x   ""    SET 1, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;

                case 0xD0: sprintf (str,  " %04x : %02x %02x   ""    SET 2, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xD1: sprintf (str,  " %04x : %02x %02x   ""    SET 2, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xD2: sprintf (str,  " %04x : %02x %02x   ""    SET 2, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xD3: sprintf (str,  " %04x : %02x %02x   ""    SET 2, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xD4: sprintf (str,  " %04x : %02x %02x   ""    SET 2, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xD5: sprintf (str,  " %04x : %02x %02x   ""    SET 2, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xD6: sprintf (str,  " %04x : %02x %02x   ""    SET 2, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xD7: sprintf (str,  " %04x : %02x %02x   ""    SET 2, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xD8: sprintf (str,  " %04x : %02x %02x   ""    SET 3, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xD9: sprintf (str,  " %04x : %02x %02x   ""    SET 3, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xDA: sprintf (str,  " %04x : %02x %02x   ""    SET 3, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xDB: sprintf (str,  " %04x : %02x %02x   ""    SET 3, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xDC: sprintf (str,  " %04x : %02x %02x   ""    SET 3, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xDD: sprintf (str,  " %04x : %02x %02x   ""    SET 3, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xDE: sprintf (str,  " %04x : %02x %02x   ""    SET 3, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xDF: sprintf (str,  " %04x : %02x %02x   ""    SET 3, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;

                case 0xE0: sprintf (str,  " %04x : %02x %02x   ""    SET 4, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xE1: sprintf (str,  " %04x : %02x %02x   ""    SET 4, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xE2: sprintf (str,  " %04x : %02x %02x   ""    SET 4, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xE3: sprintf (str,  " %04x : %02x %02x   ""    SET 4, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xE4: sprintf (str,  " %04x : %02x %02x   ""    SET 4, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xE5: sprintf (str,  " %04x : %02x %02x   ""    SET 4, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xE6: sprintf (str,  " %04x : %02x %02x   ""    SET 4, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xE7: sprintf (str,  " %04x : %02x %02x   ""    SET 4, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xE8: sprintf (str,  " %04x : %02x %02x   ""    SET 5, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xE9: sprintf (str,  " %04x : %02x %02x   ""    SET 5, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xEA: sprintf (str,  " %04x : %02x %02x   ""    SET 5, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xEB: sprintf (str,  " %04x : %02x %02x   ""    SET 5, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xEC: sprintf (str,  " %04x : %02x %02x   ""    SET 5, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xED: sprintf (str,  " %04x : %02x %02x   ""    SET 5, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xEE: sprintf (str,  " %04x : %02x %02x   ""    SET 5, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xEF: sprintf (str,  " %04x : %02x %02x   ""    SET 5, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;

                case 0xF0: sprintf (str,  " %04x : %02x %02x   ""    SET 6, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xF1: sprintf (str,  " %04x : %02x %02x   ""    SET 6, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xF2: sprintf (str,  " %04x : %02x %02x   ""    SET 6, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xF3: sprintf (str,  " %04x : %02x %02x   ""    SET 6, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xF4: sprintf (str,  " %04x : %02x %02x   ""    SET 6, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xF5: sprintf (str,  " %04x : %02x %02x   ""    SET 6, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xF6: sprintf (str,  " %04x : %02x %02x   ""    SET 6, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xF7: sprintf (str,  " %04x : %02x %02x   ""    SET 6, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xF8: sprintf (str,  " %04x : %02x %02x   ""    SET 7, B                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xF9: sprintf (str,  " %04x : %02x %02x   ""    SET 7, C                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xFA: sprintf (str,  " %04x : %02x %02x   ""    SET 7, D                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xFB: sprintf (str,  " %04x : %02x %02x   ""    SET 7, E                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xFC: sprintf (str,  " %04x : %02x %02x   ""    SET 7, H                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xFD: sprintf (str,  " %04x : %02x %02x   ""    SET 7, L                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xFE: sprintf (str,  " %04x : %02x %02x   ""    SET 7, (HL)             ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
                case 0xFF: sprintf (str,  " %04x : %02x %02x   ""    SET 7, A                ", pc, _read_byte(cpu, pc), _read_byte(cpu, pc+1)                                                          );  break;
            }
            break;

        default:
            sprintf (str,  " %04x is not implemented " , _read_byte(cpu, pc) );
            break;
    }

    return;
}

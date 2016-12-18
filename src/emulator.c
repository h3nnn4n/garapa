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

#include "instructions_0xcb.h"
#include "instructions_branch.h"
#include "instructions_logical.h"
#include "instructions_arithmetic.h"
#include "instructions_data_transfer.h"
#include "instructions_stack_io_control.h"

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
        case 0x00: emulate_NOP  ( cpu );
            break;
        case 0xde:
            emulate_SBI( cpu );
            break;
        case 0x2a:
        case 0x3a:
            emulate_LDA( cpu );
            break;
        case 0x02:
        case 0x12:
            emulate_STAX( cpu );
            break;
        case 0x0a:
        case 0x1a:
            emulate_LDAX ( cpu );
            break;
        case 0xd2:
            emulate_JNC ( cpu );
            break;
        case 0xc0:
            emulate_RNZ ( cpu );
            break;
        case 0xdc:
            emulate_CC ( cpu );
            break;
        case 0x07:
            emulate_RLC ( cpu, 0x07 );
            cpu->flags.z = 0;
            cpu->pc -= 1; // RLC eats 2 bytes thanks to the 0xcb stuff
            break;
        case 0x17:
            emulate_RL ( cpu, 0x07 );
            cpu->flags.z = 0;
            cpu->pc -= 1;
            break;
        case 0x0F:
            emulate_RRC ( cpu, 0x07 );
            cpu->flags.z = 0;
            cpu->pc -= 1;
            break;
        case 0x1f:
            emulate_RR ( cpu, 0x07 );
            cpu->flags.z = 0;
            cpu->pc -= 1;
            break;
        case 0xd4:
            emulate_CNC ( cpu );
            break;
        case 0xc7:
        case 0xd7:
        case 0xe7:
        case 0xf7:
        case 0xcf:
        case 0xdf:
        case 0xef:
        case 0xff:
            emulate_RST ( cpu );
            break;
        case 0xd9:
            emulate_RETI ( cpu );
            break;
        case 0xd8:
            emulate_RC ( cpu );
            break;
        case 0xda:
            emulate_JC ( cpu );
            break;
        case 0xca:
            emulate_JZ ( cpu );
            break;
        case 0xcc:
            emulate_CZ ( cpu );
            break;
        case 0xc2:
            emulate_JNZ ( cpu );
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
        case 0x38:
            cpu->pc = !cpu->flags.c ?
                        cpu->pc + 2   :
                        cpu->pc + (int8_t) opcode[1] + 2;
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
        case 0x27:
            emulate_DAA  ( cpu );
            break;
        case 0x3f:
            emulate_CMC  ( cpu );
            break;
        case 0x37:
            emulate_STC  ( cpu );
            break;
        case 0x2f:
            emulate_CMA  ( cpu );
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
        case 0x01:
        case 0x11:
        case 0x21:
        case 0x31:
            emulate_LD  ( cpu );
            break;
        case 0x32: // LD (HL-),A
            addr              = cpu->h << 8 | cpu->l;
            write_byte(cpu, addr, cpu->a);
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
            write_byte(cpu, 0xff00 + opcode[1],  cpu->a);
            cpu->cycles += 12;
            cpu->pc     += 2 ;
            break;
        case 0xf0:
            cpu->a = read_byte(cpu, 0xff00 + opcode[1]);
            cpu->cycles += 12;
            cpu->pc     += 2 ;
            break;
        case 0xf2:
            cpu->a = read_byte(cpu, 0xff00 + cpu->c);
            cpu->cycles += 12;
            cpu->pc     += 1 ;
            break;
        case 0xe2:
            write_byte(cpu, 0xff00 + cpu->c,  cpu->a);
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
            write_byte(cpu, addr, cpu->a);
            cpu->cycles += 8;
            cpu->pc     += 1;
            break;
        case 0xc8:
            emulate_RZ ( cpu );
            break;
        case 0xf9:
            cpu->sp = cpu->h << 8 | cpu->l;
            cpu->cycles += 5;
            cpu->pc     += 1 ;
            break;
        case 0xfe:
            emulate_CPI ( cpu );
            break;
        case 0x08:
            {
                uint16_t addr = ( opcode[2] << 8 ) | opcode[1];
                write_byte(cpu, addr + 0, ( cpu->sp & 0x00ff ) >> 0 );
                write_byte(cpu, addr + 1, ( cpu->sp & 0xff00 ) >> 8 );
                cpu->pc     += 3;
                cpu->cycles += 20;
            }
            break;
        case 0x0b:
        case 0x1b:
        case 0x2b:
        case 0x3b:
            emulate_DCX ( cpu );
            break;
        case 0x09:
        case 0x19:
        case 0x29:
        case 0x39:
            emulate_DAD ( cpu );
            break;
        case 0xa0:
        case 0xa1:
        case 0xa2:
        case 0xa3:
        case 0xa4:
        case 0xa5:
        case 0xa6:
        case 0xa7:
            emulate_ANA ( cpu );
            break;
        case 0xb8:
        case 0xb9:
        case 0xba:
        case 0xbb:
        case 0xbc:
        case 0xbd:
        case 0xbe:
        case 0xbf:
            emulate_CMP ( cpu );
            break;
        case 0x90:
        case 0x91:
        case 0x92:
        case 0x93:
        case 0x94:
        case 0x95:
        case 0x96:
        case 0x97:
            emulate_SUB ( cpu );
            break;
        case 0x98:
        case 0x99:
        case 0x9a:
        case 0x9b:
        case 0x9c:
        case 0x9d:
        case 0x9e:
        case 0x9f:
            emulate_SBB ( cpu );
            break;
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
            emulate_ADD ( cpu );
            break;
        case 0x88:
        case 0x89:
        case 0x8a:
        case 0x8b:
        case 0x8c:
        case 0x8d:
        case 0x8e:
        case 0x8f:
            emulate_ADC ( cpu );
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
        case 0xf6:
            emulate_ORI ( cpu );
            break;
        case 0x0e:
        case 0x1e:
        case 0x2e:
        case 0x3e:
        case 0x06:
        case 0x16:
        case 0x26:
        case 0x36:
            emulate_MVI ( cpu );
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
        case 0xe8:
        case 0xf8:
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
        case 0xfb:
            emulate_EI ( cpu );
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
            decode_0xcb ( cpu );
            break;
        default:
            disassembler( cpu->memory, cpu->pc );
            printf(" %2x is not implemented\n", *opcode);
            exit(-1);
    }

    cpu->instructions_executed += 1;

#ifdef __show_step
    print_registers(cpu);
    puts("");
#endif
}

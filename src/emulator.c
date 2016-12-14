#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "types.h"
#include "utils.h"
#include "emulator.h"
#include "disassembler.h"

#include "instructions_stack_io_control.h"
#include "instructions_data_transfer.h"
#include "instructions_arithmetic.h"
#include "instructions_logical.h"
#include "instructions_branch.h"

void emulator( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

#ifdef __show_step
    disassembler ( cpu->memory, cpu->pc );
    print_registers(cpu);
#endif

           if ( ( *opcode >= 0x40 && *opcode <= 0x75 ) || ( *opcode >= 0x77 && *opcode <= 0x7f ) ) {
        emulate_MOV ( cpu );
    } else if ( *opcode >= 0x80 && *opcode <= 0x87 ) {
        emulate_ADD ( cpu );
    } else if ( *opcode >= 0x88 && *opcode <= 0x8f ) {
        emulate_ADC ( cpu );
    } else if ( *opcode >= 0x90 && *opcode <= 0x97 ) {
        emulate_SUB ( cpu );
    } else if ( *opcode >= 0x98 && *opcode <= 0x9f ) {
        emulate_SBB ( cpu );
    } else if ( *opcode >= 0xa0 && *opcode <= 0xa7 ) {
        emulate_ANA ( cpu );
    } else if ( *opcode >= 0xa8 && *opcode <= 0xaf ) {
        emulate_XRA ( cpu );
    } else if ( *opcode >= 0xb0 && *opcode <= 0xb7 ) {
        emulate_ORA ( cpu );
    } else if ( *opcode >= 0xb8 && *opcode <= 0xbf ) {
        emulate_CMP ( cpu );
    } else {
        switch ( *opcode ) {
            case 0x00: emulate_NOP  ( cpu );
                break;
            case 0x05:
            case 0x0d:
            case 0x15:
            case 0x1d:
            case 0x25:
            case 0x2d:
            case 0x35:
            case 0x3d:
                emulate_DCR  ( cpu );
                break;
            case 0x27:
                emulate_DAA  ( cpu );
                break;
            case 0xd6:
                emulate_SUI  ( cpu );
                break;
            case 0x1f:
                emulate_RAR  ( cpu ); break;
            case 0x17:
                emulate_RAL  ( cpu );
                break;
            case 0x07:
                emulate_RLC  ( cpu );
                break;
            case 0x0f:
                emulate_RRC  ( cpu );
                break;
            case 0x3f:
                emulate_CMC  ( cpu );
                break;
            case 0x2f:
                emulate_CMA  ( cpu );
                break;
            case 0x32:
                emulate_STA  ( cpu );
                break;
            case 0x37:
                emulate_STC  ( cpu );
                break;
            case 0x3a:
                emulate_LDA  ( cpu );
                break;
            case 0x1a:
            case 0x0a:
                emulate_LDAX ( cpu );
                break;
            case 0x01:
            case 0x11:
            case 0x21:
            case 0x31:
                emulate_LXI  ( cpu );
                break;
            case 0x03:
            case 0x13:
            case 0x23:
            case 0x33:
                emulate_INX  ( cpu );
                break;
            case 0x09:
            case 0x19:
            case 0x29:
            case 0x39:
                emulate_DAD  ( cpu );
                break;
            case 0x04:
            case 0x0c:
            case 0x14:
            case 0x1c:
            case 0x24:
            case 0x2c:
            case 0x34:
            case 0x3c:
                emulate_INR  ( cpu );
                break;
            case 0x3e:
            case 0x36:
            case 0x26:
            case 0x2e:
            case 0x06:
            case 0x0e:
            case 0x16:
            case 0x1e:
                emulate_MVI  ( cpu );
                break;
            case 0xde:
                emulate_SBI  ( cpu );
                break;
            case 0xfa:
                emulate_JM   ( cpu );
                break;
            case 0xca:
                emulate_JZ   ( cpu );
                break;
            case 0xe2:
                emulate_JPO  ( cpu );
                break;
            case 0xea:
                emulate_JPE  ( cpu );
                break;
            case 0xc2:
                emulate_JNZ  ( cpu );
                break;
            case 0xc3:
                emulate_JMP  ( cpu );
                break;
            case 0xd2:
                emulate_JNC  ( cpu );
                break;
            case 0xf2:
                emulate_JP   ( cpu );
                break;
            case 0xda:
                emulate_JC   ( cpu );
                break;
            case 0xce:
                emulate_ACI  ( cpu );
                break;
            case 0xc6:
                emulate_ADI  ( cpu );
                break;
            case 0xd0:
                emulate_RNC  ( cpu );
                break;
            case 0xf0:
                emulate_RP   ( cpu );
                break;
            case 0xd8:
                emulate_RC   ( cpu );
                break;
            case 0xf8:
                emulate_RM   ( cpu );
                break;
            case 0xe0:
                emulate_RPO  ( cpu );
                break;
            case 0xe8:
                emulate_RPE  ( cpu );
                break;
            case 0xc0:
                emulate_RNZ  ( cpu );
                break;
            case 0xc8:
                emulate_RZ   ( cpu );
                break;
            case 0xc9:
                emulate_RET  ( cpu );
                break;
            case 0xe4:
                emulate_CPO  ( cpu );
                break;
            case 0xec:
                emulate_CPE  ( cpu );
                break;
            case 0xd4:
                emulate_CNC  ( cpu );
                break;
            case 0xf4:
                emulate_CP   ( cpu );
                break;
            case 0xfc:
                emulate_CM   ( cpu );
                break;
            case 0xdc:
                emulate_CC   ( cpu );
                break;
            case 0xc4:
                emulate_CNZ  ( cpu );
                break;
            case 0xcc:
                emulate_CZ   ( cpu );
                break;
            case 0xcd:
                emulate_CALL ( cpu );
                break;
            case 0xe1:
            case 0xd1:
            case 0xc1:
            case 0xf1:
                emulate_POP  ( cpu );
                break;
            case 0x0b:
            case 0x1b:
            case 0x2b:
            case 0x3b:
                emulate_DCX  ( cpu );
                break;
            case 0xc5:
            case 0xd5:
            case 0xe5:
            case 0xf5:
                emulate_PUSH ( cpu );
                break;
            case 0xdb:
                emulate_IN   ( cpu );
                break;
            case 0xd3:
                emulate_OUT  ( cpu );
                break;
            case 0xee:
                emulate_XRI  ( cpu );
                break;
            case 0xf6:
                emulate_ORI  ( cpu );
                break;
            case 0xe6:
                emulate_ANI  ( cpu );
                break;
            case 0xeb:
                emulate_XCHG ( cpu );
                break;
            case 0xf3:
                emulate_DI   ( cpu );
                break;
            case 0xfb:
                emulate_EI   ( cpu );
                break;
            case 0xfe:
                emulate_CPI  ( cpu );
                break;
            case 0xf9:
                emulate_SPHL ( cpu );
                break;
            case 0xe9:
                emulate_PCHL ( cpu );
                break;
            case 0xe3:
                emulate_XTHL ( cpu );
                break;
            case 0x22:
                emulate_SHLD ( cpu );
                break;
            case 0x2a:
                emulate_LHLD ( cpu );
                break;
            case 0x02:
            case 0x12:
                emulate_STAX ( cpu );
                break;
            case 0xc7:
                emulate_RST  ( cpu );
                break;
            case 0xd7:
                emulate_RST  ( cpu );
                break;
            case 0xe7:
                emulate_RST  ( cpu );
                break;
            case 0xf7:
                emulate_RST  ( cpu );
                break;
            case 0xcf:
                emulate_RST  ( cpu );
                break;
            case 0xdf:
                emulate_RST  ( cpu );
                break;
            case 0xef:
                emulate_RST  ( cpu );
                break;
            case 0xff:
                emulate_RST  ( cpu );
                break;
            default:
                disassembler ( cpu->memory, cpu->pc );
                print_registers(cpu);
                printf(" %2X is not implemented\n", cpu->memory[cpu->pc]);
                exit(-1);
        }
    }

    cpu->instructions_executed += 1;

#ifdef __show_step
    print_registers(cpu);
    puts("");
#endif
}

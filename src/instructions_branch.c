#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "types.h"

#include "instructions_branch.h"

void emulate_JMP ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xc3:
            addr = opcode[2] << 8 | opcode[1];
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 4  ;
    cpu->pc      = addr;
}

void emulate_JC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xda: // JC
            if ( cpu->flags.c  ) {
                addr = opcode[2] << 8 | opcode[1];
                cpu->pc = addr;
            } else {
                cpu->pc += 3;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 3  ;
}

void emulate_JNC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xd2: // JNC
            if ( !cpu->flags.c  ) {
                addr = opcode[2] << 8 | opcode[1];
                cpu->pc = addr;
            } else {
                cpu->pc += 3;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 3  ;
}

void emulate_JM ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr = -1;

    switch ( *opcode ) {
        case 0xfa: // JM
            addr = opcode[2] << 8 | opcode[1];
            if ( cpu->flags.n ) {
                cpu->pc = addr;
                /*cpu->cycles_machine += 5;*/
            } else {
                cpu->pc += 3;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 4;
}

void emulate_JP ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xf2: // JP
            if ( !cpu->flags.n ) {
                addr = opcode[2] << 8 | opcode[1];
                cpu->pc = addr;
            } else {
                cpu->pc += 3;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 2;
}

void emulate_JZ ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xca: // JZ
            addr = opcode[2] << 8 | opcode[1];
            if ( cpu->flags.z ) {
                cpu->pc = addr;
            } else {
                cpu->pc += 3;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 3;
}

void emulate_JNZ ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr = 0;

    switch ( *opcode ) {
        case 0xc2: // JNZ
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

    cpu->cycles_machine += 3;
}

void emulate_RETI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xd9:
            addr = cpu->mem_controller.memory[cpu->sp+1] << 8 | cpu->mem_controller.memory[cpu->sp];
            cpu->sp += 2;
            cpu->pc = addr;
            cpu->enable_interrupts = 1;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 4;
}

void emulate_RET ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xc9:
            addr = cpu->mem_controller.memory[cpu->sp+1] << 8 | cpu->mem_controller.memory[cpu->sp];
            cpu->sp += 2;
            cpu->pc = addr;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 4;
}

void emulate_RZ ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xc8:
            if ( cpu->flags.z ) {
                addr = cpu->mem_controller.memory[cpu->sp+1] << 8 | cpu->mem_controller.memory[cpu->sp];
                cpu->sp += 2;
                cpu->pc = addr;
                cpu->cycles_machine += 2;
            } else {
                cpu->pc += 1;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 1;
}

void emulate_RNZ ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xc0:
            if ( !cpu->flags.z ) {
                addr = cpu->mem_controller.memory[cpu->sp+1] << 8 | cpu->mem_controller.memory[cpu->sp];
                cpu->sp += 2;
                cpu->pc = addr;
                cpu->cycles_machine += 2;
            } else {
                cpu->pc += 1;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 1;
}

void emulate_RP ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xf0:
            if ( !cpu->flags.n ) {
                addr = cpu->mem_controller.memory[cpu->sp+1] << 8 | cpu->mem_controller.memory[cpu->sp];
                cpu->sp += 2;
                cpu->pc = addr;
                cpu->cycles_machine += 6;
            } else {
                cpu->pc += 1;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 5;
}

void emulate_RM ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xf8:
            if ( cpu->flags.n ) {
                addr = cpu->mem_controller.memory[cpu->sp+1] << 8 | cpu->mem_controller.memory[cpu->sp];
                cpu->sp += 2;
                cpu->pc = addr;
            } else {
                cpu->pc += 1;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 3;
}

void emulate_RNC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xd0: // RNC
            if ( !cpu->flags.c  ) {
                addr = cpu->mem_controller.memory[cpu->sp+1] << 8 | cpu->mem_controller.memory[cpu->sp];
                cpu->sp += 2;
                cpu->pc = addr;
                cpu->cycles_machine += 2;
            } else {
                cpu->pc += 1;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 1;
}

void emulate_RC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t addr;

    switch ( *opcode ) {
        case 0xd8:
            if ( cpu->flags.c  ) {
                addr = cpu->mem_controller.memory[cpu->sp+1] << 8 | cpu->mem_controller.memory[cpu->sp];
                cpu->sp += 2;
                cpu->pc = addr;
                cpu->cycles_machine += 2;
            } else {
                cpu->pc += 1;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 1;
}

void emulate_RST ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];
    uint16_t ret = 0;

    ret = cpu->pc + 1;

    switch ( *opcode ) {
        case 0xc7:
            cpu->pc = 0x00;
            break;
        case 0xcf:
            cpu->pc = 0x08;
            break;
        case 0xd7:
            cpu->pc = 0x10;
            break;
        case 0xdf:
            cpu->pc = 0x18;
            break;
        case 0xe7:
            cpu->pc = 0x20;
            break;
        case 0xef:
            cpu->pc = 0x28;
            break;
        case 0xf7:
            cpu->pc = 0x30;
            break;
        case 0xff:
            cpu->pc = 0x38;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->mem_controller.memory[(cpu->sp-1) & 0xffff] = (ret >> 8) & 0xff;
    cpu->mem_controller.memory[(cpu->sp-2) & 0xffff] = (ret & 0xff);
    cpu->sp                           = cpu->sp - 2;

    cpu->cycles_machine += 4;
}

void emulate_PCHL ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];

    switch ( *opcode ) {
        case 0xe9: // PCHL
            cpu->pc = cpu->h << 8 | cpu->l;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 1 ;
}

void emulate_CNC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];

    switch ( *opcode ) {
        case 0xd4:
            {
                if ( !cpu->flags.c  ) {
                    uint16_t ret           = cpu->pc + 3;
                    cpu->mem_controller.memory[cpu->sp-1] = (ret >> 8) & 0xff;
                    cpu->mem_controller.memory[cpu->sp-2] = (ret & 0xff);
                    cpu->sp                = cpu->sp - 2;
                    cpu->pc                = opcode[2] << 8 | opcode[1];
                    cpu->cycles_machine += 6;
                } else {
                    cpu->pc     += 3;
                    cpu->cycles_machine += 3;
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

void emulate_CC ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];

    switch ( *opcode ) {
        case 0xdc:
            {
                if ( cpu->flags.c  ) {
                    uint16_t ret           = cpu->pc + 3;
                    cpu->mem_controller.memory[cpu->sp-1] = (ret >> 8) & 0xff;
                    cpu->mem_controller.memory[cpu->sp-2] = (ret & 0xff);
                    cpu->sp                = cpu->sp - 2;
                    cpu->pc                = opcode[2] << 8 | opcode[1];
                    cpu->cycles_machine += 6;
                } else {
                    cpu->pc     += 3;
                    cpu->cycles_machine += 3;
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

void emulate_CNZ ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];

    switch ( *opcode ) {
        case 0xc4: // CNZ
            {
                if ( !cpu->flags.z ) {
                    uint16_t ret           = cpu->pc + 3;
                    cpu->mem_controller.memory[cpu->sp-1] = (ret >> 8) & 0xff;
                    cpu->mem_controller.memory[cpu->sp-2] = (ret & 0xff);
                    cpu->sp                = cpu->sp - 2;
                    cpu->pc                = opcode[2] << 8 | opcode[1];
                    cpu->cycles_machine += 6;
                } else {
                    cpu->pc     += 3;
                    cpu->cycles_machine += 3;
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

void emulate_CZ ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];

    switch ( *opcode ) {
        case 0xcc: //CZ
            {
                if ( cpu->flags.z ) {
                    uint16_t ret           = cpu->pc + 3;
                    cpu->mem_controller.memory[cpu->sp-1] = (ret >> 8) & 0xff;
                    cpu->mem_controller.memory[cpu->sp-2] = (ret & 0xff);
                    cpu->sp                = cpu->sp - 2;
                    cpu->pc                = opcode[2] << 8 | opcode[1];
                    cpu->cycles_machine += 6;
                } else {
                    cpu->pc     += 3;
                    cpu->cycles_machine += 3;
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }
}

void emulate_CALL ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->mem_controller.memory[cpu->pc];

    switch ( *opcode ) {
        case 0xcd:
            {
                /*assert ( cpu->sp >= 2 && "Got a segfault in the 8080" );*/
                uint16_t ret           = cpu->pc + 3;
                cpu->mem_controller.memory[(cpu->sp-1) & 0xffff] = (ret >> 8) & 0xff;
                cpu->mem_controller.memory[(cpu->sp-2) & 0xffff] = (ret & 0xff);
                cpu->sp                = cpu->sp - 2;
                cpu->pc                = opcode[2] << 8 | opcode[1];
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles_machine += 6;
}

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "types.h"
#include "microcode.h"
#include "time_keeper.h"

#include "instructions_branch.h"

void emulate_JMP ( _cpu_info *cpu ) {
    uint16_t addr = 0;

    addr  = read_byte_at_pc ( cpu );
    addr |= read_byte_at_pc ( cpu ) << 8;
    cpu->pc = addr;
}

void emulate_JC ( _cpu_info *cpu ) {
    uint16_t addr = 0;

    // FIXME
    // I think both cases take the same ammount of cycles
    if ( cpu->flags.c  ) {
        addr  = read_byte_at_pc ( cpu );
        addr |= read_byte_at_pc ( cpu ) << 8;
        cpu->pc = addr;
    } else {
        cpu->pc += 3;
    }
}

void emulate_JNC ( _cpu_info *cpu ) {
    uint16_t addr = 0;

    if ( !cpu->flags.c  ) {
        addr  = read_byte_at_pc ( cpu );
        addr |= read_byte_at_pc ( cpu ) << 8;
        cpu->pc = addr;
    } else {
        cpu->pc += 3;
    }
}

void emulate_JM ( _cpu_info *cpu ) {
    uint16_t addr = 0;

    if ( cpu->flags.n ) {
        addr  = read_byte_at_pc ( cpu );
        addr |= read_byte_at_pc ( cpu ) << 8;
        cpu->pc = addr;
    } else {
        cpu->pc += 3;
    }
}

void emulate_JP ( _cpu_info *cpu ) {
    uint16_t addr = 0;

    if ( !cpu->flags.n ) {
        addr  = read_byte_at_pc ( cpu );
        addr |= read_byte_at_pc ( cpu ) << 8;
        cpu->pc = addr;
    } else {
        cpu->pc += 3;
    }
}

void emulate_JZ ( _cpu_info *cpu ) {
    uint16_t addr = 0;

    if ( cpu->flags.z ) {
        addr  = read_byte_at_pc ( cpu );
        addr |= read_byte_at_pc ( cpu ) << 8;
        cpu->pc = addr;
    } else {
        cpu->pc += 3;
    }
}

void emulate_JNZ ( _cpu_info *cpu ) {
    uint16_t addr = 0;

    if ( !cpu->flags.z ) {
        addr  = read_byte_at_pc ( cpu );
        addr |= read_byte_at_pc ( cpu ) << 8;
        cpu->pc = addr;
    } else {
        cpu->pc += 3;
    }
}

void emulate_RETI ( _cpu_info *cpu ) {
    uint16_t addr;

    addr  = read_byte_at_sp ( cpu );
    addr |= read_byte_at_sp ( cpu ) << 8;

    timer_tick_and_full_mcycle ( cpu ); // A full cycle while the cpu does something

    cpu->pc = addr;

    cpu->enable_interrupts = 1;
}

void emulate_RET ( _cpu_info *cpu ) {
    uint16_t addr;

    addr  = read_byte_at_sp ( cpu );

    addr |= read_byte_at_sp ( cpu ) << 8;

    cpu->pc = addr;
}

void emulate_RZ ( _cpu_info *cpu ) {
    uint16_t addr;

    if ( cpu->flags.z ) {
        addr  = read_byte_at_sp ( cpu );
        addr |= read_byte_at_sp ( cpu ) << 8;

        cpu->pc = addr;
    } else {
        cpu->pc += 1;
    }
}

void emulate_RNZ ( _cpu_info *cpu ) {
    uint16_t addr;

    if ( !cpu->flags.z ) {
        addr  = read_byte_at_sp ( cpu );
        addr |= read_byte_at_sp ( cpu ) << 8;

        cpu->pc = addr;
    } else {
        cpu->pc += 1;
    }
}

void emulate_RP ( _cpu_info *cpu ) {
    uint16_t addr;

    if ( !cpu->flags.n ) {
        addr  = read_byte_at_sp ( cpu );
        addr |= read_byte_at_sp ( cpu ) << 8;

        cpu->pc = addr;
    } else {
        cpu->pc += 1;
    }
}

void emulate_RM ( _cpu_info *cpu ) {
    uint16_t addr;

    if ( cpu->flags.n ) {
        addr  = read_byte_at_sp ( cpu );
        addr |= read_byte_at_sp ( cpu ) << 8;

        cpu->pc = addr;
    } else {
        cpu->pc += 1;
    }
}

void emulate_RNC ( _cpu_info *cpu ) {
    uint16_t addr;

    if ( !cpu->flags.c ) {
        addr  = read_byte_at_sp ( cpu );
        addr |= read_byte_at_sp ( cpu ) << 8;

        cpu->pc = addr;
    } else {
        cpu->pc += 1;
    }
}

void emulate_RC ( _cpu_info *cpu ) {
    uint16_t addr;

    if ( cpu->flags.c ) {
        addr  = read_byte_at_sp ( cpu );
        addr |= read_byte_at_sp ( cpu ) << 8;

        cpu->pc = addr;
    } else {
        cpu->pc += 1;
    }
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

    /* FIXME */ abort();
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

    /* FIXME */ abort();
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
                    /* FIXME */ abort();
    cpu->cycles_machine += 6;
                } else {
                    cpu->pc     += 3;
                    /* FIXME */ abort();
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
                    /* FIXME */ abort();
    cpu->cycles_machine += 6;
                } else {
                    cpu->pc     += 3;
                    /* FIXME */ abort();
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
                    /* FIXME */ abort();
    cpu->cycles_machine += 6;
                } else {
                    cpu->pc     += 3;
                    /* FIXME */ abort();
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
                    /* FIXME */ abort();
    cpu->cycles_machine += 6;
                } else {
                    cpu->pc     += 3;
                    /* FIXME */ abort();
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

    /* FIXME */ abort();
    cpu->cycles_machine += 6;
}

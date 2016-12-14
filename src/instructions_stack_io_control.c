#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "types.h"

#include "instructions_stack_io_control.h"

void emulate_PUSH ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xc5: // PUSH B
            cpu->memory[cpu->sp-2] = cpu->c;
            cpu->memory[cpu->sp-1] = cpu->b;
            cpu->sp -= 2;
            break;
        case 0xd5: // PUSH D
            cpu->memory[cpu->sp-2] = cpu->e;
            cpu->memory[cpu->sp-1] = cpu->d;
            cpu->sp -= 2;
            break;
        case 0xe5: // PUSH H
            cpu->memory[cpu->sp-2] = cpu->l;
            cpu->memory[cpu->sp-1] = cpu->h;
            cpu->sp -= 2;
            break;
        case 0xf5: // PUSH PSW
            cpu->memory[cpu->sp-1] = cpu->a;
            cpu->memory[cpu->sp-2] = ( cpu->flags.z  ? 0x40 : 0x0 ) |
                                     ( cpu->flags.s  ? 0x80 : 0x0 ) |
                                     ( cpu->flags.p  ? 0x04 : 0x0 ) |
                                     ( cpu->flags.cy ? 0x01 : 0x0 ) |
                                     ( cpu->flags.ac ? 0x10 : 0x0 ) |
                                     0x02;
            cpu->sp = cpu->sp - 2;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 11;
    cpu->pc     += 1 ;
}

void emulate_POP ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xc1: // POP B
            cpu->c = cpu->memory[ ( cpu->sp + 0 ) & 0xffff ];
            cpu->b = cpu->memory[ ( cpu->sp + 1 ) & 0xffff ];
            cpu->sp += 2;
            break;
        case 0xd1: // POP D
            cpu->e = cpu->memory[ ( cpu->sp + 0 ) & 0xffff ];
            cpu->d = cpu->memory[ ( cpu->sp + 1 ) & 0xffff ];
            cpu->sp += 2;
            break;
        case 0xe1: // POP H
            cpu->l = cpu->memory[ ( cpu->sp + 0 ) & 0xffff ];
            cpu->h = cpu->memory[ ( cpu->sp + 1 ) & 0xffff ];
            cpu->sp += 2;
            break;
        case 0xf1: // POP PSW
            cpu->a = cpu->memory[cpu->sp+1];
            cpu->flags.z  = ((cpu->memory[cpu->sp] & 0x40));
            cpu->flags.s  = ((cpu->memory[cpu->sp] & 0x80));
            cpu->flags.p  = ((cpu->memory[cpu->sp] & 0x04));
            cpu->flags.cy = ((cpu->memory[cpu->sp] & 0x01));
            cpu->flags.ac = ((cpu->memory[cpu->sp] & 0x10));
            cpu->sp += 2;
            cpu->cycles -= 1;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 11;
    cpu->pc     += 1 ;
}

void emulate_XTHL ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xe3: // XTHL
            {
            uint8_t l = cpu->l;
            uint8_t h = cpu->h;
            cpu->l = cpu->memory[cpu->sp + 0];
            cpu->h = cpu->memory[cpu->sp + 1];
            cpu->memory[cpu->sp + 0] = l;
            cpu->memory[cpu->sp + 1] = h;
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 18;
    cpu->pc     += 1 ;
}

void emulate_SPHL ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xf9: // SPHL B
            cpu->sp = cpu->h << 8 | cpu->l;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 5;
    cpu->pc     += 1 ;
}

void emulate_IN ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xdb: // IN
            {
                uint8_t port = opcode[1];
                switch ( port ) {
                    case 0x00:
                        cpu->a = cpu->portin0;
                        break;
                    case 0x01:
                        cpu->a = cpu->portin1;
                        break;
                    case 0x02:
                        cpu->a = cpu->portin2;
                        break;
                    case 0x03: // Shiftregister port
                        {
                        uint16_t v = (cpu->shift1<<8) | cpu->shift0;
                        cpu->a = ((v >> (8 - cpu->shift_offset)) & 0xff);
                        }
                        break;
                    default:
                        assert( 0 && "Invalid Port\n" );
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10;
    cpu->pc     += 2 ;
}

void emulate_OUT ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
        case 0xd3: // OUT
            {
                uint8_t port = opcode[1];
                switch ( port ) {
                    case 0x02: // Sets the offset size for the shift register
                        cpu->shift_offset = cpu->a & 0x7;
                        break;
                    case 0x03: // sound port
                        // Do nothing
                        break;
                    case 0x04: // Throws data at the shiftregister
                        cpu->shift0 = cpu->shift1;
                        cpu->shift1 = cpu->a;
                        break;
                    case 0x05: // sound port
                        // Do nothing
                        break;
                    case 0x06: // Watchchdog port
                        // Do nothing
                        break;
                    default:
                        assert( 0 && "Invalid Port\n" );
                }
            }
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 10;
    cpu->pc     += 2 ;
}

void emulate_EI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
            case 0xfb: // EI
            cpu->enable_interrupts = 1;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_DI ( _cpu_info *cpu ) {
    unsigned char *opcode = &cpu->memory[cpu->pc];

    switch ( *opcode ) {
            case 0xf3: // EI
            cpu->enable_interrupts = 0;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_NOP ( _cpu_info *cpu ) {
    cpu->cycles += 4 ;
    cpu->pc     += 1 ;
}

void emulate_INTERRUPT ( _cpu_info *cpu ) {
    if ( cpu->enable_interrupts ) {
        uint16_t ret           = cpu->pc;
        cpu->memory[cpu->sp-1] = (ret >> 8) & 0xff;
        cpu->memory[cpu->sp-2] = (ret & 0xff);
        cpu->sp                = cpu->sp - 2;

        cpu->pc = cpu->interrupt_addr;
    } else {
        // Do nothing
    }
}

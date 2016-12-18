#include "types.h"
#include "memory.h"
#include "instructions_data_transfer.h"

/*
 * 00000xxx = RLC  xxx
 * 00001xxx = RRC  xxx
 * 00010xxx = RL   xxx
 * 00011xxx = RR   xxx
 * 00100xxx = SLA  xxx
 * 00101xxx = SRA  xxx
 * 00110xxx = SWAP xxx
 * 00111xxx = SRL  xxx
 * 01yyyxxx = BIT  yyy, xxx
 * 10yyyxxx = RES  yyy, xxx
 * 11yyyxxx = SET  yyy, xxx
 */

void emulate_RR ( _cpu_info *cpu, uint8_t target ) {
    uint8_t *a = get_reg_ref(cpu, target);

    uint8_t t    = cpu->flags.c;
    cpu->flags.c =  *a     &  0x01;
    *a           = (*a     >> 1)  | ( t << 7 );
    cpu->flags.z =  *a     == 0   ;

    /*switch ( *opcode ) {*/
        /*case 0x18:*/
        /*{*/
            /*uint8_t t    = cpu->flags.c;*/
            /*cpu->flags.c = cpu->b & 0x01;*/
            /*cpu->b       = (cpu->b >> 1) | ( t << 7 );*/
            /*cpu->flags.z = cpu->b == 0;*/
        /*}*/
            /*break;*/
        /*case 0x19:*/
        /*{*/
            /*uint8_t t    = cpu->flags.c;*/
            /*cpu->flags.c = cpu->c & 0x01;*/
            /*cpu->c       = (cpu->c >> 1) | ( t << 7 );*/
            /*cpu->flags.z = cpu->c == 0;*/
        /*}*/
            /*break;*/
        /*case 0x1a:*/
        /*{*/
            /*uint8_t t    = cpu->flags.c;*/
            /*cpu->flags.c = cpu->d & 0x01;*/
            /*cpu->d       = (cpu->d >> 1) | ( t << 7 );*/
            /*cpu->flags.z = cpu->d == 0;*/
        /*}*/
            /*break;*/
        /*case 0x1b:*/
        /*{*/
            /*uint8_t t    = cpu->flags.c;*/
            /*cpu->flags.c = cpu->e & 0x01;*/
            /*cpu->e       = (cpu->e >> 1) | ( t << 7 );*/
            /*cpu->flags.z = cpu->e == 0;*/
        /*}*/
            /*break;*/
        /*case 0x1c:*/
        /*{*/
            /*uint8_t t    = cpu->flags.c;*/
            /*cpu->flags.c = cpu->h & 0x01;*/
            /*cpu->h       = (cpu->h >> 1) | ( t << 7 );*/
            /*cpu->flags.z = cpu->h == 0;*/
        /*}*/
            /*break;*/
        /*case 0x1d:*/
        /*{*/
            /*uint8_t t    = cpu->flags.c;*/
            /*cpu->flags.c = cpu->l & 0x01;*/
            /*cpu->l       = (cpu->l >> 1) | ( t << 7 );*/
            /*cpu->flags.z = cpu->l == 0;*/
        /*}*/
            /*break;*/
        /*case 0x1e:*/
        /*{*/
            /*uint8_t     t = cpu->flags.c != 0;*/
            /*uint16_t addr = (cpu->h << 8) | cpu->l;*/
            /*uint8_t  answ = ((read_byte ( cpu, addr ) >> 1) | ( t << 7 ));*/
            /*cpu->flags.c  = read_byte ( cpu, addr ) & 0x01;*/
            /*write_byte( cpu, addr + 0, answ >> 0);*/
            /*write_byte( cpu, addr + 1, answ >> 8);*/
            /*cpu->flags.z = answ == 0;*/
        /*}*/
            /*break;*/
        /*case 0x1f:*/
        /*{*/
            /*uint8_t t    = cpu->flags.c;*/
            /*cpu->flags.c = cpu->a & 0x01;*/
            /*cpu->a       = (cpu->a >> 1) | ( t << 7 );*/
            /*cpu->flags.z = cpu->a == 0;*/
        /*}*/
            /*break;*/
        /*default:*/
            /*assert( 0 && "Code should not get here\n" );*/
    /*}*/

    cpu->flags.h = 0;
    cpu->flags.n = 0;

    cpu->pc     += 2;
    cpu->cycles += 8;
}

void emulate_RRC ( _cpu_info *cpu, uint8_t target ) {
    unsigned char *opcode = &cpu->memory[cpu->pc+1];
    uint8_t answer = 0;
    uint8_t      t = 0;
    uint16_t  addr = 0;

    assert( 0 && "Function RL   not implemented");

    switch ( *opcode ) {
        case 0x0e: // RRC
            addr = ( cpu->h << 8 ) | cpu->l;
            t = read_byte( cpu, addr );
            answer = ( t >> 1 ) | (( t & 0x1 ) << 7 );
            write_byte(cpu, addr, answer);
            cpu->flags.c  = t & 0x1;
            break;
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z = answer == 0;
    cpu->flags.n = 0;
    cpu->flags.h = 0;

    cpu->cycles += 4 ;
    cpu->pc     += 2 ;
}

void emulate_RLC ( _cpu_info *cpu, uint8_t target ) {
    unsigned char *opcode = &cpu->memory[cpu->pc+1];
    uint8_t answer = 0;
    uint8_t      t = 0;
    uint16_t  addr = 0;

    assert( 0 && "Function RL   not implemented");

    switch ( *opcode ) {
        case 0x06: // RLC
            addr = ( cpu->h << 8 ) | cpu->l;
            t = read_byte( cpu, addr );
            cpu->flags.c  = 0x80 == (t & 0x80);
            answer = ( t << 1 ) | cpu->flags.c;
            write_byte(cpu, addr, answer) ;
            break;
        /*case 0x07: // RLC*/
            /*t = cpu->a;*/
            /*cpu->flags.c  = 0x80 == (t & 0x80);*/
            /*cpu->a = ( t << 1 ) | cpu->flags.c ;*/
            /*break;*/
        default:
            assert( 0 && "Code should not get here\n" );
    }

    cpu->flags.z = answer == 0;
    cpu->flags.n = 0;
    cpu->flags.h = 0;

    cpu->cycles += 4 ;
    cpu->pc     += 2 ;
}

void emulate_RL    ( _cpu_info *cpu, uint8_t target ) { assert( 0 && "Function RL   not implemented");}
void emulate_SLA   ( _cpu_info *cpu, uint8_t target ) { assert( 0 && "Function SLA  not implemented");}
void emulate_SRA   ( _cpu_info *cpu, uint8_t target ) { assert( 0 && "Function SRA  not implemented");}
void emulate_SWAP  ( _cpu_info *cpu, uint8_t target ) { assert( 0 && "Function SWAP not implemented");}
void emulate_SRL   ( _cpu_info *cpu, uint8_t target ) { assert( 0 && "Function SRL  not implemented");}

void decode_0xcb( _cpu_info *cpu ) {
    uint8_t target;
    uint8_t opcode;

    void (*f1[])(_cpu_info *, uint8_t) =
            {
                emulate_RLC ,
                emulate_RRC ,
                emulate_RL  ,
                emulate_RR  ,
                emulate_SLA ,
                emulate_SRA ,
                emulate_SWAP,
                emulate_SRL
            };
    /*void (*f2[])(_cpu_int *, uint8_t, uint8_t) = {BIT, RES, SET};*/

    target = cpu->memory[cpu->pc + 1] &  0x0f;
    opcode = cpu->memory[cpu->pc + 1] >> 3   ;

    if ( !(opcode & 0xf0) ) {
        f1[opcode](cpu, target);
    }

    /*switch ( opcode ) {*/
        /*case 0x11:*/
            /*{*/
                /*uint8_t t;*/
                /*t = cpu->c;*/
                /*cpu->flags.c  = 0x80 == (t & 0x80);*/
                /*cpu->c = ( t << 1 ) | cpu->flags.c ;*/
                /*cpu->cycles += 8;*/
                /*cpu->pc     += 2;*/
            /*}*/
            /*break;*/
        /*case 0x18:*/
        /*case 0x19:*/
        /*case 0x1a:*/
        /*case 0x1b:*/
        /*case 0x1c:*/
        /*case 0x1d:*/
        /*case 0x1e:*/
        /*case 0x1f:*/
            /*emulate_RR ( cpu );*/
            /*break;*/
        /*case 0x06:*/
            /*emulate_RLC ( cpu );*/
            /*break;*/
        /*case 0x16:*/
            /*emulate_RAL ( cpu );*/
            /*break;*/
        /*case 0x0e:*/
            /*emulate_RRC ( cpu );*/
            /*break;*/
        /*case 0x38:*/
            /*cpu->flags.c = ( cpu->b & 0x01 );*/
            /*cpu->b       = cpu->b >> 1;*/
            /*cpu->flags.z = cpu->b == 0;*/
            /*cpu->flags.h = 0;*/
            /*cpu->flags.n = 0;*/
            /*cpu->pc     += 2;*/
            /*cpu->cycles += 8;*/
            /*break;*/
        /*default:*/
            /*printf(" %2x %2x is not implemented\n", opcode[0], opcode[1]);*/
            /*exit(-1);*/
    /*}*/
}

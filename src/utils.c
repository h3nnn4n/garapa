#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "types.h"

off_t fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    fprintf(stderr, "Cannot determine size of %s: %s\n",
            filename, strerror(errno));

    return -1;
}

unsigned short int disassembler( unsigned char *buffer, unsigned int pc ) {
    unsigned short int op_size = 1;

    switch ( buffer[pc] ) {
        case 0x00:
            printf("%08X : %X \t NOP\n", pc, buffer[pc]);
            break;
        case 0x01:
            printf("%08X : %X \t LXI %X %x\n", pc, buffer[pc], buffer[pc+1], buffer[pc+2]);
            op_size = 3;
            break;
        case 0x02:
            printf("%08X : %X \t STAX B\n", pc, buffer[pc]);
            break;
        case 0x03:
            printf("%08X : %X \t INX B\n", pc, buffer[pc]);
            break;
        case 0x04:
            printf("%08X : %X \t INR B\n", pc, buffer[pc]);
            break;
        case 0x05:
            printf("%08X : %X \t DCR B\n", pc, buffer[pc]);
            break;
        case 0x06:
            printf("%08X : %X \t MVI B, %X\n", pc, buffer[pc], buffer[pc+1]);
            op_size = 2;
            break;
        case 0x07:
            printf("%08X : %X \t RLC\n", pc, buffer[pc]);
            break;
        case 0x08:
            printf("%08X : %X \t -\n", pc, buffer[pc]);
            break;
        case 0x09:
            printf("%08X : %X \t DAD B\n", pc, buffer[pc]);
            break;

/////////////////////

        case 0x0a:
            printf("%08X : %X \t LDAX B\n", pc, buffer[pc]);
            break;
        case 0x0b:
            printf("%08X : %X \t DCX B\n", pc, buffer[pc]);
            break;
        case 0x0c:
            printf("%08X : %X \t INR C\n", pc, buffer[pc]);
            break;
        case 0x0d:
            printf("%08X : %X \t DCR C\n", pc, buffer[pc]);
            break;
        case 0x0e:
            printf("%08X : %X \t MVI C, %X\n", pc, buffer[pc], buffer[pc+1]);
            break;
        case 0x0f:
            printf("%08X : %X \t RRC B\n", pc, buffer[pc]);
            break;

/////////////////////

        case 0x10:
            printf("%08X : %X \t -\n", pc, buffer[pc]);
            break;
        case 0x11:
            printf("%08X : %X \t LXI D,  %X %X\n", pc, buffer[pc], buffer[pc+1], buffer[pc+2]);
            op_size = 3;
            break;
        case 0x12:
            printf("%08X : %X \t STAX D\n", pc, buffer[pc]);
            break;
        case 0x13:
            printf("%08X : %X \t INX D\n", pc, buffer[pc]);
            break;
        case 0x14:
            printf("%08X : %X \t INR D\n", pc, buffer[pc]);
            break;
        case 0x15:
            printf("%08X : %X \t DCR D\n", pc, buffer[pc]);
            break;
        case 0x16:
            printf("%08X : %X \t MVI D, %X\n", pc, buffer[pc], buffer[pc+1]);
            op_size = 2;
            break;
        case 0x17:
            printf("%08X : %X \t RAL\n", pc, buffer[pc]);
            break;
        case 0x18:
            printf("%08X : %X \t -\n", pc, buffer[pc]);
            break;
        case 0x19:
            printf("%08X : %X \t DAD D\n", pc, buffer[pc]);
            break;

/////////////////////

        case 0x1a:
            printf("%08X : %X \t LDAX D\n", pc, buffer[pc]);
            break;
        case 0x1b:
            printf("%08X : %X \t DCX D\n", pc, buffer[pc]);
            break;
        case 0x1c:
            printf("%08X : %X \t INR D\n", pc, buffer[pc]);
            break;
        case 0x1d:
            printf("%08X : %X \t DCR D\n", pc, buffer[pc]);
            break;
        case 0x1e:
            printf("%08X : %X \t MVI D, %X\n", pc, buffer[pc], buffer[pc+1]);
            break;
        case 0x1f:
            printf("%08X : %X \t RAR\n", pc, buffer[pc]);
            break;

/////////////////////

        case 0x20:
            printf("%08X : %X \t RIM\n", pc, buffer[pc]);
            break;
        case 0x21:
            printf("%08X : %X \t LXI H,  %X %X\n", pc, buffer[pc], buffer[pc+1], buffer[pc+2]);
            op_size = 3;
            break;
        case 0x22:
            printf("%08X : %X \t SHLD %X %X\n", pc, buffer[pc], buffer[pc+1], buffer[pc+2]);
            op_size = 3;
            break;
        case 0x23:
            printf("%08X : %X \t INX H\n", pc, buffer[pc]);
            break;
        case 0x24:
            printf("%08X : %X \t INR H\n", pc, buffer[pc]);
            break;
        case 0x25:
            printf("%08X : %X \t DCR H\n", pc, buffer[pc]);
            break;
        case 0x26:
            printf("%08X : %X \t MVI H, %X\n", pc, buffer[pc], buffer[pc+1]);
            op_size = 2;
            break;
        case 0x27:
            printf("%08X : %X \t DAA\n", pc, buffer[pc]);
            break;
        case 0x28:
            printf("%08X : %X \t -\n", pc, buffer[pc]);
            break;
        case 0x29:
            printf("%08X : %X \t DAD H\n", pc, buffer[pc]);
            break;

/////////////////////

        case 0x2a:
            printf("%08X : %X \t LHLD %X %X\n", pc, buffer[pc], buffer[pc+1], buffer[pc+2]);
            op_size = 3;
            break;
        case 0x2b:
            printf("%08X : %X \t DCX H\n", pc, buffer[pc]);
            break;
        case 0x2c:
            printf("%08X : %X \t INR L\n", pc, buffer[pc]);
            break;
        case 0x2d:
            printf("%08X : %X \t DCR L\n", pc, buffer[pc]);
            break;
        case 0x2e:
            printf("%08X : %X \t MVI L, %X\n", pc, buffer[pc], buffer[pc+1]);
            break;
        case 0x2f:
                printf("%08X : %X \t CMA\n", pc, buffer[pc]);
            break;

/////////////////////

        case 0x30:
            printf("%08X : %X \t SIM\n", pc, buffer[pc]);
            break;
        case 0x31:
            printf("%08X : %X \t LXI SP,  %X %X\n", pc, buffer[pc], buffer[pc+1], buffer[pc+2]);
            op_size = 3;
            break;
        case 0x32:
            printf("%08X : %X \t STA %X %X\n", pc, buffer[pc], buffer[pc+1], buffer[pc+2]);
            op_size = 3;
            break;
        case 0x33:
            printf("%08X : %X \t INX SP\n", pc, buffer[pc]);
            break;
        case 0x34:
            printf("%08X : %X \t INR M\n", pc, buffer[pc]);
            break;
        case 0x35:
            printf("%08X : %X \t DCR M\n", pc, buffer[pc]);
            break;
        case 0x36:
            printf("%08X : %X \t MVI M, %X\n", pc, buffer[pc], buffer[pc+1]);
            op_size = 2;
            break;
        case 0x37:
            printf("%08X : %X \t STC\n", pc, buffer[pc]);
            break;
        case 0x38:
            printf("%08X : %X \t -\n", pc, buffer[pc]);
            break;
        case 0x39:
            printf("%08X : %X \t DAD SP\n", pc, buffer[pc]);
            break;

/////////////////////

        case 0x3a:
            printf("%08X : %X \t LDA %X %X\n", pc, buffer[pc], buffer[pc+1], buffer[pc+2]);
            op_size = 3;
            break;
        case 0x3b:
            printf("%08X : %X \t DCX SP\n", pc, buffer[pc]);
            break;
        case 0x3c:
            printf("%08X : %X \t INR A\n", pc, buffer[pc]);
            break;
        case 0x3d:
            printf("%08X : %X \t DCR A\n", pc, buffer[pc]);
            break;
        case 0x3e:
            printf("%08X : %X \t MVI A, %X\n", pc, buffer[pc], buffer[pc+1]);
            break;
        case 0x3f:
            printf("%08X : %X \t STC\n", pc, buffer[pc]);
            break;

/////////////////////

        case 0x40: printf( "%08X : %X \t MOV B, B\n", pc, buffer[pc] ) ; break ;
        case 0x41: printf( "%08X : %X \t MOV B, C\n", pc, buffer[pc] ) ; break ;
        case 0x42: printf( "%08X : %X \t MOV B, D\n", pc, buffer[pc] ) ; break ;
        case 0x43: printf( "%08X : %X \t MOV B, E\n", pc, buffer[pc] ) ; break ;
        case 0x44: printf( "%08X : %X \t MOV B, H\n", pc, buffer[pc] ) ; break ;
        case 0x45: printf( "%08X : %X \t MOV B, L\n", pc, buffer[pc] ) ; break ;
        case 0x46: printf( "%08X : %X \t MOV B, M\n", pc, buffer[pc] ) ; break ;
        case 0x47: printf( "%08X : %X \t MOV B, A\n", pc, buffer[pc] ) ; break ;
        case 0x48: printf( "%08X : %X \t MOV C, B\n", pc, buffer[pc] ) ; break ;
        case 0x49: printf( "%08X : %X \t MOV C, C\n", pc, buffer[pc] ) ; break ;
        case 0x4a: printf( "%08X : %X \t MOV C, D\n", pc, buffer[pc] ) ; break ;
        case 0x4b: printf( "%08X : %X \t MOV C, E\n", pc, buffer[pc] ) ; break ;
        case 0x4c: printf( "%08X : %X \t MOV C, H\n", pc, buffer[pc] ) ; break ;
        case 0x4d: printf( "%08X : %X \t MOV C, L\n", pc, buffer[pc] ) ; break ;
        case 0x4e: printf( "%08X : %X \t MOV C, M\n", pc, buffer[pc] ) ; break ;
        case 0x4f: printf( "%08X : %X \t MOV C, A\n", pc, buffer[pc] ) ; break ;

        case 0x50: printf( "%08X : %X \t MOV D, B\n", pc, buffer[pc] ) ; break ;
        case 0x51: printf( "%08X : %X \t MOV D, C\n", pc, buffer[pc] ) ; break ;
        case 0x52: printf( "%08X : %X \t MOV D, D\n", pc, buffer[pc] ) ; break ;
        case 0x53: printf( "%08X : %X \t MOV D, E\n", pc, buffer[pc] ) ; break ;
        case 0x54: printf( "%08X : %X \t MOV D, H\n", pc, buffer[pc] ) ; break ;
        case 0x55: printf( "%08X : %X \t MOV D, L\n", pc, buffer[pc] ) ; break ;
        case 0x56: printf( "%08X : %X \t MOV D, M\n", pc, buffer[pc] ) ; break ;
        case 0x57: printf( "%08X : %X \t MOV D, A\n", pc, buffer[pc] ) ; break ;
        case 0x58: printf( "%08X : %X \t MOV E, B\n", pc, buffer[pc] ) ; break ;
        case 0x59: printf( "%08X : %X \t MOV E, C\n", pc, buffer[pc] ) ; break ;
        case 0x5a: printf( "%08X : %X \t MOV E, D\n", pc, buffer[pc] ) ; break ;
        case 0x5b: printf( "%08X : %X \t MOV E, E\n", pc, buffer[pc] ) ; break ;
        case 0x5c: printf( "%08X : %X \t MOV E, H\n", pc, buffer[pc] ) ; break ;
        case 0x5d: printf( "%08X : %X \t MOV E, L\n", pc, buffer[pc] ) ; break ;
        case 0x5e: printf( "%08X : %X \t MOV E, M\n", pc, buffer[pc] ) ; break ;
        case 0x5f: printf( "%08X : %X \t MOV E, A\n", pc, buffer[pc] ) ; break ;

        case 0x60: printf( "%08X : %X \t MOV H, B\n", pc, buffer[pc] ) ; break ;
        case 0x61: printf( "%08X : %X \t MOV H, C\n", pc, buffer[pc] ) ; break ;
        case 0x62: printf( "%08X : %X \t MOV H, D\n", pc, buffer[pc] ) ; break ;
        case 0x63: printf( "%08X : %X \t MOV H, E\n", pc, buffer[pc] ) ; break ;
        case 0x64: printf( "%08X : %X \t MOV H, H\n", pc, buffer[pc] ) ; break ;
        case 0x65: printf( "%08X : %X \t MOV H, L\n", pc, buffer[pc] ) ; break ;
        case 0x66: printf( "%08X : %X \t MOV H, M\n", pc, buffer[pc] ) ; break ;
        case 0x67: printf( "%08X : %X \t MOV H, A\n", pc, buffer[pc] ) ; break ;
        case 0x68: printf( "%08X : %X \t MOV L, B\n", pc, buffer[pc] ) ; break ;
        case 0x69: printf( "%08X : %X \t MOV L, C\n", pc, buffer[pc] ) ; break ;
        case 0x6a: printf( "%08X : %X \t MOV L, D\n", pc, buffer[pc] ) ; break ;
        case 0x6b: printf( "%08X : %X \t MOV L, E\n", pc, buffer[pc] ) ; break ;
        case 0x6c: printf( "%08X : %X \t MOV L, H\n", pc, buffer[pc] ) ; break ;
        case 0x6d: printf( "%08X : %X \t MOV L, L\n", pc, buffer[pc] ) ; break ;
        case 0x6e: printf( "%08X : %X \t MOV L, M\n", pc, buffer[pc] ) ; break ;
        case 0x6f: printf( "%08X : %X \t MOV L, A\n", pc, buffer[pc] ) ; break ;

        case 0x70: printf( "%08X : %X \t MOV M, B\n", pc, buffer[pc] ) ; break ;
        case 0x71: printf( "%08X : %X \t MOV M, C\n", pc, buffer[pc] ) ; break ;
        case 0x72: printf( "%08X : %X \t MOV M, D\n", pc, buffer[pc] ) ; break ;
        case 0x73: printf( "%08X : %X \t MOV M, E\n", pc, buffer[pc] ) ; break ;
        case 0x74: printf( "%08X : %X \t MOV M, H\n", pc, buffer[pc] ) ; break ;
        case 0x75: printf( "%08X : %X \t MOV M, L\n", pc, buffer[pc] ) ; break ;
        case 0x76: printf( "%08X : %X \t HTL     \n", pc, buffer[pc] ) ; break ;
        case 0x77: printf( "%08X : %X \t MOV M, A\n", pc, buffer[pc] ) ; break ;
        case 0x78: printf( "%08X : %X \t MOV A, B\n", pc, buffer[pc] ) ; break ;
        case 0x79: printf( "%08X : %X \t MOV A, C\n", pc, buffer[pc] ) ; break ;
        case 0x7a: printf( "%08X : %X \t MOV A, D\n", pc, buffer[pc] ) ; break ;
        case 0x7b: printf( "%08X : %X \t MOV A, E\n", pc, buffer[pc] ) ; break ;
        case 0x7c: printf( "%08X : %X \t MOV A, H\n", pc, buffer[pc] ) ; break ;
        case 0x7d: printf( "%08X : %X \t MOV A, L\n", pc, buffer[pc] ) ; break ;
        case 0x7e: printf( "%08X : %X \t MOV A, M\n", pc, buffer[pc] ) ; break ;
        case 0x7f: printf( "%08X : %X \t MOV A, A\n", pc, buffer[pc] ) ; break ;


/////////////////////




        case 0xc3:
            printf("%08X : %X \t JMP %X %X\n", pc, buffer[pc], buffer[pc+1], buffer[pc+2]);
            op_size = 3;
            break;

        case 0xc5:
            printf("%08X : %X \t PUSH B\n", pc, buffer[pc]);
            op_size = 1;
            break;

        case 0xd4:
            printf("%08X : %X \t CNC %X %X\n", pc, buffer[pc], buffer[pc+1], buffer[pc+2]);
            op_size = 3;
            break;

        case 0xd5:
            printf("%08X : %X \t PUSH D\n", pc, buffer[pc]);
            op_size = 1;
            break;

        case 0xe5:
            printf("%08X : %X \t PUSH H\n", pc, buffer[pc]);
            op_size = 1;
            break;

        case 0xf5:
            printf("%08X : %X \t PUSH PSW\n", pc, buffer[pc]);
            break;

        default:
            printf(" %2X is not implemented\n", buffer[pc]);
            break;
    }

    return op_size;
}

void init_cpu( _cpu_info *cpu ) {
    cpu->memory = malloc ( 64 * 1024 ) ; // Allocs 64Kb of ram
    cpu->pc     = 0;
    cpu->a      = 0;
    cpu->b      = 0;
    cpu->c      = 0;
    cpu->d      = 0;
    cpu->e      = 0;
    cpu->f      = 0;
}

void unimplemented_opcode( _cpu_info *cpu, unsigned char *buffer ) {
    fprintf(stderr, "Unimplemented OP code at %08x:%x \n", cpu->pc, buffer[cpu->pc]);
    exit(-1);
}

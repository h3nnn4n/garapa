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
            printf("%08X : %X \t DRC B\n", pc, buffer[pc]);
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



        case 0x3e:
            printf("%08X : %X \t MVI A, %X\n", pc, buffer[pc], buffer[pc+1]);
            op_size = 2;
            break;

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

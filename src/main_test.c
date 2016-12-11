#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>

#include "utils.h"
#include "types.h"
#include "emulator.h"
#include "disassembler.h"

void tester( const char *name ) {
    FILE *f = NULL;
    _cpu_info cpu;

    init_cpu(&cpu);
    off_t buffer_size = -1;
    f = fopen(name, "rb");
    buffer_size = fsize( name );

    uint16_t offset = 0x100;

    if ( fread(cpu.memory + offset, buffer_size, 1, f) != 1 ) {
        printf("Something went weird while reding into buffer\n");
    }

    fclose(f);

    printf("Opening: %s\nFile contains: %lu bytes\n", name, buffer_size);
    printf("Test starting\n\n");

    cpu.pc = 0x0100;
    cpu.memory[0x05] = 0xC9;

    while ( 1 ) {
        emulator ( &cpu );

        if (cpu.pc == 0x0000) {
            printf("\n\nExecution endend\n");
            break;
        }

        if (cpu.memory[cpu.pc] == 0x76) {
            printf("HLT at %04X\n", cpu.pc);
            exit(1);
        }

        if (cpu.pc == 0x0005) {
            if (cpu.c == 9) {
                for (int i = cpu.d << 8 | cpu.e; cpu.memory[i] != '$'; ++i)
                    printf("%c", cpu.memory[i]);
            }
            if (cpu.c == 2) putchar((char)cpu.e);
        }
    }

    printf("Test finished\n --------------------\n");
}

int main( ) {
    tester( "roms/8080PRE.COM" );
    tester( "roms/8080EX1.COM" );
    tester( "roms/CPUTEST.COM" );
    tester( "roms/TEST.COM" );
    return 0;
}

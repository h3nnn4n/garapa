#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>

#include "utils.h"
#include "types.h"
#include "emulator.h"
#include "graphics.h"
#include "disassembler.h"

int main(int argc, char *argv[]) {
    FILE *f = NULL;
    _cpu_info cpu;

    if ( argc == 1 ) {
        printf("Usage: %s file\n", argv[0]);
        return EXIT_FAILURE;
    } else {
        init_cpu(&cpu);
        off_t buffer_size = -1;
        f = fopen(argv[1], "rb");
        buffer_size = fsize( argv[1] );

        uint16_t offset = 0x100;

        if ( fread(cpu.memory + offset, buffer_size, 1, f) != 1 ) {
            printf("Something went weird while reding into buffer\n");
        }

        fclose(f);

        /*printf("Opening: %s\nFile contains: %lu bytes\n", argv[1], buffer_size);*/
    }

    sdl_init();
    atexit(SDL_Quit);

    cpu.sp = 0x0;
    cpu.pc = 0x100;

    cpu.memory[0x05] = 0xC9;

    int success = 0;

    while ( 1 ) {
        /*cpu.pc += disassembler(cpu.memory, cpu.pc);*/

        if (cpu.pc == 0x0005) {
            if (cpu.c == 9) {
                int i;
                for (i = cpu.d << 8 | cpu.e; cpu.memory[i] != '$'; ++i)
                    printf("%c", cpu.memory[i]);
                success = 1;
            }
            if (cpu.c == 2)
                putchar((char)cpu.e);
        }

        emulator ( &cpu );

        if (cpu.memory[cpu.pc] == 0x76) {
            printf("HLT at %04X\n", cpu.pc);
            exit(1);
        }

        if ( cpu.pc == 0x0000 && !success ) {
            printf("Got to 0x0000 without the success flag. RIP\n");
            exit(-1);
        }

        if (cpu.pc == 0) {
            printf("\n\n\n");
            break;;
        }
    }

    return 0;
}

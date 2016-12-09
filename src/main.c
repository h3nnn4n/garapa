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

        uint16_t offset = 0x000;

        if ( fread(cpu.memory + offset, buffer_size, 1, f) != 1 ) {
            printf("Something went weird while reding into buffer\n");
        }

        fclose(f);

        printf("Opening: %s\nFile contains: %lu bytes\n", argv[1], buffer_size);
    }

    sdl_init();
    atexit(SDL_Quit);

    /*int ff = 0;*/

    while ( 1 ) {
        /*cpu.pc += disassembler(cpu.memory, cpu.pc);*/

        /*if ( cpu.memory[cpu.pc+0] == 0xcd &&*/
             /*cpu.memory[cpu.pc+1] == 0x50 &&*/
             /*cpu.memory[cpu.pc+2] == 0x05 ){*/
            /*ff = 1;*/
            /*printf("%04x -> sp = %04x\n", cpu.pc, cpu.sp);// %4x: %2x   \t %4x: %2x\n", cpu.pc, 0x2078-1, cpu.memory[0x2078-1], 0x2078-2, cpu.memory[0x2078-2]);*/
        /*}*/

        emulator ( &cpu );

        /*if ( cpu.instructions_executed > 2030000 ) {*/
            /*disassembler ( cpu.memory, cpu.pc );*/
            /*print_registers(&cpu);*/
        /*}*/

        /*if ( cpu.instructions_executed > 2040000 ) {*/
            /*break;*/
        /*}*/


        /*if ( ff ) {*/
            /*printf("%04x -> sp = %04x\n", cpu.pc, cpu.sp);// %4x: %2x   \t %4x: %2x\n", cpu.pc, 0x2078-1, cpu.memory[0x2078-1], 0x2078-2, cpu.memory[0x2078-2]);*/
            /*for (int i = 0; i < 10; ++i) {*/
                /*printf("        %4x: %2x\n", 0x23e8 + (i-4), cpu.memory[0x23e8 + (i-4)]);*/
            /*}*/
            /*break;*/
        /*}*/

        /*if ( cpu.instructions_executed % 100000 == 0 ) {*/
            /*printf(" -- %8d\n", cpu.instructions_executed);*/
        /*}*/


        if ( cpu.interrupt_addr == 0x10 && cpu.cycles > 28500 ) {
            cpu.cycles -= 28500;
            cpu.interrupt_addr = 0x08;
            emulate_INTERRUPT( &cpu );
            update_screen ( &cpu );
        } else if ( cpu.interrupt_addr == 0x08 && cpu.cycles > 4850 ) {
            cpu.cycles -= 4850;
            cpu.interrupt_addr = 0x10;
            emulate_INTERRUPT( &cpu );
            update_input ( &cpu );
            /*SDL_Delay(16);*/
        }
    }

    return 0;
}

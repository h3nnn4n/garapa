#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "rev.h"
#include "other_window.h"
#include "types.h"
#include "opt_to_str.h"

static _rev_data rev_data;

static uint8_t memory_1[0x10000];
static uint8_t memory_2[0x10000];

void reset_code_and_data(){
    for (int i = 0; i < MEM_SIZE; ++i) {
        for (int j = 0; j < TEXT_SIZE; ++j) {
            rev_data.code_and_data[i][j] = '\0';
            rev_data.is_executed[i] = 0;
        }
    }
}

void write_executed_code (_cpu_info *cpu) {
    if ( rev_data.is_executed[cpu->pc] == 0 ) {
        rev_data.is_executed[cpu->pc] = 1;
        get_opt_str(cpu, rev_data.code_and_data[cpu->pc]);
        /*printf("wrote: %s\n", rev_data.code_and_data[cpu->pc]);*/
    } else if  ( rev_data.is_executed[cpu->pc] != 1 ) {
        abort();
    }
}

void dump_to_file() {
    static int number = 0;
    char name[256];

    sprintf(name, "periodic_dump_%08d.txt", number++);

    FILE *f = fopen(name, "wt");

    for (int i = 0; i < MEM_SIZE; ++i) {
        /*if ( rev_data.is_executed == 1 ) {*/
            fprintf(f, "%s\n", rev_data.code_and_data[i]);
        /*}*/
    }

    fclose(f);
}

void mem_find_reset() {
    for (int i = 0; i < 0x10000; ++i) {
        memory_1[i] = 0;
        memory_2[i] = 0;
    }
}

void mem_find_start() {
    static int started = 0;

    printf("mem_find_start was running? %s\n", started == 0 ? "no" : "yes");

    if ( started == 0 ) {
        started ++;

        _cpu_info* cpu = get_cpu_pointer();
        for (int i = 0; i < 0x10000; ++i) {
            memory_1[i] = cpu->mem_controller.memory[i];
        }
    }
}

void mem_find_mark_diff() {
    printf("mem_find_find\n");
    _cpu_info* cpu = get_cpu_pointer();
    for (int i = 0; i < 0x10000; ++i) {
        if ( memory_1[i] == cpu->mem_controller.memory[i] ) {
            memory_2[i] = 1;
        } else {
            memory_1[i] = cpu->mem_controller.memory[i];
        }
    }

    mem_find_dump();
}

void mem_find_mark_equal() {
    printf("mem_find_find\n");
    _cpu_info* cpu = get_cpu_pointer();
    for (int i = 0; i < 0x10000; ++i) {
        if ( memory_1[i] != cpu->mem_controller.memory[i] ) {
            memory_2[i] = 1;
        } else {
            memory_1[i] = cpu->mem_controller.memory[i];
        }
    }

    mem_find_dump();
}

void mem_find_dump() {
    static int number = 0;
    char name[256];

    sprintf(name, "mem_dump_%08d.txt", number++);

    printf("mem_find_dump to: %s\n", name);

    FILE *f = fopen(name, "wt");

    for (int i = 0; i < 0x10000; ++i) {
        if ( memory_2[i] == 0 ) {
            fprintf(f, "%4x: %2x\n", i, memory_1[i]);
        }
    }

    fclose(f);
}

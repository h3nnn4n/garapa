#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rev.h"
#include "types.h"
#include "opt_to_str.h"

static _rev_data rev_data;

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

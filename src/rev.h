#ifndef REV_H
#define REV_H

#include "types.h"

#define MEM_SIZE  0xffff
#define TEXT_SIZE 1024

typedef struct {
    char code_and_data[MEM_SIZE][TEXT_SIZE];
    int is_executed[MEM_SIZE];
} _rev_data;

void reset_code_and_data();
void write_executed_code (_cpu_info *cpu);
void dump_to_file();

void mem_find_reset() ;
void mem_find_start() ;
void mem_find_mark_diff() ;
void mem_find_mark_equal() ;
void mem_find_dump() ;

#endif /* REV_H */

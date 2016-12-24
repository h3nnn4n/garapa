#ifndef UTILS_H
#define UTILS_H

#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "types.h"

unsigned short int disassembler ( unsigned char *buffer   , unsigned int pc     ) ;
struct timespec            diff ( struct timespec start   , struct timespec end ) ;
off_t                     fsize ( const char    *filename                       ) ;
void            print_registers ( _cpu_info     *cpu                            ) ;
void                   init_cpu ( _cpu_info     *cpu                            ) ;
void       unimplemented_opcode ( _cpu_info     *cpu                            ) ;
void                   load_rom ( _cpu_info     *cpu      , const char* fname, uint16_t offset ) ;

#endif /* UTILS_H */

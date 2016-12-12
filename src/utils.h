#ifndef UTILS_H
#define UTILS_H

#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "types.h"

unsigned short int disassembler ( unsigned char *buffer, unsigned int pc     ) ;
off_t              fsize        ( const char    *filename                    ) ;
void               init_cpu     ( _cpu_info     *cpu                         ) ;
void       unimplemented_opcode ( _cpu_info     *cpu                         ) ;
int                  parity_bit ( int b                                      ) ;
struct timespec             diff( struct timespec start, struct timespec end ) ;

#endif /* UTILS_H */

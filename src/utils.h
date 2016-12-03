#ifndef UTILS_H
#define UTILS_H

#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "types.h"

unsigned short int disassembler ( unsigned char *buffer, unsigned int pc ) ;
off_t              fsize        ( const char    *filename                ) ;
void               init_cpu     ( _cpu_info     *cpu                     ) ;
void       unimplemented_opcode ( _cpu_info *cpu, unsigned char *buffer  ) ;

#endif /* UTILS_H */

/******************************************************************************
 * Copyright (C) 2016  Renan S. Silva                                         *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgment in the product documentation would be   *
 *    appreciated but is not required.                                        *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 ******************************************************************************/
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

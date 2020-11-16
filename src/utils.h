/******************************************************************************
 * Copyright (C) 2016-2018  Renan S. Silva                                    *
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
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "types.h"

struct timespec diff(struct timespec start, struct timespec end);
off_t           fsize(const char *filename);
void            print_registers(_cpu_info *cpu);
void            init_cpu(_cpu_info *cpu);
void            unimplemented_opcode(_cpu_info *cpu);
void            print_state(_cpu_info *cpu);
void            load_rom(_cpu_info *cpu, const char *fname, uint16_t offset);

#endif /* UTILS_H */

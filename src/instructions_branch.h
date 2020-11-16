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
#ifndef INSTRUCTIONS_BRANCH_H
#define INSTRUCTIONS_BRANCH_H

#include "types.h"

void emulate_JMP(_cpu_info *cpu);
void emulate_JC(_cpu_info *cpu);
void emulate_JNC(_cpu_info *cpu);
void emulate_JM(_cpu_info *cpu);
void emulate_JP(_cpu_info *cpu);
void emulate_JZ(_cpu_info *cpu);
void emulate_JNZ(_cpu_info *cpu);
void emulate_JPO(_cpu_info *cpu);
void emulate_JPE(_cpu_info *cpu);
void emulate_RETI(_cpu_info *cpu);
void emulate_RET(_cpu_info *cpu);
void emulate_RZ(_cpu_info *cpu);
void emulate_RNZ(_cpu_info *cpu);
void emulate_RP(_cpu_info *cpu);
void emulate_RM(_cpu_info *cpu);
void emulate_RNC(_cpu_info *cpu);
void emulate_RC(_cpu_info *cpu);
void emulate_RPO(_cpu_info *cpu);
void emulate_RPE(_cpu_info *cpu);
void emulate_RST(_cpu_info *cpu);
void emulate_PCHL(_cpu_info *cpu);
void emulate_CPO(_cpu_info *cpu);
void emulate_CPE(_cpu_info *cpu);
void emulate_CNC(_cpu_info *cpu);
void emulate_CP(_cpu_info *cpu);
void emulate_CM(_cpu_info *cpu);
void emulate_CC(_cpu_info *cpu);
void emulate_CC(_cpu_info *cpu);
void emulate_CNZ(_cpu_info *cpu);
void emulate_CZ(_cpu_info *cpu);
void emulate_CALL(_cpu_info *cpu);

#endif /* INSTRUCTIONS_BRANCH_H */

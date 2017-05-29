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
#ifndef TRAINER_H
#define TRAINER_H

#include "types.h"
#include "tetris.h"
#include "lelmark.h"
#include "other_window.h"

//#define TRAIN

double get_cost();

void mutation ( _obj_costs *individual );
void print_pop();
void boot_brain();
void evaluate_cost();
void finished_evaluating_individual();
void update_fitness();
void update_diversity();

_brain* get_brain_pointer();

#endif /* TRAINER_H */

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
#include "other_window.h"

#define TRAIN

#define POP_SIZE 50
#define GEN_P_FUNCTION 3
#define N_FUNCTION     14
#define N_GENES (N_FUNCTION * GEN_P_FUNCTION)

typedef struct {
    double cost[N_GENES];
    double weight[N_GENES];
    int fitness;
    int worst;

    double min[N_GENES];
    double max[N_GENES];
} _obj_costs;

typedef struct {
    double min[N_GENES];
    double max[N_GENES];

    int current;
    int runs;
    int max_runs;
    _obj_costs population[POP_SIZE];
    double mutation_chance;
    double crossover_chance;

    int elapsed_generations;
    int most_lines_cleared;
    int worst_lines_cleared;

    double diversity;

    int rng;

    int round_has_cleaned_lines;
    _bg_info bg_info_copy;
} _brain;

typedef struct {
    _point coord;
    _piece blocks;
    _piece_type type;
    int set;
    int nrotations;
    _obj_costs parameters;
} _best_piece;

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

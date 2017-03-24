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
#include <stdio.h>
#include <stdlib.h>

#include "other_window.h"
#include "trainer.h"
#include "tetris.h"
#include "types.h"

static _brain brain;

void evaluate_cost() {
    brain.population[brain.current].cost[0] = aggregate_height();
    brain.population[brain.current].cost[1] = complete_rows();
    brain.population[brain.current].cost[2] = covered_cells();
    brain.population[brain.current].cost[3] = surface_smoothness();
    brain.population[brain.current].cost[4] = well_cells();
}

void initialize_pop (){
    for (int i = 0; i < POP_SIZE; ++i) {
        for (int j = 0; j < N_GENES; ++j) {
            brain.population[i].weight[j] = 10;
            /*brain.population[i].weight[j] = ( drand48() * 2.0 - 1.0 ) * 50.0;*/
            brain.population[i].cost[j]   = 0;
        }

        brain.population[i].fitness = 0;
    }

    print_pop();
}

double get_cost(){
    _obj_costs* obj = &brain.population[brain.current];

    return (double)obj->cost[0] * obj->weight[0] +
           (double)obj->cost[1] * obj->weight[1] +
           (double)obj->cost[2] * obj->weight[2] +
           (double)obj->cost[3] * obj->weight[3] +
           (double)obj->cost[4] * obj->weight[4] ;
}

_brain* get_brain_pointer() {
    return &brain;
}

void crossover ( _obj_costs *new_pop, _obj_costs *old_pop, int p1, int p2, int pos) {
    int n = rand() % N_GENES;

    _obj_costs a = old_pop[p1];
    _obj_costs b = old_pop[p2];

    if ( drand48() < brain.crossover_chance ) {
        for (int i = 0; i < n; ++i) {
            new_pop[pos + 0].weight[i] = a.weight[i];
            new_pop[pos + 1].weight[i] = b.weight[i];
        }

        for (int i = n; i < N_GENES; ++i) {
            new_pop[pos + 0].weight[i] = b.weight[i];
            new_pop[pos + 1].weight[i] = a.weight[i];
        }
    } else {
        for (int i = 0; i < N_GENES; ++i) {
            new_pop[pos + 0].weight[i] = a.weight[i];
            new_pop[pos + 1].weight[i] = b.weight[i];
        }
    }
}

void mutation ( _obj_costs *individual ) {
    for (int i = 0; i < N_GENES; ++i) {
        if ( drand48() < brain.mutation_chance ) {
            double p = drand48() * 2.0 - 1.0;

            individual->weight[i] += p;
        }
    }
}

_obj_costs get_best_individual() {
    int best = 0;
    int best_i = 0;
    for (int i = 0; i < POP_SIZE; ++i) {
        if ( brain.population[i].fitness > best ) {
            best = brain.population[i].fitness;
            best_i = i;
        }
    }

    brain.most_lines_cleared = best;
    return brain.population[best_i];
}

void selection(_obj_costs *old, _obj_costs *new) {
    int c = 0;

    do {
        int p1 = rand() % POP_SIZE;
        int p2 = rand() % POP_SIZE;

        if ( old[p1].fitness < old[p2].fitness ) {
            new[c] = old[p1];
        } else {
            new[c] = old[p2];
        }
    } while ( ++c < POP_SIZE );
}

void print_pop() {
    for (int i = 0; i < POP_SIZE; ++i) {
        for (int j = 0; j < N_GENES; ++j) {
            printf("%6.2f ", brain.population[i].weight[j]);
        }
        printf("= %4d\n", brain.population[i].fitness);
    }

    printf("\n");
}

void evolutionary_step(){
    _obj_costs new_pop[POP_SIZE];
    _obj_costs best = get_best_individual();

    for (int i = 0; i < POP_SIZE/2; ++i) {
        int p1 = rand() % POP_SIZE;
        int p2 = rand() % POP_SIZE;

        crossover (new_pop, brain.population, p1, p2, i);
    }

    for (int i = 0; i < POP_SIZE; ++i) {
        mutation(&new_pop[i]);
    }

    selection(new_pop, brain.population);

    brain.population[0] = best;

    print_pop();
}

void boot_brain() {
    initialize_pop();
    brain.current          = 0;
    brain.mutation_chance  = ( 1.0 / POP_SIZE * N_GENES ) * 4.0;
    brain.crossover_chance = 0.85;
}

void update_fitness() {
    int a = get_cpu_pointer()->mem_controller.memory[0x9951];
    int b = get_cpu_pointer()->mem_controller.memory[0x9950];
    int c = get_cpu_pointer()->mem_controller.memory[0x994f];
    int d = get_cpu_pointer()->mem_controller.memory[0x994e];

    a = a == 0x2f ? 0 : a;
    b = b == 0x2f ? 0 : b;
    c = c == 0x2f ? 0 : c;
    d = d == 0x2f ? 0 : d;

    brain.population[brain.current].fitness = a + b * 10 + c * 100 + d * 1000;

    /*printf("%3d has fitness = %4d\n", brain.current, brain.population[brain.current].fitness);*/
}

void finished_evaluating_individual () {
    printf("%3d has fitness = %4d\n", brain.current, brain.population[brain.current].fitness);
    brain.current ++;

    if ( brain.current >= POP_SIZE ) {
        evolutionary_step();
        brain.current = 0;
    }
}

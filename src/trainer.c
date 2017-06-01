/******************************************************************************
 * Copyright (C) 2016-2017  Renan S. Silva                                    *
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
#include <math.h>
#include <stdlib.h>
#include <float.h>

#include "feature_functions.h"
#include "ff_controller.h"
#include "other_window.h"
#include "feature_set.h"
#include "trainer.h"
#include "tetris.h"
#include "types.h"
#include "ia.h"
#include "tester.h"

static _brain brain;

/*double trained_ia[] = { -0.80 , 33.87 , 19.87 ,  8.28 , 66.68 , 64.06}; // fitness =   39*/
double trained_ia[] = { 32.35, -49.50, 44.92, -4.96, 67.28, -4.26, -9.10, 42.57, -16.80, 6.98, -12.69, 16.81, -52.29, 35.24, 12.67, -22.58, 18.89, 38.76, -50.81, 70.76, -51.03, -52.32, -37.36, 40.87, -66.04, 39.31, -13.83, 38.29, 58.41, -26.93, 64.36, 52.25, 16.16, -68.15, 51.03, -50.47, 69.55, 4.40, -60.75 }; // fitness =  113

void normalizer() {
    for (int i = 0; i < ff_ctrl_current_plus(); ++i) {
        if ( brain.population[brain.current].cost[i] < brain.population[brain.current].min[i] ) {
            /*printf("new min %f %f\n", brain.population[brain.current].cost[i], brain.population[brain.current].min[i] );*/
            brain.population[brain.current].min[i] = brain.population[brain.current].cost[i];
        }

        if ( brain.population[brain.current].cost[i] > brain.population[brain.current].max[i] ) {
            /*printf("new max %f %f\n", brain.population[brain.current].cost[i], brain.population[brain.current].max[i] );*/
            brain.population[brain.current].max[i] = brain.population[brain.current].cost[i];
        }

        if ( brain.population[brain.current].min[i] != brain.population[brain.current].max[i] ) {
            double cost = brain.population[brain.current].cost[i] - brain.population[brain.current].min[i];
            cost /= brain.population[brain.current].max[i] - brain.population[brain.current].min[i];

            brain.population[brain.current].cost[i] = cost;
        } else {
            brain.population[brain.current].cost[i] = 0;
        }
    }
}

void scaler() {
    for (int i = 0; i < ff_ctrl_current_plus(); ++i) {
        brain.population[brain.current].cost[i] *= brain.population[brain.current].weight[ (i * GEN_P_FUNCTION) + 2 ];
    }
}

void evaluate_cost() {
    get_brain_pointer()->round_has_cleaned_lines = cleaned_any_row() ? 1 : 0;

    ff_ctrl_reset();

    /*feature_set_FBDP();*/
    /*feature_set_NDP();*/
    /*feature_set_KBR();*/
    feature_set_CMA();
    /*feature_set_HA();*/
    /*feature_set_LELmark();*/
    /*feature_set_ALL();*/

    /*normalizer();*/

    scaler();
}

void initialize_pop (){
    for (int i = 0; i < POP_SIZE; ++i) {
        for (int j = 0; j < N_GENES; ++j) {
#ifdef TRAIN
            brain.population[i].weight[j] = ( drand48() * 2.0 - 1.0 ) * 75.0;
            /*brain.population[i].weight[j] = ( drand48() * 2.0 - 1.0 ) * 7.5;*/
#else
            brain.population[i].weight[j] = trained_ia[j];
#endif

            brain.population[i].min[j]    = DBL_MAX;
            brain.population[i].max[j]    =-DBL_MAX;
            brain.population[i].cost[j]   = 0;
            brain.population[i].fitness   = 0;
            brain.population[i].worst     = 0;

            for (int k = 0; k < NRUNS; ++k) {
                brain.population[i].lines_cleared[k]  = 0;
                brain.population[i].pieces_spawned[k] = 0;
            }

            brain.population[i].lines_cleared_total  = 0;
            brain.population[i].pieces_spawned_total = 0;
        }


        brain.population[i].fitness = 0;
        /*mutation(&brain.population[i]);*/
    }
}

void update_diversity() {
    brain.diversity = 0;

    for (int i = 0; i < POP_SIZE; ++i) {
        for (int j = 0; j < POP_SIZE; ++j) {
            double d = 0;
            for (int k = 0; k < ff_ctrl_ngens(); ++k) {
                d += sqrt ( pow(brain.population[i].weight[k] - brain.population[j].weight[k], 2) );
            }
            brain.diversity += d / ( POP_SIZE * POP_SIZE );
        }
    }

}

double get_cost(){
    _obj_costs* obj = &brain.population[brain.current];

    double result = 0;

    for (int i = 0; i < ff_ctrl_current_plus(); i ++ ) {
        result += obj->cost[i];
    }

    return result;
}

_brain* get_brain_pointer() {
    return &brain;
}

void crossover ( _obj_costs *new_pop, _obj_costs *old_pop, int p1, int p2, int pos) {
    _obj_costs a = old_pop[p1];
    _obj_costs b = old_pop[p2];

    if ( drand48() < brain.crossover_chance ) {
        for (int i = 0; i < ff_ctrl_ngens(); ++i) {
            if ( drand48() < .1 ) {
                new_pop[pos + 0].weight[i] = a.weight[i];
                new_pop[pos + 1].weight[i] = b.weight[i];
            } else {
                new_pop[pos + 0].weight[i] = b.weight[i];
                new_pop[pos + 1].weight[i] = a.weight[i];
            }
        }
    } else {
        for (int i = 0; i < ff_ctrl_ngens(); ++i) {
            new_pop[pos + 0].weight[i] = a.weight[i];
            new_pop[pos + 1].weight[i] = b.weight[i];
        }
    }

    new_pop[pos + 0].fitness = -1;
    new_pop[pos + 1].fitness = -1;
}

double random_normal() {
      return sqrt(-2*log(drand48())) * cos(2*M_PI*drand48());
}

void mutation ( _obj_costs *individual ) {
    for (int i = 0; i < ff_ctrl_ngens(); ++i) {
        if ( drand48() < brain.mutation_chance ) {
            individual->weight[i] += random_normal() * 0.3;
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

    return brain.population[best_i];
}

void print_pop() {
    for (int i = 0; i < POP_SIZE; ++i) {
        printf("double trained_ia[] = ");
        for (int j = 0; j < ff_ctrl_ngens(); ++j) {
            printf("%6.2f ,", brain.population[i].weight[j]);
        }
        printf("}; // fitness = %4d\n", brain.population[i].fitness);
    }

    printf("// best: %4d / %4d - %12.4f\n", brain.most_lines_cleared, brain.worst_lines_cleared, brain.diversity);

    printf("\n");
    fflush(stdout);
    fflush(stderr);
}

void selection(_obj_costs *old, _obj_costs *new) {
    for (int c = 0; c < POP_SIZE; c++) {
        int p1 = rand() % POP_SIZE;
        int p2 = rand() % POP_SIZE;

        do {
            p1 = rand() % POP_SIZE;
            p2 = rand() % POP_SIZE;
        } while ( p1 == p2 );

        if ( old[p1].fitness > old[p2].fitness ) {
            for (int i = 0; i < ff_ctrl_ngens(); ++i) {
                new[c].weight[i] = old[p1].weight[i];
                new[c].cost[i]   = 0;
                new[c].fitness   = old[p1].fitness;
            }
        } else {
            for (int i = 0; i < ff_ctrl_ngens(); ++i) {
                new[c].weight[i] = old[p2].weight[i];
                new[c].cost[i]   = 0;
                new[c].fitness   = old[p2].fitness;
            }
        }
    }
}

void evolutionary_step(){
    /*print_pop();*/

    if ( POP_SIZE == 1 )
        return;

    _obj_costs new_pop[POP_SIZE];
    _obj_costs best = get_best_individual();

    selection(brain.population, new_pop);

    for (int i = 0; i < POP_SIZE/2; ++i) {
        int p1 = rand() % POP_SIZE;
        int p2 = rand() % POP_SIZE;

        crossover (brain.population, new_pop, p1, p2, i*2);
    }

    for (int i = 0; i < POP_SIZE; ++i) {
        mutation(&brain.population[i]);
    }

    brain.population[0] = best;

    brain.most_lines_cleared = best.fitness > brain.most_lines_cleared ?
                               best.fitness : brain.most_lines_cleared ;

    brain.elapsed_generations += 1;

    update_diversity();
}

void boot_brain() {
    brain.current             = 0;
#ifdef TRAIN
    brain.mutation_chance     = 0.04;
    brain.crossover_chance    = 0.85;
#else
    brain.mutation_chance     = 0.0;
    brain.crossover_chance    = 0.0;
#endif
    brain.max_runs            = NRUNS;
    brain.runs                = 0;
    brain.worst_lines_cleared = 0;
    brain.most_lines_cleared  = 0;
    brain.diversity           =-1;
    brain.rng                 = 1;

    initialize_pop();
    update_diversity();
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

    int best = a + b * 10 + c * 100 + d * 1000;

    /*printf("%d\n", best);*/

    brain.population[brain.current].fitness = best;

    brain.most_lines_cleared = best > brain.most_lines_cleared ?
                               best : brain.most_lines_cleared ;
}

void finished_evaluating_individual () {
    if ( brain.population[brain.current].fitness < brain.population[brain.current].worst || brain.runs == 0 ) {
        brain.population[brain.current].worst = brain.population[brain.current].fitness;
    }

    brain.runs++;

    /*if ( brain.runs == brain.max_runs || brain.population[brain.current].fitness == 0 ) {*/
    if ( brain.runs == brain.max_runs ) {
        printf("double trained_ia[] = {");
        for (int j = 0; j < ff_ctrl_ngens(); ++j) {
            printf("%6.2f ,", brain.population[brain.current].weight[j]);
        }
        printf("}; // fitness = %4d\n", brain.population[brain.current].fitness);

        for (int i = 0; i < NRUNS; ++i) {
            printf("GEN,%d,CURRENT,%d,RUNS,%d,PS:%d,LC,%d\n", brain.elapsed_generations, brain.current, i,
                    brain.population[brain.current].pieces_spawned[i], brain.population[brain.current].lines_cleared[i]);
            brain.population[brain.current].pieces_spawned[i] = 0;
            brain.population[brain.current].lines_cleared[i] = 0;
        }
        printf("GEN,%d,CURRENT,%d,RUNS,%d,PS:%d,LC,%d\n", brain.elapsed_generations, brain.current, -1,
                brain.population[brain.current].pieces_spawned_total, brain.population[brain.current].lines_cleared_total);
        brain.population[brain.current].pieces_spawned_total = 0;
        brain.population[brain.current].lines_cleared_total = 0;

        fflush(NULL);

        brain.runs = 0;

        if ( brain.population[brain.current].worst > brain.worst_lines_cleared ) {
            brain.worst_lines_cleared = brain.population[brain.current].worst;
        }

        brain.population[brain.current].fitness = brain.population[brain.current].worst;

        brain.current ++;

        if ( brain.current >= POP_SIZE ) {
            evolutionary_step();
            brain.current = 0;
            printf("DIVERSITY,%f\n", brain.diversity);
        }
    }
}


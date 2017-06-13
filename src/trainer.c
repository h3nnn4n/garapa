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
#include "stats.h"
#include "types.h"
#include "ia.h"
#include "tester.h"

#define print_ga_status
#define print_piece_sequence
#define print_piece_stats

static _brain brain;

// HA
/*double trained_ia[] = {32.1508 , 2.8777 , -49.4253 , 39.8463 , 28.9834 , 9.8544  , -32.4268 , 32.5170 , 16.7758 , -27.1612 , -33.5880 , -44.6022 , -6.4512 , 33.3889 , -34.1346 , 9.7343 , -35.0173 , 48.9320 , -46.6092 , -25.8300 , 30.7455 , -31.4399 , 8.8452  , 41.2565 , -33.2424 , 24.3321 , 33.6927 , 0.3008 , 28.2476 , -41.0953 , -0.7421 , 15.4908 , 43.7104 , -30.0510 , -14.2121 , 39.8671 , 24.6210 , 45.9686 , -41.0614 , -27.9304 , 31.9153 , 7.8082 , 28.5994 , -29.1026 , 1.2354  , 42.3936 , -0.5425 , -22.4464 , 2.6247 , -25.2252 , -18.9337 ,  2.9831 , -33.8267 , -46.4482 , -49.1497 , 37.7229 , -10.7922}; // fitness =   879*/
/*double trained_ia[] = {32.1508 , 2.8777 , -49.4253 , 39.8463 , 28.9834 , 9.8544  , -32.4268 , 32.5170 , 16.7758 , -27.1612 , -33.5880 , -44.6022 , -6.4512 , 33.3889 , -34.1346 , 9.7343 , -35.0173 , 48.9320 , -46.6092 , -25.8300 , 30.7455 , -31.4399 , 8.8452  , 41.2565 , -33.2424 , 24.3321 , 33.6927 , 0.3008 , 28.2476 , -41.0953 , -0.7421 , 15.4908 , 43.7104 , -30.0510 , -14.2121 , 39.8671 , 24.6210 , 45.9686 , -41.0614 , -27.9304 , 31.9153 , 7.8082 , 28.5994 , -29.1026 , 1.2354  , 42.3936 , -0.5425 , -22.4464 , 2.6247 , -25.2252 , -18.9337 ,  2.9831 , -33.8267 , -46.4482 , -49.1497 , 37.7229 , -10.7922}; // fitness =   980*/
/*double trained_ia[] = {2.1508  , 2.8777 , -9.4253  , 39.8463 , 28.9834 , 9.8544  , -32.4268 , 32.5170 , 16.7758 , -27.1612 , -33.5880 , -44.6022 , -6.4512 , 33.3889 , -34.1346 , 9.7343 , -35.0173 , 48.9320 , -46.6092 , -25.8300 , 30.7455 , -31.4399 , 8.8452  , 41.2565 , -33.2424 , 24.3321 , 33.6927 , 0.3008 , 28.2476 , -41.0953 , -0.7421 , 15.4908 , 43.7104 , -30.0510 , -14.2121 , 39.8671 , 24.6210 , 45.9686 , -41.0614 , -27.9304 , 31.9153 , 7.8082 , 28.5994 , -29.1026 , 1.2354  , 42.3936 , -0.5425 , -22.4464 , 2.6247 , -25.2252 , -18.9337 ,  2.9831 , -33.8267 , -46.4482 , -49.1497 , 37.7229 , -10.7922}; // fitness =   980*/
/*double trained_ia[] = {  2.1508  , 3.1125 , -14.9078 , 38.3829 , 27.6896 , 11.1381 , -28.3904 , 32.5193 , 16.4433 , -27.1612 , -31.2132 , -44.6022 , -3.3508 , 39.0966 , -34.8357 , 6.3660 , -35.0173 , 49.5293 , -47.4517 , -24.6400 , 29.2209 , -30.4969 , 11.1659 , 45.4112 , -33.2424 , 22.9507 , 32.3496 , 2.7980 , 32.9887 , -40.4129 ,  1.5861 , 11.9503 , 44.7596 , -30.6095 , -14.2121 , 39.8671 , 23.4110 , 46.1063 , -44.6796 , -27.9304 , 32.5854 , 9.2890 , 28.5994 , -30.5301 , -0.0054 , 39.7670 , -0.9932 , -24.5741 , 0.8372 , -25.2088 , -18.9337 , -0.4725 , -35.4515 , -38.1581 , -46.5789 , 37.7229 , -11.9746}; // fitness =  212  126 2784*/

// LELmark
double trained_ia[] = {20.6020, -42.5758, -49.4884, 13.7851, -4.3697, -35.8785, -26.4867, 22.8774, -7.9126, -39.2573, 9.5697, 33.1055, -20.4994, -13.8848, 36.7828, 0.1949, 7.1100, 18.5837, 38.6700, 45.9333, -2.0515, 22.2005, -32.9955, -42.1782, -0.8339, -34.0897, 43.0075, -46.2835, 49.8465, 32.8970, 5.6275, 38.5842, -36.4714, 31.0443, 1.9854, -35.3409, 37.3766, 14.7024, -10.7531, -21.2297, 42.2838, 45.4191}; // fitness =   87   54  372

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
    /*feature_set_CMA();*/
    /*feature_set_HA();*/
    feature_set_LELmark();
    /*feature_set_ALL();*/

    /*feature_set_testing();*/

    /*normalizer();*/

    scaler();
}

void initialize_pop (){
    for (int i = 0; i < POP_SIZE; ++i) {
        for (int j = 0; j < N_GENES; ++j) {
#ifdef TRAIN
            brain.population[i].weight[j] = ( drand48() * 2.0 - 1.0 ) * 50.0;
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
        mutation(&brain.population[i]);
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
            double r = random_normal() * 1.5;
            /*printf("Mutation ammount: %f\n", r);*/
            individual->weight[i] += r;
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

    brain.population[brain.current].fitness = best;

    brain.most_lines_cleared = best > brain.most_lines_cleared ?
                               best : brain.most_lines_cleared ;
}

void print_piece() {
    static int counter = 0;
    switch (get_cpu_pointer()->mem_controller.memory[0xc203]) {
        case 0x0c:
        case 0x0d:
        case 0x0e:
        case 0x0f:
            printf("O");
            break;

        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
            printf("J");
            break;

        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
            printf("L");
            break;

        case 0x08:
        case 0x0a:
        case 0x09:
        case 0x0b:
            printf("I");
            break;

        case 0x14:
        case 0x16:
        case 0x15:
        case 0x17:
            printf("S");
            break;

        case 0x10:
        case 0x12:
        case 0x11:
        case 0x13:
            printf("Z");
            break;

        case 0x18:
        case 0x19:
        case 0x1a:
        case 0x1b:
            printf("T");
            break;

        default:
            fprintf(stderr, "Invalid piece in get_current_piece\n");
            abort();
    }

    if ( (counter++) % 5 == 0 ){
        fflush(NULL);
    }
}

void finished_evaluating_individual () {
    if ( brain.population[brain.current].fitness < brain.population[brain.current].worst || brain.runs == 0 ) {
        brain.population[brain.current].worst = brain.population[brain.current].fitness;
    }
#ifdef print_piece_sequence
    printf("\n");
#endif

    brain.runs++;

    if ( brain.runs == brain.max_runs || brain.population[brain.current].fitness == 0 ) {
#ifdef print_ga_status
        printf("double trained_ia[] = {");
        for (int j = 0; j < ff_ctrl_ngens(); ++j) {
            printf("%6.4f, ", brain.population[brain.current].weight[j]);
        }
        printf("}; // fitness = %4d %4d %4d\n", brain.population[brain.current].fitness, brain.population[brain.current].worst, brain.population[brain.current].lines_cleared_total);
#endif

        for (int i = 0; i < NRUNS; ++i) {
#ifdef print_ga_status
            printf("GEN,%d,CURRENT,%d,RUNS,%d,PS:%d,LC,%d\n", brain.elapsed_generations, brain.current, i,
                    brain.population[brain.current].pieces_spawned[i], brain.population[brain.current].lines_cleared[i]);
#endif
            brain.population[brain.current].pieces_spawned[i] = 0;
            brain.population[brain.current].lines_cleared[i] = 0;
        }
#ifdef print_ga_status
        printf("GEN,%d,CURRENT,%d,RUNS,%d,PS:%d,LC,%d\n", brain.elapsed_generations, brain.current, -1,
                brain.population[brain.current].pieces_spawned_total, brain.population[brain.current].lines_cleared_total);
#endif
        brain.population[brain.current].pieces_spawned_total = 0;
        brain.population[brain.current].lines_cleared_total = 0;

        brain.runs = 0;

        if ( brain.population[brain.current].worst > brain.worst_lines_cleared ) {
            brain.worst_lines_cleared = brain.population[brain.current].worst;
        }

        brain.population[brain.current].fitness = brain.population[brain.current].worst;

        brain.current ++;

        if ( brain.current >= POP_SIZE ) {
            evolutionary_step();
            brain.current = 0;
#ifdef print_ga_status
            printf("DIVERSITY,%f\n", brain.diversity);
#endif
        }

#ifdef print_piece_stats
        print_stats();
#endif

        fflush(NULL);
    }
}


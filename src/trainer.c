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
        brain.population[i].weight[0] = ( drand48() * 2.0 - 1.0 ) * 50.0;
        brain.population[i].weight[1] = ( drand48() * 2.0 - 1.0 ) * 50.0;
        brain.population[i].weight[2] = ( drand48() * 2.0 - 1.0 ) * 50.0;
        brain.population[i].weight[3] = ( drand48() * 2.0 - 1.0 ) * 50.0;
        brain.population[i].weight[4] = ( drand48() * 2.0 - 1.0 ) * 50.0;
    }
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

void evolutionary_step(){
    _obj_costs new_pop[POP_SIZE];

    for (int i = 0; i < POP_SIZE/2; ++i) {
        int p1 = rand() % POP_SIZE;
        int p2 = rand() % POP_SIZE;

        crossover (new_pop, brain.population, p1, p2, i);
    }

    for (int i = 0; i < POP_SIZE; ++i) {
        mutation(&brain.population[i]);
        brain.population[i] = new_pop[i];
    }
}

void boot_brain() {
    initialize_pop();
    brain.current = 0;
}

void finished_evaluating_individual () {
    brain.current ++;

    if ( brain.current >= POP_SIZE ) {
        evolutionary_step();
        brain.current = 0;
    }
}

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

void evolutionary_step(){

}

void boot_brain() {
    initialize_pop();
    brain.current = 0;
}

void finished_evaluating_individual () {
    brain.current ++;

    if ( brain.current >= POP_SIZE ) {
        evolutionary_step();
    }
}

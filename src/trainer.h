#ifndef TRAINER_H
#define TRAINER_H

#define POP_SIZE 4
#define N_GENES 5

typedef struct {
    //int aggregate_height_cost;
    //int complete_rows_cost;
    //int covered_cells_cost;
    //int surface_smoothness_cost;
    //int well_cells_cost;

    //double aggregate_height_weight;
    //double complete_rows_weight;
    //double covered_cells_weight;
    //double surface_smoothness_weight;
    //double well_cells_weight;

    int cost[N_GENES];
    double weight[N_GENES];
    int fitness;
} _obj_costs;

typedef struct {
    int current;
    _obj_costs population[POP_SIZE];
    double mutation_chance;
    double crossover_chance;
    int elapsed_generations;
    int most_lines_cleared;
} _brain;

double get_cost();
void print_pop();
void boot_brain();
void evaluate_cost();
void finished_evaluating_individual();
void update_fitness();

_brain* get_brain_pointer();

#endif /* TRAINER_H */

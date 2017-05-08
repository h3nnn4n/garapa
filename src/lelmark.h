#ifndef LELMARK_H
#define LELMARK_H

#define POP_SIZE 50
#define GEN_P_FUNCTION 3
//#define N_FUNCTION     14
#define N_FUNCTION     40
#define N_GENES (N_FUNCTION * GEN_P_FUNCTION)

#define MAX_SPRITE 1000
#define __X_SIZE 10
#define __Y_SIZE 17

typedef struct {
    int x;
    int y;
} _point;

typedef struct {
    _point a;
    _point b;
    _point c;
    _point d;
} _piece;

typedef enum {SQUARE, Ja, Jb, Jc, Jd, La, Lb, Lc, Ld, Ia, Ib, Sa, Sb, Za, Zb, Ta, Tb, Tc, Td, Unknow} _piece_type;

typedef struct {
    double cost[N_GENES];
    double weight[N_GENES];
    int fitness;
    int worst;

    double min[N_GENES];
    double max[N_GENES];
} _obj_costs;

typedef struct {
    int data[__X_SIZE][__Y_SIZE];
} _bg_info;

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

typedef struct {
    int posx;
    int posy;
    int id;
} _sprite_list;

typedef struct {
    _sprite_list sprite_list[MAX_SPRITE];
    int used_sprites;
} _sprite_t_info;

typedef struct {
    int ready;
    int wait_rotation;
} _move_queue;

typedef enum {BOOTING, INGAME, GAMEOVER} _game_state;

typedef struct {
    _game_state game_state;
} _ai_state;

#endif /* LELMARK_H */

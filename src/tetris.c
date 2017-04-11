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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "other_window.h"
#include "types.h"
#include "tetris.h"
#include "trainer.h"
#include "graphics.h"

_piece Ta_piece     = {{ -1, -1 }, {  0, -1 }, { 1, -1 }, { 0, 0 }} ; // Ta
_piece Tb_piece     = {{  0, -2 }, {  0, -1 }, { 1, -1 }, { 0, 0 }} ; // Tb
_piece Tc_piece     = {{ -1, -1 }, { -2,  0 }, {-1,  0 }, { 0, 0 }} ; // Tc
_piece Td_piece     = {{  0, -2 }, { -1, -1 }, { 0, -1 }, { 0, 0 }} ; // Td

_piece Ja_piece     = {{ -2, -1 }, { -1, -1 }, { 0, -1 }, { 0, 0 }} ; // Ja
_piece Jb_piece     = {{  0, -2 }, {  1, -2 }, { 0, -1 }, { 0, 0 }} ; // Jb
_piece Jc_piece     = {{ -2, -1 }, { -2,  0 }, {-1,  0 }, { 0, 0 }} ; // Jc
_piece Jd_piece     = {{  0, -2 }, {  0, -1 }, {-1,  0 }, { 0, 0 }} ; // Jd

_piece La_piece     = {{  0, -1 }, {  1, -1 }, { 2, -1 }, { 0, 0 }} ; // La
_piece Lb_piece     = {{ -1, -2 }, { -1, -1 }, {-1,  0 }, { 0, 0 }} ; // Lb
_piece Lc_piece     = {{  0, -1 }, { -2,  0 }, {-1,  0 }, { 0, 0 }} ; // Lc
_piece Ld_piece     = {{ -1, -2 }, {  0, -2 }, { 0, -1 }, { 0, 0 }} ; // Ld

_piece Sa_piece     = {{  0, -1 }, {  1, -1 }, {-1,  0 }, { 0, 0 }} ; // Sa
_piece Sb_piece     = {{ -1, -2 }, { -1, -1 }, { 0, -1 }, { 0, 0 }} ; // Sb

_piece Ia_piece     = {{ -3, 0  }, { -2,  0 }, {-1,  0 }, { 0, 0 }} ; // Ia
_piece Ib_piece     = {{  0, -3 }, {  0, -2 }, { 0, -1 }, { 0, 0 }} ; // Ib

_piece Za_piece     = {{ -2, -1 }, { -1, -1 }, {-1,  0 }, { 0, 0 }} ; // Za
_piece Zb_piece     = {{  1, -2 }, {  0, -1 }, { 1, -1 }, { 0, 0 }} ; // Zb

_piece Square_piece = {{ -1, -1 }, {  0, -1 }, {-1,  0 }, { 0, 0 }} ; // Square

// Function n 0
double aggregate_height() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 0;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    for (int i = 0; i < __X_SIZE; ++i) {
        int last = __Y_SIZE;
        int x    = 0;
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] >= 1 ) {
                last = j;
                break;
            }
        }

        x = __Y_SIZE - last;

        x = base[0] * x + base[1];

        total += x;
    }

    return total * base[2];
}

// Function n 1
double complete_rows(){
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 1;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    for (int j = 0; j < __Y_SIZE; ++j) {
        int ok = 1;
        for (int i = 0; i < __X_SIZE; ++i) {
            if ( bg_info->data[i][j] == 0 ) {
                ok = 0;
                break;
            }
        }

        total += ok;
    }

    return total * base[2];
}

// Function n 2
double surface_variance() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 2;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    int h[__X_SIZE];

    for (int i = 0; i < __X_SIZE; ++i) {
        h[i] = 0;
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] >= 1 ) {
                h[i] = __Y_SIZE - j;
                break;
            }
        }
    }

    for (int i = 0; i < 9; ++i) {
        double x = abs(h[i] - h[i+1]);
        total += x * base[0] + base[1];
    }

    return total * base[2];
}

// Function n 3
double covered_cells() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 3;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    int total = 0;

    for (int i = 0; i < __X_SIZE; ++i) {
        int found = 0;
        for (int j = 0; j < __Y_SIZE; ++j) {
            int x    = 0;
            if ( bg_info->data[i][j] >= 1 && !found ) {
                found = 1;
            } else if ( bg_info->data[i][j] == 0 && found ) {
                x = __Y_SIZE - j;

                x = x * base[0] + base[1];
                total += x;
            }
        }
    }

    return total * base[2];
}

// Function n 4
double well_cells(){
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 4;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    for (int i = 1; i < 9; ++i) {
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] == 0 && bg_info->data[i-1][j] >= 1 && bg_info->data[i+1][j] >= 1  ) {
                double x = (__Y_SIZE - j);
                total += x * base[0] + base[1];
            } else if ( bg_info->data[i][j] >= 1 ) {
                break;
            }
        }
    }

    for (int j = 0; j < __Y_SIZE; ++j) {
        if ( bg_info->data[9][j] == 0 && bg_info->data[8][j] >= 1  ) {
            double x = (__Y_SIZE - j);
            total += x * base[0] + base[1];
        } else if ( bg_info->data[9][j] >= 1 ) {
            break;
        }
    }

    for (int j = 0; j < __Y_SIZE; ++j) {
        if ( bg_info->data[0][j] == 0 && bg_info->data[1][j] >= 1  ) {
            double x = (__Y_SIZE - j);
            total += x * base[0] + base[1];
        } else if ( bg_info->data[0][j] >= 1 ) {
            break;
        }
    }

    return total * base[2];
}

// Function n 5
double covered_cells_after_clear(){
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 5;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;
    double found_complete = 0;

    for (int j = 0; j < __Y_SIZE; ++j) {
        int ok = 1;
        for (int i = 0; i < __X_SIZE; ++i) {
            if ( bg_info->data[i][j] == 0 ) {
                ok = 0;
                break;
            }
        }

        if ( ok ) {
            found_complete = j;
            break;
        }
    }

    if ( found_complete ) {
        for (int i = 0; i < __X_SIZE; ++i) {
            int found = 0;
            for (int j = 0; j < found_complete; ++j) {
                int x    = 0;
                if ( bg_info->data[i][j] >= 1 && !found ) {
                    found = 1;
                } else if ( bg_info->data[i][j] == 0 && found ) {

                    x = base[0] * x + base[1];

                    x = __Y_SIZE - j;

                    x = x * base[0] + base[1];
                    total += x;
                }
            }
        }
    }

    return total * base[2];
}

// Function n 6
double lock_heigth(){
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 6;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    for (int i = 0; i < __X_SIZE; ++i) {
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] > 1 ) {
                total += base[0] * (__Y_SIZE -j) + base[1];
            }
        }
    }

    return total * base[2];
}


// Function n 7
double burried_cells() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 7;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    int total = 0;

    for (int i = 0; i < __X_SIZE; ++i) {
        int found = 0;
        for (int j = 0; j < __Y_SIZE; ++j) {
            int x    = 0;
            if ( bg_info->data[i][j] > 1 && !found ) {
                found = 1;
            } else if ( bg_info->data[i][j] == 0 && found ) {
                x = __Y_SIZE - j;

                x = x * base[0] + base[1];
                total += x;
            }
        }
    }

    return total * base[2];
}

// Function n 8
double highest_cell() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 8;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    for (int i = 0; i < __X_SIZE; ++i) {
        int last = __Y_SIZE;
        int x    = 0;
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] >= 1 ) {
                last = j;
                break;
            }
        }

        x = __Y_SIZE - last;

        if ( x > total ) {
            total = x;
        }
    }

    return (total * base[0] + base[1] ) * base[2];
}

// Function n 9
double height_delta() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 9;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double min = 10000;
    double max =-10000;

    for (int i = 0; i < __X_SIZE; ++i) {
        int last = __Y_SIZE;
        int x    = 0;
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] >= 1 ) {
                last = j;
                break;
            }
        }

        x = __Y_SIZE - last;

        max = max > x ? max : x;
        min = min < x ? min : x;
    }

    return ((max - min) * base[0] + base[1] ) * base[2];
}

// Function n 10
double vertical_roughness() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 10;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    for (int i = 0; i < __X_SIZE; ++i) {
        for (int j = 1; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] != bg_info->data[i][j - 1] ) {
                total += base[0] * (1) + base[1];
            }
        }
    }

    return total * base[2];
}

// Function n 11
double horizontal_roughness() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 11;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    for (int i = 1; i < __X_SIZE; ++i) {
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] != bg_info->data[i - 1][j] ) {
                total += base[0] * (1) + base[1];
            }
        }
    }

    return total * base[2];
}

// Function n 12
double vertical_roughness_w() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 10;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    for (int i = 0; i < __X_SIZE; ++i) {
        for (int j = 1; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] != bg_info->data[i][j - 1] ) {
                total += base[0] * (17 - j) + base[1];
            }
        }
    }

    return total * base[2];
}

// Function n 13
double horizontal_roughness_w() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 13;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    for (int i = 1; i < __X_SIZE; ++i) {
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] != bg_info->data[i - 1][j] ) {
                total += base[0] * (17 - j) + base[1];
            }
        }
    }

    return total * base[2];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

int piece_touched_the_ground (_piece piece, int dx, int dy){
    int x = get_cpu_pointer()->mem_controller.memory[0xff92] - 8;
    int y = get_cpu_pointer()->mem_controller.memory[0xff93] - 16;

    int max_y = -999;

    dx += x;

    x = dx;

    max_y = max_y > piece.a.y ? max_y : piece.a.y;
    max_y = max_y > piece.b.y ? max_y : piece.b.y;
    max_y = max_y > piece.c.y ? max_y : piece.c.y;
    max_y = max_y > piece.d.y ? max_y : piece.d.y;

    max_y *= 8;

    if ( y + dy + max_y >= 142 )
        return 1;

    return 0;
}

int is_inside_bounds(_piece piece, int dx, int dy){
    int x = get_cpu_pointer()->mem_controller.memory[0xff92] - 8;
    int y = get_cpu_pointer()->mem_controller.memory[0xff93] - 16;

    int min_x =  999;
    int max_x = -999;

    dy += y;

    y = dy;

    min_x = min_x < piece.a.x ? min_x : piece.a.x;
    min_x = min_x < piece.b.x ? min_x : piece.b.x;
    min_x = min_x < piece.c.x ? min_x : piece.c.x;
    min_x = min_x < piece.d.x ? min_x : piece.d.x;

    max_x = max_x > piece.a.x ? max_x : piece.a.x;
    max_x = max_x > piece.b.x ? max_x : piece.b.x;
    max_x = max_x > piece.c.x ? max_x : piece.c.x;
    max_x = max_x > piece.d.x ? max_x : piece.d.x;

    min_x *= 8;
    max_x *= 8;

    if ( x + dx + min_x < 16 || x + dx + max_x > 88 ) {
        return 0;
    }

    return 1;
}

int add_piece(_piece piece, int dx, int dy) {
    int x = get_cpu_pointer()->mem_controller.memory[0xff92] - 8;
    int y = get_cpu_pointer()->mem_controller.memory[0xff93] - 16;
    _bg_info *bg = get_bg_info_pointer();

    int xx;
    int yy;

    xx = (x + dx + piece.a.x*8) / 8 - 2;
    yy = (y + dy + piece.a.y*8) / 8 - 1;
    bg->data[xx][yy] += 2;

    xx = (x + dx + piece.b.x*8) / 8 - 2;
    yy = (y + dy + piece.b.y*8) / 8 - 1;
    bg->data[xx][yy] += 2;

    xx = (x + dx + piece.c.x*8) / 8 - 2;
    yy = (y + dy + piece.c.y*8) / 8 - 1;
    bg->data[xx][yy] += 2;

    xx = (x + dx + piece.d.x*8) / 8 - 2;
    yy = (y + dy + piece.d.y*8) / 8 - 1;
    bg->data[xx][yy] += 2;

    return 1;
}

int can_fit(_piece piece, int dx, int dy) {
    int x = get_cpu_pointer()->mem_controller.memory[0xff92] - 8;
    int y = get_cpu_pointer()->mem_controller.memory[0xff93] - 16;
    _bg_info *bg = get_bg_info_pointer();

    int xx ;
    int yy ;

    xx = (x + dx + piece.a.x*8) / 8 - 2;
    yy = (y + dy + piece.a.y*8) / 8 - 1;

    if ( bg->data[xx][yy] == 1 ) return 0;

    xx = (x + dx + piece.b.x*8) / 8 - 2;
    yy = (y + dy + piece.b.y*8) / 8 - 1;

    if ( bg->data[xx][yy] == 1 ) return 0;

    xx = (x + dx + piece.c.x*8) / 8 - 2;
    yy = (y + dy + piece.c.y*8) / 8 - 1;

    if ( bg->data[xx][yy] == 1 ) return 0;

    xx = (x + dx + piece.d.x*8) / 8 - 2;
    yy = (y + dy + piece.d.y*8) / 8 - 1;

    if ( bg->data[xx][yy] == 1 ) return 0;

    return 1;
}

void restore_bg() {
    for (int i = 0; i < __X_SIZE; ++i) {
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( get_bg_info_pointer()->data[i][j] > 1 ) {
                get_bg_info_pointer()->data[i][j] = 0;
            }
        }
    }
}

void dump_bg() {
    printf("    +---------------------+\n");
    for (int j = 0; j < __Y_SIZE; ++j) {
        printf("%3d | ", j);
        for (int i = 0; i < __X_SIZE; ++i) {
            switch ( get_bg_info_pointer()->data[i][j] ) {
                case 0:
                    printf("  ");
                    break;
                case 1:
                    printf("X ");
                    break;
                case 2:
                    printf("O ");
                    break;
                default:
                    break;
            }
        }
        printf("|\n");
    }
    printf("    +---------------------+\n\n");
}
void get_best_move(){
    double best_cost = -999999;
    _best_piece *best_piece = get_best_piece_pointer();

    best_piece->set = 0;

    _piece_type piece_type = get_current_piece();
    _piece piece = get_piece_coord_from_id(piece_type);

    int x = get_cpu_pointer()->mem_controller.memory[0xff92] - 8;
    int y = get_cpu_pointer()->mem_controller.memory[0xff93] - 24;

    for (int n_totation = 0; n_totation < get_piece_rotation(piece_type); ++n_totation) {
        for (int dx = -80 ; dx < 96; dx += 8) {
            if ( is_inside_bounds(piece, dx, 16)) {
                int first = 0;
                for (int dy = 24; dy < 8*20; dy += 8 ) {
                    if ( can_fit(piece, dx, dy )) {
                        first = 1;
                    } else if ( first ) {
                        add_piece(piece, dx, dy - 8);
                        evaluate_cost();
                        if ( best_cost < get_cost() ) {
                            best_cost           = get_cost();
                            best_piece->coord.x = dx + x;
                            best_piece->coord.y = dy + y;
                            best_piece->type    = piece_type;
                            best_piece->blocks  = piece;
                            best_piece->set     = 1;
                            best_piece->nrotations = n_totation;
                        }

                        restore_bg();
                        break;
                    } else {
                        break;
                    }

                    if ( piece_touched_the_ground (piece, dx, dy) ) {
                        add_piece(piece, dx, dy - 8);
                        evaluate_cost();
                        best_cost           = get_cost();
                        best_piece->coord.x = dx + x;
                        best_piece->coord.y = dy + y;
                        best_piece->type    = piece_type;
                        best_piece->blocks  = piece;
                        best_piece->set     = 1;
                        best_piece->nrotations = n_totation;
                        restore_bg();
                        break;
                    }
                }
            }
        }

        piece_type = rotate_piece ( piece_type );
        piece = get_rotated_piece ( piece_type );
    }
}

_piece get_rotated_piece (_piece_type piece_type ) {
    switch (piece_type) {
        case La:
            return La_piece;
        case Lb:
            return Lb_piece;
        case Lc:
            return Lc_piece;
        case Ld:
            return Ld_piece;

        case Ta:
            return Ta_piece;
        case Tb:
            return Tb_piece;
        case Tc:
            return Tc_piece;
        case Td:
            return Td_piece;

        case Ja:
            return Ja_piece;
        case Jb:
            return Jb_piece;
        case Jc:
            return Jc_piece;
        case Jd:
            return Jd_piece;

        case Za:
            return Za_piece;
        case Zb:
            return Zb_piece;

        case Sa:
            return Sa_piece;
        case Sb:
            return Sb_piece;

        case Ia:
            return Ia_piece;
        case Ib:
            return Ib_piece;

        case SQUARE:
            return Square_piece;

        default:
            fprintf(stderr, "Invalid piece in rotate_piece\n");
            abort();
    }
}

_piece_type rotate_piece (_piece_type piece_type ) {
    switch (piece_type) {
        case La:
            return Lb;
        case Lb:
            return Lc;
        case Lc:
            return Ld;
        case Ld:
            return La;

        case Ta:
            return Tb;
        case Tb:
            return Tc;
        case Tc:
            return Td;
        case Td:
            return Ta;

        case Ja:
            return Jb;
        case Jb:
            return Jc;
        case Jc:
            return Jd;
        case Jd:
            return Ja;

        case Za:
            return Zb;
        case Zb:
            return Za;

        case Sa:
            return Sb;
        case Sb:
            return Sa;

        case Ia:
            return Ib;
        case Ib:
            return Ia;

        case SQUARE:
            return SQUARE;

        default:
            fprintf(stderr, "Invalid piece in rotate_piece\n");
            abort();
    }
}

int get_piece_rotation ( _piece_type piece_type ) {
    switch (piece_type) {
        case La:
        case Lb:
        case Lc:
        case Ld:

        case Ta:
        case Tb:
        case Tc:
        case Td:

        case Ja:
        case Jb:
        case Jc:
        case Jd:
            return 4;

        case Za:
        case Zb:

        case Sa:
        case Sb:

        case Ia:
        case Ib:
            return 2;

        case SQUARE:
            return 1;

        default:
            fprintf(stderr, "Invalid piece on get_piece_rotation\n");
            abort();
    }
}

_piece get_piece_coord_from_id( _piece_type piece_type ) {
    switch (piece_type) {
        case La:
        case Lb:
        case Lc:
        case Ld:
            return La_piece;

        case Ta:
        case Tb:
        case Tc:
        case Td:
            return Ta_piece;

        case Ja:
        case Jb:
        case Jc:
        case Jd:
            return Ja_piece;

        case Za:
        case Zb:
            return Za_piece;

        case Sa:
        case Sb:
            return Sa_piece;

        case Ia:
        case Ib:
            return Ia_piece;

        case SQUARE:
            return Square_piece;

        default:
            fprintf(stderr, "Invalid piece in get_piece_coord_from_id\n");
            abort();
    }
}

_piece_type get_current_piece(){
    _cpu_info *cpu = get_cpu_pointer();
    int index;
    int value;

    index = 0xc203;
    value = cpu->mem_controller.memory[index];

    switch (value) {
        case 0x0c:
        case 0x0d:
        case 0x0e:
        case 0x0f:
            return SQUARE;

        case 0x04:
            return Ja;
        case 0x05:
            return Jb;
        case 0x06:
            return Jc;
        case 0x07:
            return Jd;

        case 0x00:
            return La;
        case 0x01:
            return Lb;
        case 0x02:
            return Lc;
        case 0x03:
            return Ld;

        case 0x08:
        case 0x0a:
            return Ia;
        case 0x09:
        case 0x0b:
            return Ib;

        case 0x14:
        case 0x16:
            return Sa;
        case 0x15:
        case 0x17:
            return Sb;

        case 0x10:
        case 0x12:
            return Za;
        case 0x11:
        case 0x13:
            return Zb;

        case 0x18:
            return Ta;
        case 0x19:
            return Tb;
        case 0x1a:
            return Tc;
        case 0x1b:
            return Td;

        default:
            fprintf(stderr, "Invalid piece in get_current_piece\n");
            abort();
    }
}

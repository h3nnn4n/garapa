/******************************************************************************
 * Copyright (C) 2017  Renan S. Silva                                         *
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
#include <string.h>
#include <stdlib.h>

#include "feature_functions.h"
#include "other_window.h"

// Function n 0
// The sum of all column heights
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

    return total;
}

// Function n 1
// Number of rows that will be cleared
double complete_rows(){
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 0;
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

    return (base[0] * total + base[1]);
}

// Function n 2
// The sum of the height difference between 2 adjacent columns
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

    for (int i = 0; i < __X_SIZE - 1; ++i) {
        double x = abs(h[i] - h[i+1]);
        total += x * base[0] + base[1];
    }

    return total;
}

// Function n 3
// Number of hole cells weighted by their height
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

    return total;
}

// Function n 4
// Number of well cells, also can be used as total well depth
double well_cells(){ // total well depth
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 4;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    for (int i = 1; i < 9; ++i) {
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] == 0 && bg_info->data[i-1][j] >= 1 && bg_info->data[i+1][j] >= 1  ) {
                double x = 1;
                total += x * base[0] + base[1];
            } else if ( bg_info->data[i][j] >= 1 ) {
                break;
            }
        }
    }

    for (int j = 0; j < __Y_SIZE; ++j) {
        if ( bg_info->data[9][j] == 0 && bg_info->data[8][j] >= 1  ) {
            double x = 1;
            total += x * base[0] + base[1];
        } else if ( bg_info->data[9][j] >= 1 ) {
            break;
        }
    }

    for (int j = 0; j < __Y_SIZE; ++j) {
        if ( bg_info->data[0][j] == 0 && bg_info->data[1][j] >= 1  ) {
            double x = 1;
            total += x * base[0] + base[1];
        } else if ( bg_info->data[0][j] >= 1 ) {
            break;
        }
    }

    return total;
}

// Function n 5
// Number of completed rows weighted by their height
double complete_rows_weighted(){
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 5;
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

        if ( ok ) {
            total += 17 - j;
        }
    }

    return (base[0] * total + base[1]);
}

// Function n 6
// The lock height (Y position of where the piece was placed
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

    return total;
}

// Function n 7
// Number of empty blocks below the highest hole
double burried_cells() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 7;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    int total = 0;

    int highest = 0;
    int pos = 0;
    int clear = 1;

    // This finds the highest hole
    for (int i = 0; i < __X_SIZE; ++i) {
        int found = 0;
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] > 1 && !found ) {
                found = 1;
            } else if ( bg_info->data[i][j] == 0 && found ) {
                clear = 0;
                if ( (__Y_SIZE - j) > highest ) {
                    pos = j;
                    highest = __Y_SIZE - j;
                }
            }
        }
    }

    if ( clear ) {
        total = 0;
    } else {
        for (int i = 0; i < __X_SIZE; ++i) {
            for (int j = pos; j < __Y_SIZE; ++j) {
                if ( bg_info->data[i][j] == 0 ) {
                    total += 1 * base[0] + base[1];
                }
            }
        }
    }

    return total;
}

// Function n 8
// The hiighest cell on the board
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

    return (total * base[0] + base[1] );
}

// Function n 9
// the bigegst height difference between 2 columns
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

    return ((max - min) * base[0] + base[1] );
}

// Function n 10
// aka verical transitions
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

    return total;
}

// Function n 11
// Same as above, but for horizontal changes
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

    return total;
}

// Function n 12
// same as f10 but weighted
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

    return total;
}

// Function n 13
// same as f11 but weighted
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

    return total;
}

// Function n 14
// The height of a single column
double column_height( int pos ) {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 14;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    int i = pos;

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

    return total;
}

// Function n 15
// the height difference between 2 adjacent columns
double height_difference( int pos ) { // Column Difference
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 15;
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

    int i = pos;

    double x = abs(h[i] - h[i+1]);
    total += x * base[0] + base[1];

    return total;
}

// Function n 16
// the mean column height
double mean_column_height() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 16;
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
        total += h[i];
    }

    total = (total / __X_SIZE) * base[0] + base[1];

    return total;
}

// Function n 17
// max - mean column height
double max_mean_column_height() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 17;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    int h[__X_SIZE];

    int max = 0;

    for (int i = 0; i < __X_SIZE; ++i) {
        h[i] = 0;
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] >= 1 ) {
                h[i] = __Y_SIZE - j;
                break;
            }
        }
        max = max > h[i] ? max : h[i];
        total += h[i];
    }

    total = (max - total / __X_SIZE) * base[0] + base[1];

    return total;
}

// Function n 18
// mean - min column height
double min_mean_column_height() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 18;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    int h[__X_SIZE];

    int min = 0;

    for (int i = 0; i < __X_SIZE; ++i) {
        h[i] = 0;
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] >= 1 ) {
                h[i] = __Y_SIZE - j;
                break;
            }
        }
        min = min < h[i] ? min : h[i];
        total += h[i];
    }

    total = ((total / __X_SIZE) - min) * base[0] + base[1];

    return total;
}

// Function n 19
// mean hole depth
double mean_hole_depth() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 19;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    int total = 0;
    int nholes = 0;

    for (int i = 0; i < __X_SIZE; ++i) {
        int found = 0;
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] >= 1 && !found ) {
                found = 1;
            } else if ( bg_info->data[i][j] == 0 && found ) {
                total += __Y_SIZE - j;
                nholes ++;
            }
        }
    }

    total = (total / nholes) * base[0] + base[1];;

    return total;
}

// Function n 20
// The number of blocks in the board
double blocks() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 20;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    for (int i = 0; i < __X_SIZE; ++i) {
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] >= 1 ) {
                total += base[0] * 1 + base[1];
            }
        }
    }

    return total;
}

// Function n 21
// The number of blocks in the board, but a block on the i-th row counts i times as much
double blocks_weighted() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 21;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    double total = 0;

    for (int i = 0; i < __X_SIZE; ++i) {
        int x    = 0;
        for (int j = 0; j < __Y_SIZE; ++j) {
            if ( bg_info->data[i][j] >= 1 ) {
                x = __Y_SIZE - j;
                x = base[0] * x + base[1];
                total += base[0] * x + base[1];
            }
        }
    }

    return total;
}

// Function n 22
// Eroded pieces
double eroded_pieces() {
    fprintf(stderr, "Not implemented\n");

    return 0;
}

// Function n 23
// Number of well cells, also can be used as total well depth, weighted
double well_cells_weigthed(){ // total well depth
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 24;
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

    return total;
}

// functon n24
double rows_with_a_hole() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 24;
    _brain* brain     = get_brain_pointer();
    double *base      = &brain->population[brain->current].weight[fid * GEN_P_FUNCTION];

    int holes[__Y_SIZE];

    for (int i = 0; i < __Y_SIZE; ++i) holes[i] = 0;

    int total = 0;

    for (int j = 0; j < __Y_SIZE; ++j) {
        int found = 0;
        for (int i = 0; i < __X_SIZE; ++i) {
            if ( bg_info->data[i][j] >= 1 && !found ) {
                found = 1;
            } else if ( bg_info->data[i][j] == 0 && found ) {
                holes[j] ++;
            }
        }
    }

    for (int i = 0; i < __Y_SIZE; ++i) {
        if ( holes[i] > 0 ) {
            total += base[0] * 1 + base[1];
        }
    }

    return total;
}

// WIP TODO FIXME
// Function n 25
// The sum of the number of occupied pieces above each hole
double hole_depth() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 25;
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

    return total;
}

// Function n 26
// minimum height
double min_height() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 26;
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

    return ((min) * base[0] + base[1] );
}

// Function n 27
// Maximum height
double max_height() {
    _bg_info *bg_info = get_bg_info_pointer();
    int fid           = 1;
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

    return ((max) * base[0] + base[1] );
}


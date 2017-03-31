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
#ifndef TETRIS_H
#define TETRIS_H

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

_piece get_piece_coord_from_id();

void get_best_move();
//double get_cost();

double aggregate_height();          // f1
double complete_rows();             // f2
double surface_smoothness();        // f3
double covered_cells();             // f4
double well_cells();                // f5
double covered_cells_after_clear(); // f6
double lock_heigth();               // f7
double burried_cells();             // f8

void initialize_weight ();

int get_piece_rotation ( _piece_type piece_type ) ;
_piece_type rotate_piece (_piece_type piece_type ) ;
_piece get_rotated_piece (_piece_type piece_type ) ;

_piece_type get_current_piece();

#endif /* TETRIS_H */

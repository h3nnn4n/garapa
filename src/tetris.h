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

#include "lelmark.h"

_piece get_piece_coord_from_id();

void get_best_move();

int cleaned_any_row();
void clear_lines();
void restore_bg();
void save_bg();
void dump_bg();
void reset_bg();

void initialize_weight ();

int get_piece_rotation ( _piece_type piece_type ) ;
_piece_type rotate_piece (_piece_type piece_type ) ;
_piece get_rotated_piece (_piece_type piece_type ) ;

_piece_type get_current_piece();

int try_and_fit(double *best_cost, int *first, int *found, int dx, int dy, int x, int y, _piece piece, _piece_type piece_type, int n_totation );

#endif /* TETRIS_H */

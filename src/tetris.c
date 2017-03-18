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
#include <stdio.h>
#include <stdlib.h>

#include "other_window.h"
#include "types.h"
#include "tetris.h"
#include "graphics.h"

_piece Ta_piece     = {  { -1 , -1 } , { 0  , -1 } , { 1  , -1 } , { 0 , 0 }  } ; // Ta
_piece Tb_piece     = {  { -1 , -1 } , { 0  , -1 } , { 1  , -1 } , { 0 , 0 }  } ; // Ta
_piece Tc_piece     = {  { -1 , -1 } , { 0  , -1 } , { 1  , -1 } , { 0 , 0 }  } ; // Ta
_piece Td_piece     = {  { -1 , -1 } , { 0  , -1 } , { 1  , -1 } , { 0 , 0 }  } ; // Ta

_piece Ja_piece     = {  { -2 , -1 } , { -1 , -1 } , { 0  , -1 } , { 0 , 0 }  } ; // Ja
_piece Jb_piece     = {  { 0  , -2 } , { 1  , -2 } , { 0  , -1 } , { 0 , 0 }  } ; // Jb
_piece Jc_piece     = {  { -2 , -1 } , { -2 , 0  } , { -1 , 0  } , { 0 , 0 }  } ; // Jc
_piece Jd_piece     = {  { 0  , -2 } , { 0  , -1 } , { -1 , 0  } , { 0 , 0 }  } ; // Jd

_piece La_piece     = {  { 0  , -1 } , { 1  , -1 } , { 2  , -1 } , { 0 , 0 }  } ; // La
_piece Lb_piece     = {  { -1 , -2 } , { -1 , -1 } , { -1 , 0  } , { 0 , 0 }  } ; // Lb
_piece Lc_piece     = {  { 0  , -1 } , { -2 , 0  } , { -1 , 0  } , { 0 , 0 }  } ; // Lc
_piece Ld_piece     = {  { -1 , -2 } , { 0  , -2 } , { 0  , -1 } , { 0 , 0 }  } ; // Ld

_piece Sa_piece     = {  { 0  , -1 } , { 1  , -1 } , { -1 , 0  } , { 0 , 0 }  } ; // Sa
_piece Sb_piece     = {  { -1 , -2 } , { -1 , -1 } , { 0  , -1 } , { 0 , 0 }  } ; // Sb

_piece Ia_piece     = {  { -3 , 0  } , { -2 , 0  } , { -1 , 0  } , { 0 , 0 }  } ; // Ia
_piece Ib_piece     = {  { 0  , -3 } , { 0  , -2 } , { 0  , -1 } , { 0 , 0 }  } ; // Ib

_piece Za_piece     = {  { -2 , -1 } , { -1 , -1 } , { -1 , 0  } , { 0 , 0 }  } ; // Za
_piece Zb_piece     = {  { 1  , -2 } , { 0  , -1 } , { 1  , -1 } , { 0 , 0 }  } ; // Zb

_piece Square_piece = {  { -1 , -1 } , { 0  , -1 } , { -1 , 0  } , { 0 , 0 }  } ; // Square


_piece NULL_piece = {{-1,  -1}, { 0,  -1}, { -1,  0}, { 0,   0}}; // Null piece, also a square

int aggregate_height() {
    _bg_info *bg_info = get_bg_info_pointer();

    int total = 0;

    for (int i = 0; i < 10; ++i) {
        int last = 17;
        for (int j = 0; j < 17; ++j) { if ( bg_info->data[i][j] == 1 ) { last = j;
                break;
            }
        }

        total += 17 - last;
    }

    return total;
}

int complete_rows(){
    _bg_info *bg_info = get_bg_info_pointer();

    int total = 0;

    for (int j = 0; j < 17; ++j) {
        int ok = 1;
        for (int i = 0; i < 10; ++i) {
            if ( bg_info->data[i][j] == 0 ) {
                ok = 0;
                break;
            }
        }

        total += ok;
    }

    return total;
}

int surface_smoothness() {
    _bg_info *bg_info = get_bg_info_pointer();

    int total = 0;

    int h[10];

    for (int i = 0; i < 10; ++i) {
        h[i] = 0;
        for (int j = 0; j < 17; ++j) {
            if ( bg_info->data[i][j] == 1 ) {
                h[i] = 17 - j;
                break;
            }
        }
    }

    for (int i = 0; i < 9; ++i) {
        total += abs(h[i] - h[i+1]);
    }

    return total;
}

int covered_cells() {
    _bg_info *bg_info = get_bg_info_pointer();

    int total = 0;

    for (int i = 0; i < 10; ++i) {
        int found = 0;
        for (int j = 0; j < 17; ++j) {
            if ( bg_info->data[i][j] == 1 && !found ) {
                found = 1;
            } else if ( bg_info->data[i][j] == 0 && found ) {
                total++;
            }
        }
    }

    return total;
}

int well_cells(){
    _bg_info *bg_info = get_bg_info_pointer();

    int total = 0;

    for (int i = 1; i < 9; ++i) {
        for (int j = 0; j < 17; ++j) {
            if ( bg_info->data[i][j] == 0 && bg_info->data[i-1][j] == 1 && bg_info->data[i+1][j] == 1  ) {
                total += 1;
            } else if ( bg_info->data[i][j] == 1 ) {
                break;
            }
        }
    }

    for (int j = 0; j < 17; ++j) {
        if ( bg_info->data[9][j] == 0 && bg_info->data[8][j] == 1  ) {
            total += 1;
        } else if ( bg_info->data[9][j] == 1 ) {
            break;
        }
    }

    for (int j = 0; j < 17; ++j) {
        if ( bg_info->data[0][j] == 0 && bg_info->data[1][j] == 1  ) {
            total += 1;
        } else if ( bg_info->data[0][j] == 1 ) {
            break;
        }
    }

    return total;
}

double get_cost(){
    _obj_costs* obj = get_obj_cost_pointer();

    return (double)obj->aggregate_height_cost   * obj->aggregate_height_weight   +
           (double)obj->complete_rows_cost      * obj->complete_rows_weight      +
           (double)obj->covered_cells_cost      * obj->covered_cells_weight      +
           (double)obj->surface_smoothness_cost * obj->surface_smoothness_weight +
           (double)obj->well_cells_cost         * obj-> well_cells_weight        ;
}

void initialize_weight (){
    _obj_costs* obj = get_obj_cost_pointer();

    obj->aggregate_height_weight   = 5.0;
    obj->complete_rows_weight      = 5.0;
    obj->covered_cells_weight      = 5.0;
    obj->surface_smoothness_weight = 5.0;
    obj->well_cells_weight         = 5.0;
}

_point get_best_move(){
    _point best = {0, 0};
    _piece piece = get_piece_coord_from_id();

    int min_x = 999;
    int max_x = 0;

    min_x = min_x < piece.a.x ? min_x : piece.a.x;
    min_x = min_x < piece.b.x ? min_x : piece.b.x;
    min_x = min_x < piece.c.x ? min_x : piece.c.x;
    min_x = min_x < piece.d.x ? min_x : piece.d.x;

    max_x = max_x > piece.a.x ? min_x : piece.a.x;
    max_x = max_x > piece.b.x ? min_x : piece.b.x;
    max_x = max_x > piece.c.x ? min_x : piece.c.x;
    max_x = max_x > piece.d.x ? min_x : piece.d.x;

    printf("%3d %3d\n", min_x, max_x);

    return best;
}

_piece get_piece_coord_from_id() {
    _piece_type piece_type = get_current_piece();

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

        case Sa:
        case Sb:
            return Sa_piece;

        case Ia:
        case Ib:
            return Ia_piece;

        case SQUARE:
            return Ia_piece;

        default:
            return NULL_piece;
    }

    return NULL_piece;
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
            return Unknow;
    }
}

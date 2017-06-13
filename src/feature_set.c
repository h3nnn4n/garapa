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

#include "feature_functions.h"
#include "ff_controller.h"
#include "feature_set.h"
#include "trainer.h"
#include "tetris.h"

void feature_set_testing(){
    _brain *brain = get_brain_pointer();

    brain->population[brain->current].cost[ff_ctrl_next()] = complete_rows();
    brain->population[brain->current].cost[ff_ctrl_next()] = complete_rows_weighted();
    brain->population[brain->current].cost[ff_ctrl_next()] = eroded_pieces();

    if ( cleaned_any_row() ) {
        clear_lines();
    }

    brain->population[brain->current].cost[ff_ctrl_next()] = max_height();
    brain->population[brain->current].cost[ff_ctrl_next()] = min_height();
    /*brain->population[brain->current].cost[ff_ctrl_next()] = mean_column_height();*/
    /*brain->population[brain->current].cost[ff_ctrl_next()] = max_mean_column_height();*/
    /*brain->population[brain->current].cost[ff_ctrl_next()] = min_mean_column_height();*/
    brain->population[brain->current].cost[ff_ctrl_next()] = holes();
    /*brain->population[brain->current].cost[ff_ctrl_next()] = holes_vertical();*/
    /*brain->population[brain->current].cost[ff_ctrl_next()] = mean_hole_depth();*/
    /*brain->population[brain->current].cost[ff_ctrl_next()] = height_delta();*/
    brain->population[brain->current].cost[ff_ctrl_next()] = well_cells();
    /*brain->population[brain->current].cost[ff_ctrl_next()] = max_well_depth();*/
    /*brain->population[brain->current].cost[ff_ctrl_next()] = number_of_wells();*/
    /*brain->population[brain->current].cost[ff_ctrl_next()] = total_well_depth();*/
    /*brain->population[brain->current].cost[ff_ctrl_next()] = well_cells_weigthed();*/
    brain->population[brain->current].cost[ff_ctrl_next()] = lock_heigth();
    brain->population[brain->current].cost[ff_ctrl_next()] = aggregate_height();
    /*brain->population[brain->current].cost[ff_ctrl_next()] = blocks();*/
    brain->population[brain->current].cost[ff_ctrl_next()] = blocks_weighted();

    brain->population[brain->current].cost[ff_ctrl_next()] = horizontal_roughness();
    brain->population[brain->current].cost[ff_ctrl_next()] = vertical_roughness();

    /*brain->population[brain->current].cost[ff_ctrl_next()] = highest_hole();*/
    /*brain->population[brain->current].cost[ff_ctrl_next()] = blocks_above_highest_hole();*/
    brain->population[brain->current].cost[ff_ctrl_next()] = potential_rows();
    brain->population[brain->current].cost[ff_ctrl_next()] = surface_variance();
    brain->population[brain->current].cost[ff_ctrl_next()] = rows_with_a_hole();
    /*brain->population[brain->current].cost[ff_ctrl_next()] = hole_depth();*/
    brain->population[brain->current].cost[ff_ctrl_next()] = pattern_diversity();
    /*brain->population[brain->current].cost[ff_ctrl_next()] = free_blocks();*/

    /*brain->population[brain->current].cost[ff_ctrl_next()] = blocks_above_highest_hole_w();*/
    /*brain->population[brain->current].cost[ff_ctrl_next()] = horizontal_roughness_w();*/
    /*brain->population[brain->current].cost[ff_ctrl_next()] = vertical_roughness_w();*/

    /*for (int i = 0; i < 10; ++i) {*/
        /*brain->population[brain->current].cost[ff_ctrl_next()]  = column_height(i);*/
    /*}*/

    /*for (int i = 0; i < 9; ++i) {*/
        /*brain->population[brain->current].cost[ff_ctrl_next()]  = height_difference(i);*/
    /*}*/
}

void feature_set_FBDP(){
    _brain *brain = get_brain_pointer();

    if ( cleaned_any_row() ) {
        clear_lines();
    }

    brain->population[brain->current].cost[ff_ctrl_next()] = holes(); // 1
    brain->population[brain->current].cost[ff_ctrl_next()] = highest_cell(); // 6
}

void feature_set_NDP(){
    _brain *brain = get_brain_pointer();

    if ( cleaned_any_row() ) {
        clear_lines();
    }

    brain->population[brain->current].cost[ff_ctrl_next()] = holes();
    brain->population[brain->current].cost[ff_ctrl_next()] = highest_cell();

    for (int i = 0; i < 10; ++i) {
        brain->population[brain->current].cost[ff_ctrl_next()] = column_height(i);
    }

    for (int i = 0; i < 9; ++i) {
        brain->population[brain->current].cost[ff_ctrl_next()] = height_difference(i);
    }
}

void feature_set_KBR(){
    _brain *brain = get_brain_pointer();

    if ( cleaned_any_row() ) {
        clear_lines();
    }

    brain->population[brain->current].cost[ff_ctrl_next()] = holes(); // 1
    brain->population[brain->current].cost[ff_ctrl_next()] = highest_cell(); // 6

    for (int i = 0; i < 10; ++i) {
        brain->population[brain->current].cost[ff_ctrl_next()] = column_height(i);
    }

    for (int i = 0; i < 9; ++i) {
        brain->population[brain->current].cost[ff_ctrl_next()] = height_difference(i);
    }
    brain->population[brain->current].cost[ff_ctrl_next()] = mean_column_height();     // 3
    brain->population[brain->current].cost[ff_ctrl_next()] = min_height();             // 2
    brain->population[brain->current].cost[ff_ctrl_next()] = max_mean_column_height(); // 4
    brain->population[brain->current].cost[ff_ctrl_next()] = min_mean_column_height(); // 5
    brain->population[brain->current].cost[ff_ctrl_next()] = mean_hole_depth();        // 8
    brain->population[brain->current].cost[ff_ctrl_next()] = well_cells();             // 16
}

void feature_set_CMA(){
    _brain *brain = get_brain_pointer();

    brain->population[brain->current].cost[ff_ctrl_next()] = complete_rows(); // 10
    brain->population[brain->current].cost[ff_ctrl_next()] = eroded_pieces(); // 32

    if ( cleaned_any_row() ) {
        clear_lines();
    }

    brain->population[brain->current].cost[ff_ctrl_next()] = holes(); // 6
    brain->population[brain->current].cost[ff_ctrl_next()] = blocks_weighted(); // 21
    brain->population[brain->current].cost[ff_ctrl_next()] = well_cells(); // 16
    brain->population[brain->current].cost[ff_ctrl_next()] = blocks(); // 20
    brain->population[brain->current].cost[ff_ctrl_next()] = lock_heigth(); // 18

    brain->population[brain->current].cost[ff_ctrl_next()] = horizontal_roughness(); // 22
    brain->population[brain->current].cost[ff_ctrl_next()] = vertical_roughness(); // 24

    brain->population[brain->current].cost[ff_ctrl_next()] = well_cells_weigthed(); // 17
    brain->population[brain->current].cost[ff_ctrl_next()] = hole_depth(); // 34

    brain->population[brain->current].cost[ff_ctrl_next()] = rows_with_a_hole(); // 33
    brain->population[brain->current].cost[ff_ctrl_next()] = pattern_diversity(); // 35
}

void feature_set_HA2(){
    _brain *brain = get_brain_pointer();

    brain->population[brain->current].cost[ff_ctrl_next()] = complete_rows(); // 10
    brain->population[brain->current].cost[ff_ctrl_next()] = complete_rows_weighted();
    brain->population[brain->current].cost[ff_ctrl_next()] = eroded_pieces(); // 32

    if ( cleaned_any_row() ) {
        clear_lines();
    }

    brain->population[brain->current].cost[ff_ctrl_next()] = holes(); // 1
    brain->population[brain->current].cost[ff_ctrl_next()] = highest_cell(); // 6
    brain->population[brain->current].cost[ff_ctrl_next()] = holes_vertical(); // 7
    brain->population[brain->current].cost[ff_ctrl_next()] = height_delta(); // 12
    brain->population[brain->current].cost[ff_ctrl_next()] = max_well_depth(); // 14
    brain->population[brain->current].cost[ff_ctrl_next()] = well_cells(); // 13
    brain->population[brain->current].cost[ff_ctrl_next()] = lock_heigth(); // 18

    brain->population[brain->current].cost[ff_ctrl_next()] = blocks(); // 20
    brain->population[brain->current].cost[ff_ctrl_next()] = blocks_weighted(); // 21
    brain->population[brain->current].cost[ff_ctrl_next()] = horizontal_roughness(); // 22
    brain->population[brain->current].cost[ff_ctrl_next()] = vertical_roughness(); // 24
    brain->population[brain->current].cost[ff_ctrl_next()] = horizontal_roughness_w();
    brain->population[brain->current].cost[ff_ctrl_next()] = vertical_roughness_w();

    brain->population[brain->current].cost[ff_ctrl_next()] = highest_hole(); // 26
    brain->population[brain->current].cost[ff_ctrl_next()] = blocks_above_highest_hole(); // 27
    brain->population[brain->current].cost[ff_ctrl_next()] = potential_rows(); // 29
    brain->population[brain->current].cost[ff_ctrl_next()] = surface_variance(); // 30
    brain->population[brain->current].cost[ff_ctrl_next()] = rows_with_a_hole(); // 33
    brain->population[brain->current].cost[ff_ctrl_next()] = hole_depth(); // 34
}

void feature_set_HA(){
    _brain *brain = get_brain_pointer();

    brain->population[brain->current].cost[ff_ctrl_next()] = complete_rows(); // 10
    brain->population[brain->current].cost[ff_ctrl_next()] = eroded_pieces(); // 32

    if ( cleaned_any_row() ) {
        clear_lines();
    }

    brain->population[brain->current].cost[ff_ctrl_next()] = holes(); // 1
    brain->population[brain->current].cost[ff_ctrl_next()] = highest_cell(); // 6
    brain->population[brain->current].cost[ff_ctrl_next()] = holes_vertical(); // 7
    brain->population[brain->current].cost[ff_ctrl_next()] = height_delta(); // 12
    brain->population[brain->current].cost[ff_ctrl_next()] = max_well_depth(); // 14
    brain->population[brain->current].cost[ff_ctrl_next()] = well_cells(); // 13
    brain->population[brain->current].cost[ff_ctrl_next()] = lock_heigth(); // 18

    brain->population[brain->current].cost[ff_ctrl_next()] = blocks(); // 20
    brain->population[brain->current].cost[ff_ctrl_next()] = blocks_weighted(); // 21
    brain->population[brain->current].cost[ff_ctrl_next()] = horizontal_roughness(); // 22
    brain->population[brain->current].cost[ff_ctrl_next()] = vertical_roughness(); // 24

    brain->population[brain->current].cost[ff_ctrl_next()] = highest_hole(); // 26
    brain->population[brain->current].cost[ff_ctrl_next()] = blocks_above_highest_hole(); // 27
    brain->population[brain->current].cost[ff_ctrl_next()] = potential_rows(); // 29
    brain->population[brain->current].cost[ff_ctrl_next()] = surface_variance(); // 30
    brain->population[brain->current].cost[ff_ctrl_next()] = rows_with_a_hole(); // 33
    brain->population[brain->current].cost[ff_ctrl_next()] = hole_depth(); // 34
}

void feature_set_LELmark(){
    _brain *brain = get_brain_pointer();

    brain->population[brain->current].cost[ff_ctrl_next()] = complete_rows();
    brain->population[brain->current].cost[ff_ctrl_next()] = complete_rows_weighted();

    if ( cleaned_any_row() ) {
        clear_lines();
    }

    brain->population[brain->current].cost[ff_ctrl_next()] = aggregate_height();
    brain->population[brain->current].cost[ff_ctrl_next()] = covered_cells();
    brain->population[brain->current].cost[ff_ctrl_next()] = surface_variance();
    brain->population[brain->current].cost[ff_ctrl_next()] = well_cells();
    brain->population[brain->current].cost[ff_ctrl_next()] = lock_heigth();
    brain->population[brain->current].cost[ff_ctrl_next()] = burried_cells();
    brain->population[brain->current].cost[ff_ctrl_next()] = highest_cell();
    brain->population[brain->current].cost[ff_ctrl_next()] = height_delta();
    brain->population[brain->current].cost[ff_ctrl_next()] = vertical_roughness();
    brain->population[brain->current].cost[ff_ctrl_next()] = horizontal_roughness();
    brain->population[brain->current].cost[ff_ctrl_next()] = vertical_roughness_w();
    brain->population[brain->current].cost[ff_ctrl_next()] = horizontal_roughness_w();
}

void feature_set_ALL(){
    _brain *brain = get_brain_pointer();

    brain->population[brain->current].cost[ff_ctrl_next()] = complete_rows();
    brain->population[brain->current].cost[ff_ctrl_next()] = complete_rows_weighted();
    brain->population[brain->current].cost[ff_ctrl_next()] = eroded_pieces();

    if ( cleaned_any_row() ) {
        clear_lines();
    }

    brain->population[brain->current].cost[ff_ctrl_next()] = max_height();
    brain->population[brain->current].cost[ff_ctrl_next()] = min_height();
    brain->population[brain->current].cost[ff_ctrl_next()] = mean_column_height();
    brain->population[brain->current].cost[ff_ctrl_next()] = max_mean_column_height();
    brain->population[brain->current].cost[ff_ctrl_next()] = min_mean_column_height();
    brain->population[brain->current].cost[ff_ctrl_next()] = holes();
    brain->population[brain->current].cost[ff_ctrl_next()] = holes_vertical();
    brain->population[brain->current].cost[ff_ctrl_next()] = mean_hole_depth();
    brain->population[brain->current].cost[ff_ctrl_next()] = height_delta();
    brain->population[brain->current].cost[ff_ctrl_next()] = well_cells();
    brain->population[brain->current].cost[ff_ctrl_next()] = max_well_depth();
    brain->population[brain->current].cost[ff_ctrl_next()] = number_of_wells();
    brain->population[brain->current].cost[ff_ctrl_next()] = total_well_depth();
    brain->population[brain->current].cost[ff_ctrl_next()] = well_cells_weigthed();
    brain->population[brain->current].cost[ff_ctrl_next()] = lock_heigth();
    brain->population[brain->current].cost[ff_ctrl_next()] = aggregate_height();
    brain->population[brain->current].cost[ff_ctrl_next()] = blocks();
    brain->population[brain->current].cost[ff_ctrl_next()] = blocks_weighted();

    brain->population[brain->current].cost[ff_ctrl_next()] = horizontal_roughness();
    brain->population[brain->current].cost[ff_ctrl_next()] = vertical_roughness();

    brain->population[brain->current].cost[ff_ctrl_next()] = highest_hole();
    brain->population[brain->current].cost[ff_ctrl_next()] = blocks_above_highest_hole();
    brain->population[brain->current].cost[ff_ctrl_next()] = potential_rows();
    brain->population[brain->current].cost[ff_ctrl_next()] = surface_variance();
    brain->population[brain->current].cost[ff_ctrl_next()] = rows_with_a_hole();
    brain->population[brain->current].cost[ff_ctrl_next()] = hole_depth();
    brain->population[brain->current].cost[ff_ctrl_next()] = pattern_diversity();
    brain->population[brain->current].cost[ff_ctrl_next()] = free_blocks();

    brain->population[brain->current].cost[ff_ctrl_next()] = blocks_above_highest_hole_w();
    brain->population[brain->current].cost[ff_ctrl_next()] = horizontal_roughness_w();
    brain->population[brain->current].cost[ff_ctrl_next()] = vertical_roughness_w();
}

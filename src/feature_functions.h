#ifndef FEATURE_FUNCTIONS_H
#define FEATURE_FUNCTIONS_H

double aggregate_height();          // f0
double complete_rows();             // f1
double surface_variance();          // f2
double covered_cells();             // f3
double well_cells();                // f4
double complete_rows_weighted();    // f5
double lock_heigth();               // f6
double burried_cells();             // f7
double highest_cell();              // f8
double height_delta();              // f9
double vertical_roughness();        // f10
double horizontal_roughness();      // f11
double vertical_roughness_w();      // f12
double horizontal_roughness_w();    // f13
double column_height(int pos);      // f14
double height_difference(int pos);  // f15
double mean_column_height();        // f16
double max_mean_column_height();    // f17
double min_mean_column_height();    // f18
double mean_hole_depth();           // f19
double blocks();                    // f20
double blocks_weighted();           // f21
double eroded_pieces();             // f22
double well_cells_weigthed();       // f23
double rows_with_a_hole();          // f24
double min_height();                // f25
double max_height();                // f26
double holes();                     // f27
double hole_depth();
double pattern_diversity();

#endif /* FEATURE_FUNCTIONS_H */

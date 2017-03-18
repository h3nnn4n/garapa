#ifndef TETRIS_H
#define TETRIS_H

typedef enum {SQUARE, Ja, Jb, Jc, Jd, La, Lb, Lc, Ld, Ia, Ib, Sa, Sb, Za, Zb, Ta, Tb, Tc, Td, Unknow} _piece_type;

int aggregate_height();
int holes();
int covered_cells();
int surface_smoothness();
int well_cells();
int complete_rows();

_piece_type get_current_piece();

#endif /* TETRIS_H */

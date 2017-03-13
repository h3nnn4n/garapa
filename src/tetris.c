#include <stdio.h>
#include <stdlib.h>

#include "other_window.h"
#include "types.h"
#include "graphics.h"

int aggregate_height() {
    _bg_info *bg_info = get_bg_info_pointer();

    int total = 0;

    for (int i = 0; i < 10; ++i) {
        int last = 17;
        for (int j = 0; j < 17; ++j) {
            if ( bg_info->data[i][j] == 1 ) {
                last = j;
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
        int found = 0;
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

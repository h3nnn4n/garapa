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

int holes() {
    _bg_info *bg_info = get_bg_info_pointer();

    int total = 0;

    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 16; ++j) {
            if ( bg_info->data[i][j] == 0 ) {
                if ( bg_info->data[i-1][j] == 1 && bg_info->data[i+1][j] == 1 ) {
                    if ( bg_info->data[i][j-1] == 1 && bg_info->data[i][j+1] == 1 ) {
                        total += 1;
                    }
                }
            }
        }
    }

    return total;
}

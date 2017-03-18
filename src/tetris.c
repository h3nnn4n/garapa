#include <stdio.h>
#include <stdlib.h>

#include "other_window.h"
#include "types.h"
#include "tetris.h"
#include "graphics.h"

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

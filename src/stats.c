#include <stdio.h>
#include <stdlib.h>

#include "stats.h"

static _stats stats;

void reset_stats(){
    stats.O = 0;
    stats.S = 0;
    stats.Z = 0;
    stats.L = 0;
    stats.J = 0;
    stats.I = 0;
    stats.T = 0;
}

void update_stats(int id){
    switch (id) {
        case 0x0c:
        case 0x0d:
        case 0x0e:
        case 0x0f:
            stats.O ++;
            break;

        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
            stats.J ++;
            break;

        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
            stats.L ++;
            break;

        case 0x08:
        case 0x0a:
        case 0x09:
        case 0x0b:
            stats.I ++;
            break;

        case 0x14:
        case 0x16:
        case 0x15:
        case 0x17:
            stats.S ++;
            break;

        case 0x10:
        case 0x12:
        case 0x11:
        case 0x13:
            stats.Z ++;
            break;

        case 0x18:
        case 0x19:
        case 0x1a:
        case 0x1b:
            stats.T ++;
            break;

        default:
            fprintf(stderr, "Invalid piece in get_current_piece\n");
            abort();
    }
}

void print_stats() {
    printf("O: %4d I: %4d S: %4d Z: %4d L: %4d J: %4d T: %4d\n", stats.O, stats.I, stats.S, stats.Z, stats.L, stats.J, stats.T);
}

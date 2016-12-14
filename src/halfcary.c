#include <stdint.h>

#include "halfcary.h"

int halfcary_sub( uint8_t a, uint8_t b, uint8_t c ) {
    uint8_t b1 = ( a & 0x08 ) >> 1;
    uint8_t b2 = ( b & 0x08 ) >> 2;
    uint8_t b3 = ( c & 0x08 ) >> 3;
    static int table[] = { 0, 1, 1, 1, 0, 0, 0, 1 };

    return table[ b1 | b2 | b3 ];
}

int halfcary( uint8_t a, uint8_t b, uint8_t c ) {
    uint8_t b1 = ( a & 0x08 ) >> 1;
    uint8_t b2 = ( b & 0x08 ) >> 2;
    uint8_t b3 = ( c & 0x08 ) >> 3;
    static int table[] = { 0, 0, 1, 0, 1, 0, 1, 1 };

    return table[ b1 | b2 | b3 ];
}

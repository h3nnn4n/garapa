#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int main(int argc, char *argv[]) {
    FILE *f = NULL;
    unsigned char *buffer = NULL;

    if ( argc == 1 ) {
        printf("Usage: %s file\n", argv[0]);
        return EXIT_FAILURE;
    } else {
        f = fopen(argv[1], "rb");
        buffer = malloc( fsize( argv[1] ) );
    }

    return 0;
}

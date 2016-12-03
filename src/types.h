#ifndef TYPES_H
#define TYPES_H

typedef struct {
    unsigned char *memory;

    unsigned int pc;
    unsigned int sp;

    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
    unsigned int e;
    unsigned int f;
} _cpu_info;

#endif /* TYPES_H */

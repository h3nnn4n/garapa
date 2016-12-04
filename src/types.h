#ifndef TYPES_H
#define TYPES_H

typedef struct {
    unsigned char z  ; // Zero flag
    unsigned char s  ; // Signal bit
    unsigned char p  ; // Parity bit
    unsigned char cy ; // Cary bit
    unsigned char ac ; // Auxiliary carry
} _cpu_flags;

typedef struct {
    unsigned char *memory;

    _cpu_flags flags;

    unsigned int pc;
    unsigned int sp;

    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
    unsigned int e;
    unsigned int h;
    unsigned int l;
} _cpu_info;

#endif /* TYPES_H */

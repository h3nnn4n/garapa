#ifndef STATS_H
#define STATS_H

typedef struct {
    int O;
    int S;
    int Z;
    int L;
    int J;
    int I;
    int T;
} _stats;

void reset_stats();
void update_stats(int id);
void print_stats();

#endif /* STATS_H */

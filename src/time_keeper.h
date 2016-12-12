#ifndef TIME_KEEPER_H
#define TIME_KEEPER_H

#include <time.h>
#include <unistd.h>

struct timespec time_diff( struct timespec start, struct timespec end );
void     timekeeper_tic  ( struct timespec *t1                        );
void     timekeeper_wait ( struct timespec *t1  , struct timespec *t2 );

#endif /* TIME_KEEPER_H */

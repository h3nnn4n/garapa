#include <time.h>
#include <unistd.h>

#include "time_keeper.h"

struct timespec time_diff(struct timespec start, struct timespec end) {
    struct timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}

void timekeeper_tic  (struct timespec *t1) {
    clock_gettime(CLOCK_MONOTONIC, t1);
}

void timekeeper_wait (struct timespec *t1, struct timespec *t2) {
    clock_gettime(CLOCK_MONOTONIC, t2);
    struct timespec t3;
    t3.tv_nsec = 16667000 - (time_diff(*t1, *t2).tv_nsec);
    t3.tv_sec  = 0;
    nanosleep( &t3, NULL );
    timekeeper_tic(t1);
}

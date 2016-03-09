#include <time.h>
#include <sys/time.h>
#include "my_time_utils.h"

#define EXP_3 1000
#define EXP_6 1000000
#define EXP_9 1000000000

void sleep_ns(int nanosec) {
    struct timespec interval;
    interval.tv_sec = nanosec / EXP_9;
    interval.tv_nsec = nanosec % EXP_9;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &interval, NULL);
}

void sleep_us(int microsec) {
    sleep_ns(microsec * EXP_3);
}

// to use only with values up to a couple seconds
void sleep_ms(int millisec) {
    sleep_ns(millisec * EXP_6);
}
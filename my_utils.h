#ifndef MY_UTILS_H
#define MY_UTILS_H

#define DEFAULT_PRIORITY 	0
#define MIN_PRIORITY 		1
#define MAX_PRIORITY		99

typedef enum {
    RPI_UNKNOWN,
    RPI_1B_1,
    RPI_1B_2,
    RPI_1A_2,
    RPI_1BPLUS_1,
    RPI_1APLUS_1,
    RPI_1BPLUS_12,
    RPI_2B_11,
    RPI_ZERO_12,
    RPI_3B_12
} rpi_model;


rpi_model get_rpi_model(void);
int set_process_priority(int priority);

#endif
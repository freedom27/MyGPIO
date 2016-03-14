#include <sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_utils.h"


rpi_model get_rpi_model_from_revision(char *str_rev);

rpi_model get_rpi_model_from_revision(char *str_rev) {
    int revision = strtol(str_rev, NULL, 16);
    switch(revision) {
        case 0x2:
        case 0x3:
            return RPI_1B_1;
        case 0x4:
        case 0x5:
        case 0x6:
        case 0xD:
        case 0xE:
        case 0xF:
            return RPI_1B_2;
        case 0x7:
        case 0x8:
        case 0x9:
            return RPI_1A_2;
        case 0x10:
            return RPI_1BPLUS_1;
        case 0x12:
            return RPI_1APLUS_1;
        case 0x13:
            return RPI_1BPLUS_12;
        case 0xA01041:
        case 0xA21041:
            return RPI_2B_11;
        case 0x900092:
            return RPI_ZERO_12;
        case 0xA02082:
            return RPI_3B_12;
        default:
            return RPI_UNKNOWN;
    }
}

rpi_model get_rpi_model(void) {
    FILE *fd;
    char file_line[256];
    char cpu_revision[10];
    
    if ((fd = fopen("/proc/cpuinfo", "r")) == NULL) {
        exit(1);
    }
    
    while (!feof(fd) && fgets(file_line, sizeof(file_line), fd) != NULL) {
        if (strncmp (file_line, "Revision", 8) == 0) {
            sscanf(file_line, "Revision\t: %s", cpu_revision);
            break;
        }
    }

    return get_rpi_model_from_revision(cpu_revision);
}

int set_process_priority(int priority) {
    struct sched_param params;
    memset(&params, 0, sizeof(params));
	
    switch(priority) {
        case DEFAULT_PRIORITY:
            params.sched_priority = 0;
            break;
        case MAX_PRIORITY:
            params.sched_priority = sched_get_priority_max(SCHED_FIFO);
            break;
        case MIN_PRIORITY:
            params.sched_priority = sched_get_priority_min(SCHED_FIFO);
            break;
        default:
            params.sched_priority = priority;
            break;
    }
    
    return sched_setscheduler(0, SCHED_FIFO, &params);
}
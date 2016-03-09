#include <sched.h>
#include <string.h>
#include "my_utils.h"

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
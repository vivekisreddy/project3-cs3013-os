#include "scheduler.h"
#include <stdio.h>

void run_fifo(job_t* jobs) {
    printf("Execution trace with FIFO:\n");
    while (jobs) {
        printf("Job %d ran for: %d\n", jobs->id, jobs->length);
        jobs = jobs->next;
    }
    printf("End of execution with FIFO.\n");
}
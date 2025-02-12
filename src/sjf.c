#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>

void run_sjf(job_t* jobs) {
    int count = 0;
    for (job_t* temp = jobs; temp; temp = temp->next) count++;
    
    job_t** job_array = malloc(count * sizeof(job_t*));
    int i = 0;
    for (job_t* temp = jobs; temp; temp = temp->next)
        job_array[i++] = temp;
    
    for (int j = 0; j < count - 1; j++) {
        for (int k = j + 1; k < count; k++) {
            if (job_array[j]->length > job_array[k]->length) {
                job_t* tmp = job_array[j];
                job_array[j] = job_array[k];
                job_array[k] = tmp;
            }
        }
    }
    printf("Execution trace with SJF:\n");
    for (i = 0; i < count; i++)
        printf("Job %d ran for: %d\n", job_array[i]->id, job_array[i]->length);
    
    printf("End of execution with SJF.\n");
    free(job_array);
}

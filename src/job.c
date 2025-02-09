#include "job.h"
#include <stdio.h>
#include <stdlib.h>

job_t* load_jobs(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;
    
    job_t *head = NULL, *tail = NULL;
    int length, id = 0;
    while (fscanf(file, "%d", &length) == 1) {
        job_t* new_job = malloc(sizeof(job_t));
        new_job->id = id++;
        new_job->length = length;
        new_job->next = NULL;
        if (!head) head = tail = new_job;
        else tail = tail->next = new_job;
    }
    fclose(file);
    return head;
}

void free_jobs(job_t* head) {
    while (head) {
        job_t* temp = head;
        head = head->next;
        free(temp);
    }
}
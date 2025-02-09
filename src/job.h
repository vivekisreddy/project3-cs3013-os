#ifndef JOB_H
#define JOB_H

typedef struct job {
    int id;
    int length;
    struct job *next;
} job_t;

job_t* load_jobs(const char* filename);
void free_jobs(job_t* head);
#endif
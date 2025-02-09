#ifndef JOB_H
#define JOB_H

typedef struct job {
    int id;
    int length;
    struct job *next;
} job_t;


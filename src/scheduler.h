#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "job.h"
void run_fifo(job_t* jobs);
void run_sjf(job_t* jobs);
void run_rr(job_t* jobs, int time_slice);
#endif

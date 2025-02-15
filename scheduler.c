#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Job struct representing each job in the workload
struct job {
    int id;
    int length;
    int response_time;
    int turnaround_time;
    int wait_time;
    struct job *next;
};

struct job* read_workload(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    struct job *head = NULL, *tail = NULL;
    int length, id = 0;
    
    while (fscanf(file, "%d", &length) == 1) {
        struct job *new_job = (struct job*)malloc(sizeof(struct job));
        new_job->id = id++;
        new_job->length = length;
        new_job->next = NULL;
        
        if (!head) {
            head = tail = new_job;
        } else {
            tail->next = new_job;
            tail = new_job;
        }
    }

    fclose(file);
    return head;
}

void free_jobs(struct job *head) {
    struct job *current = head;
    while (current) {
        struct job *temp = current;
        current = current->next;
        free(temp);
    }
}

void fifo_scheduler(struct job *head) {
    printf("Execution trace with FIFO:\n");

    int time = 0;
    struct job *current = head;
    float total_response = 0, total_turnaround = 0, total_wait = 0;
    int count = 0;

    while (current) {
        printf("Job %d ran for : %d\n", current->id, current->length);

        current->response_time = time;
        current->turnaround_time = time + current->length;
        current->wait_time = time;

        total_response += current->response_time;
        total_turnaround += current->turnaround_time;
        total_wait += current->wait_time;
        count++;

        time += current->length;
        current = current->next;
    }

    printf("End of execution with FIFO.\n");
    printf("Begin analyzing FIFO:\n");

    current = head;
    while (current) {
        printf("Job %d -- Response time: %d Turnaround: %d Wait: %d\n",
               current->id, current->response_time, current->turnaround_time, current->wait_time);
        current = current->next;
    }

    printf("Average -- Response: %.2f Turnaround: %.2f Wait: %.2f\n",
           total_response / count, total_turnaround / count, total_wait / count);
    printf("End analyzing FIFO.\n");

    free_jobs(head);
}

void sjf_scheduler(struct job *head) {
    printf("Execution trace with SJF:\n");

    int count = 0;
    struct job *temp = head;
    
    while (temp) {
        count++;
        temp = temp->next;
    }

    struct job **job_array = (struct job**)malloc(count * sizeof(struct job*));
    temp = head;
    for (int i = 0; i < count; i++) {
        job_array[i] = temp;
        temp = temp->next;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (job_array[i]->length > job_array[j]->length) {
                struct job *swap = job_array[i];
                job_array[i] = job_array[j];
                job_array[j] = swap;
            }
        }
    }

    int time = 0;
    float total_response = 0, total_turnaround = 0, total_wait = 0;

    for (int i = 0; i < count; i++) {
        struct job *current = job_array[i];
        printf("Job %d ran for : %d\n", current->id, current->length);

        current->response_time = time;
        current->turnaround_time = time + current->length;
        current->wait_time = time;

        total_response += current->response_time;
        total_turnaround += current->turnaround_time;
        total_wait += current->wait_time;

        time += current->length;
    }

    printf("End of execution with SJF.\n");
    printf("Begin analyzing SJF:\n");

    for (int i = 0; i < count; i++) {
        printf("Job %d -- Response time: %d Turnaround: %d Wait: %d\n",
               job_array[i]->id, job_array[i]->response_time, job_array[i]->turnaround_time, job_array[i]->wait_time);
    }

    printf("Average -- Response: %.2f Turnaround: %.2f Wait: %.2f\n",
           total_response / count, total_turnaround / count, total_wait / count);
    printf("End analyzing SJF.\n");

    free(job_array);
    free_jobs(head);
}

void rr_scheduler(struct job *head, int time_slice) {
    printf("Execution trace with RR:\n");

    int time = 0;
    struct job *queue = NULL, *tail = NULL, *current = head;
    struct job **job_tracker; 
    int job_count = 0;

    struct job *temp = head;
    while (temp) {
        job_count++;
        temp = temp->next;
    }

    job_tracker = (struct job**)malloc(job_count * sizeof(struct job*));
    int *original_lengths = (int*)malloc(job_count * sizeof(int));  // Store original lengths
    int index = 0;
    
    while (current) {
        current->response_time = -1;  
        job_tracker[index] = current; 
        original_lengths[index] = current->length;  
        index++;

        struct job *next_job = current->next;
        current->next = NULL;

        if (!queue) {
            queue = tail = current;
        } else {
            tail->next = current;
            tail = current;
        }
        current = next_job;
    }

    float total_response = 0, total_turnaround = 0, total_wait = 0;
    int completed_jobs = 0;

    while (queue) {
        struct job *current = queue;
        queue = queue->next;  

        if (current->response_time == -1) {
            current->response_time = time;  
        }

        int run_time = (current->length > time_slice) ? time_slice : current->length;
        printf("Job %d ran for : %d\n", current->id, run_time);
        current->length -= run_time;
        time += run_time;

        if (current->length > 0) {
            
            if (!queue) {
                queue = tail = current;
            } else {
                tail->next = current;
                tail = current;
            }
            tail->next = NULL;
        } else {
            
            current->turnaround_time = time;
            int original_length = original_lengths[current->id]; 
            current->wait_time = current->turnaround_time - original_length;

            total_response += current->response_time;
            total_turnaround += current->turnaround_time;
            total_wait += current->wait_time;
            completed_jobs++;
        }
    }

    printf("End of execution with RR.\n");
    printf("Begin analyzing RR:\n");

    for (int i = 0; i < job_count; i++) {
        struct job *job = job_tracker[i];
        printf("Job %d -- Response time: %d Turnaround: %d Wait: %d\n",
               job->id, job->response_time, job->turnaround_time, job->wait_time);
    }

    printf("Average -- Response: %.2f Turnaround: %.2f Wait: %.2f\n",
           total_response / job_count, total_turnaround / job_count, total_wait / job_count);
    printf("End analyzing RR.\n");

    free(job_tracker);
    free(original_lengths); 
    free_jobs(head);
}


int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <policy> <workload_file> [time_slice]\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct job *job_list = read_workload(argv[2]);

    if (strcmp(argv[1], "FIFO") == 0) {
        fifo_scheduler(job_list);
    } else if (strcmp(argv[1], "SJF") == 0) {
        sjf_scheduler(job_list);
    } else if (strcmp(argv[1], "RR") == 0) {
        int time_slice = atoi(argv[3]);
        rr_scheduler(job_list, time_slice);
    }

    return EXIT_SUCCESS;
}

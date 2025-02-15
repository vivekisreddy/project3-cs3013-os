#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declare struct job
struct job;

// Function prototype for analyze_performance
void analyze_performance(struct job *head);

// Job struct representing each job in the workload
struct job {
    int id;
    int length;
    int response_time;
    int turnaround_time;
    int wait_time;
    struct job *next;
};

// Function to read workload from a file
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

// Free allocated memory for jobs
void free_jobs(struct job *head) {
    struct job *current = head;
    while (current) {
        struct job *temp = current;
        current = current->next;
        free(temp);
    }
}

// FIFO Scheduling
void fifo_scheduler(struct job *head) {
    printf("Execution trace with FIFO:\n");

    int time = 0;
    struct job *current = head;

    while (current) {
        printf("Job %d ran for: %d\n", current->id, current->length);
        current->response_time = time;
        current->turnaround_time = time + current->length;
        current->wait_time = time;
        time += current->length;
        current = current->next;
    }

    printf("End of execution with FIFO.\n");
    analyze_performance(head);
}

// SJF Scheduling
void sjf_scheduler(struct job *head) {
    printf("Execution trace with SJF:\n");

    int count = 0;
    struct job *temp = head;

    // Count the total number of jobs
    while (temp) {
        count++;
        temp = temp->next;
    }

    // Allocate memory for the job array
    struct job **job_array = (struct job**)malloc(count * sizeof(struct job*));
    temp = head;
    for (int i = 0; i < count; i++) {
        job_array[i] = temp;
        temp = temp->next;
    }

    // Sort jobs by their length (Shortest Job First)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (job_array[i]->length > job_array[j]->length || 
                (job_array[i]->length == job_array[j]->length && job_array[i]->id > job_array[j]->id)) {
                struct job *swap = job_array[i];
                job_array[i] = job_array[j];
                job_array[j] = swap;
            }
        }
    }

    int time = 0;
    for (int i = 0; i < count; i++) {
        struct job *current = job_array[i];
        printf("Job %d ran for: %d\n", current->id, current->length);
        current->response_time = time;  // Response time is when the job starts
        current->turnaround_time = time + current->length;  // Turnaround time is when the job ends
        current->wait_time = time;  // Wait time is when it starts (assuming no arrival times)
        time += current->length;
    }

    printf("End of execution with SJF.\n");

    // After executing jobs, analyze the performance (response time, turnaround time, wait time, averages)
    analyze_performance(head);

    free(job_array);
}

// RR Scheduling
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
        current->response_time = -1;  // Default to -1 before being assigned a response time
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

    // Process the queue using RR scheduling
    while (queue) {
        struct job *current = queue;
        queue = queue->next;

        if (current->response_time == -1) {
            current->response_time = time;  // Set response time for first execution
        }

        int run_time = (current->length > time_slice) ? time_slice : current->length;
        printf("Job %d ran for: %d\n", current->id, run_time);

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
        }
    }

    printf("End of execution with RR.\n");
    analyze_performance(head);

    free(job_tracker);
    free(original_lengths);
}

// Function to analyze the performance metrics for all jobs
void analyze_performance(struct job *head) {
    // Begin analyzing SJF
    printf("Begin analyzing SJF:\n");

    // Loop through the jobs to display the calculated performance metrics
    struct job *current = head;
    while (current) {
        printf("Job %d -- Response time: %d Turnaround: %d Wait: %d\n", 
            current->id, current->response_time, current->turnaround_time, current->wait_time);
        current = current->next;
    }

    // Calculate and print average metrics
    float total_response_time = 0, total_turnaround_time = 0, total_wait_time = 0;
    int job_count = 0;
    current = head;
    while (current) {
        total_response_time += current->response_time;
        total_turnaround_time += current->turnaround_time;
        total_wait_time += current->wait_time;
        job_count++;
        current = current->next;
    }

    printf("Average -- Response: %.2f Turnaround: %.2f Wait: %.2f\n", 
           total_response_time / job_count, 
           total_turnaround_time / job_count, 
           total_wait_time / job_count);

    // End analyzing SJF
    printf("End analyzing SJF.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <policy> <workload_file> [time_slice]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *policy = argv[1];
    const char *workload_file = argv[2];
    int time_slice = argc == 4 ? atoi(argv[3]) : 0;  // Default to 0 if no time_slice is provided

    struct job *head = read_workload(workload_file);

    if (strcmp(policy, "FIFO") == 0) {
        fifo_scheduler(head);
    } else if (strcmp(policy, "SJF") == 0) {
        sjf_scheduler(head);
    } else if (strcmp(policy, "RR") == 0) {
        rr_scheduler(head, time_slice);
    } else {
        fprintf(stderr, "Unknown policy: %s\n", policy);
        free_jobs(head);
        return EXIT_FAILURE;
    }

    free_jobs(head);
    return EXIT_SUCCESS;
}
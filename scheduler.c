#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Job struct representing each job in the workload
struct job {
    int id;
    int length;
    struct job *next;
};

// Function to read jobs from the workload file into a linked list
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

// Function to free the job list
void free_jobs(struct job *head) {
    struct job *current = head;
    while (current) {
        struct job *temp = current;
        current = current->next;
        free(temp);
    }
}

// Function to execute jobs using FIFO scheduling
void fifo_scheduler(struct job *head) {
    printf("Execution trace with FIFO:\n");
    struct job *current = head;
    while (current) {
        printf("Job %d ran for : %d\n", current->id, current->length);
        current = current->next;
    }
    printf("End of execution with FIFO.\n");
    free_jobs(head);
}

// Function to execute jobs using SJF scheduling
void sjf_scheduler(struct job *head) {
    printf("Execution trace with SJF:\n");

    // Count jobs
    int count = 0;
    struct job *temp = head;
    while (temp) {
        count++;
        temp = temp->next;
    }

    // Copy jobs to an array for sorting
    struct job **job_array = (struct job**)malloc(count * sizeof(struct job*));
    temp = head;
    for (int i = 0; i < count; i++) {
        job_array[i] = temp;
        temp = temp->next;
    }

    // Sort jobs by length (Shortest Job First)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (job_array[i]->length > job_array[j]->length) {
                struct job *swap = job_array[i];
                job_array[i] = job_array[j];
                job_array[j] = swap;
            }
        }
    }

    // Execute jobs in sorted order
    for (int i = 0; i < count; i++) {
        printf("Job %d ran for : %d\n", job_array[i]->id, job_array[i]->length);
    }
    printf("End of execution with SJF.\n");

    free(job_array);
    free_jobs(head);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <policy> <workload_file> 0\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct job *job_list = read_workload(argv[2]);

    if (strcmp(argv[1], "FIFO") == 0) {
        fifo_scheduler(job_list);
    } else if (strcmp(argv[1], "SJF") == 0) {
        sjf_scheduler(job_list);
    } else {
        fprintf(stderr, "Unknown scheduling policy: %s\n", argv[1]);
        free_jobs(job_list);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

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

// Function to execute jobs using FIFO scheduling
void fifo_scheduler(struct job *head) {
    printf("Execution trace with FIFO:\n");
    struct job *current = head;
    while (current) {
        printf("Job %d ran for : %d\n", current->id, current->length);
        struct job *temp = current;
        current = current->next;
        free(temp);  // Free memory after execution
    }
    printf("End of execution with FIFO.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 4 || strcmp(argv[1], "FIFO") != 0) {
        fprintf(stderr, "Usage: %s FIFO <workload_file> 0\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    struct job *job_list = read_workload(argv[2]);
    fifo_scheduler(job_list);
    
    return EXIT_SUCCESS;
}

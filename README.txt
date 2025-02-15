Assignment 3 
Vivek Reddy Kasireddy & Jahnavi Prudhivi

Project Overview
This project implements a job scheduling simulator using three different scheduling policies: FIFO (First In First Out), SJF (Shortest Job First), and RR (Round Robin). The project reads a list of jobs from various workload files, simulates job execution under these policies, and calculates key performance metrics for each job, such as response time, turnaround time, and wait time. The performance of each policy is analyzed and averaged across all jobs.

Data Structures Used
The core data structure used for this project is a linked list of struct job. Each job contains the following fields:

id: Unique identifier for the job.
length: The burst time of the job (i.e., how long the job needs to execute).
response_time: The time at which the job starts execution.
turnaround_time: The total time the job takes from its arrival to completion.
wait_time: The time the job spends waiting in the queue before execution.
next: A pointer to the next job in the linked list.
Algorithm for Scheduling Policies
FIFO Scheduling (First In First Out):

The jobs are executed in the order they are read from the input file. Each job runs to completion before the next job begins.
The response_time is set as the current time when a job starts, turnaround_time is computed as the time the job finishes, and wait_time is calculated as the time the job has been waiting in the queue.
SJF Scheduling (Shortest Job First):

Jobs are sorted by their burst time (ascending order). If two jobs have the same burst time, they are ordered by their job ID.
The response_time, turnaround_time, and wait_time are then calculated for each job in the sorted list.
RR Scheduling (Round Robin):

Jobs are processed in a cyclic manner, with each job receiving a fixed "time slice" to run. If a job is not completed within its time slice, it is re-added to the queue with its remaining burst time.
The response_time, turnaround_time, and wait_time are updated as the jobs are processed.
Workload Files
The project uses five different workload files, each containing a set of job burst times. These files provide input to the job scheduling simulator and allow for testing under different job conditions. Below are the contents and a brief explanation of each workload file:

workload1.in:
3
6
9
12
15
This workload file contains five jobs with burst times: 3, 6, 9, 12, and 15 units. It represents a scenario with a moderate spread of burst times.
workload2.in:
1
2
3
10
10
This workload file contains five jobs with burst times: 1, 2, 3, 10, and 10 units. This scenario tests the system's handling of small burst jobs as well as jobs with equal burst times (10).
workload3.in:
2
3
5
7
8
This workload file contains five jobs with burst times: 2, 3, 5, 7, and 8 units. It represents a scenario where jobs have varied but relatively small burst times.
workload4.in:
50
70
120
This workload file contains three jobs with burst times: 50, 70, and 120 units. It represents a scenario with larger, more time-consuming jobs.
workload5.in:
1
10
3
This workload file contains three jobs with burst times: 1, 10, and 3 units. It represents a scenario with a mix of very small and large jobs.
Test Failures and Issues
Test 15 (SJF Scheduling)
In Test 15, the SJF (Shortest Job First) policy failed to provide the expected output because the order of execution and the way the output was printed were not correctly handled. After sorting the jobs by their burst time, the jobs were executed in the wrong order, causing inaccurate calculation of the response_time, turnaround_time, and wait_time. This issue arose from improper handling of the sorted array and linked list during the execution trace and performance analysis.

Test 17 (RR Scheduling)
In Test 17, the RR (Round Robin) policy encountered a similar issue. The jobs were processed in the correct order, but the output was printed in a confusing manner due to the dynamic modification of the queue while jobs were executed. As jobs were re-added to the queue, the output did not match the expected execution trace, leading to incorrect response_time and turnaround_time for some jobs. The performance metrics were calculated based on the updated queue rather than the original job order, which caused discrepancies in the analysis output.

Key Lessons Learned
Correct Handling of Linked List and Sorted Array:

After sorting jobs in SJF, the order in which jobs are processed must be carefully managed. 
It is essential to preserve the sorted order for accurate performance analysis.
For RR, care should be taken when jobs are re-added to the queue to ensure the correct job order and execution trace.
Printing Output in Correct Order:

The execution trace output should be printed in the order in which jobs actually run. 
For RR, when a job is split into multiple rounds, the time and order of execution need to be carefully printed 
to match the expected results.

Conclusion
This project has provided valuable insights into job scheduling algorithms and their performance analysis. 
The issues encountered in Test 15 and Test 17 have highlighted the importance of correctly managing the job 
execution order, especially when jobs are modified during the scheduling process. Future improvements can focus 
on ensuring that job execution and output printing are handled in a more structured manner to prevent these issues.
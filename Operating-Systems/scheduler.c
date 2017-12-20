/**
 * Author: hari-007 (Hari)
 * Description: Round Robin & FCFS scheduling simmulation functionality, based given bursttime from input file
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>

/* Process Node Structure */
typedef struct process {
	int pid, burst_time, tat, tmp_bt;
	struct process *next;
} process;

/* prototypes for entire input functionality */
process *new_list(int burst_time, int pid);
int read_input(char **argv,  process **head, process **tail);
void link_new_process(process **head, process **tail,int burst_time, int id);
void fcfs_schedule(process **head, int list_count_result);
int check_for_complete(int a[], int n);
void round_robin_schedule(process **head, int list_count_result);
int check_input_args(int argc, char **argv);
void print_req_input();
void free_circular_list(process **head, int list_count_result);

int main(int argc, char **argv) {
	
	/* Here checks for Program execution step arguments */
	int input_result = check_input_args(argc, argv);
	if( input_result != -1) {

		process *head, *tail;
		head = NULL;
		
		/* gets total number of process burst time / read_input function status for success or error */
		int list_count_result = read_input(argv, &head, &tail); 
		if(list_count_result > 0) {
			/* if user provded option as 0 -> FCFS scheduling / 1-> Round Robin scheduling execution */
			if(input_result == 0)  
				fcfs_schedule(&head, list_count_result);
			else 
				round_robin_schedule(&head, list_count_result);

				/* Clearing allocated memory in heap once operation completed */
				free_circular_list(&head, list_count_result);
		} else if(list_count_result == -1) {
			printf("Unable to read File. Please check given input file & try again! \n");
		} else {
			printf("Input FIle doesn't contain any data to perform!! \n");
		}
	} else {
		print_req_input();
	}	
	return 0;
}

/* Macro function for creating a new Process structure node and returning its address */
process *new_list(int burst_time, int pid) {
	process *node = malloc(sizeof *node);

	if(node) {
		node->burst_time = burst_time;
		node->tmp_bt = burst_time;
		node->pid = pid;
		node->next = node;
	} else {
		printf("Unable to create Lined list. Malloc failed!! \n");
        exit(-1);
	}

	return node;
}

/* Reading input from given File wth argument of programm */
int read_input(char **argv, process **head, process **tail) {

	if(access(argv[2], R_OK ) != -1) {
		int  i = 0, bt;
		FILE *input_file = fopen(argv[2],"r");
		
		while(fscanf(input_file,"%d", &bt) != EOF) {
			/* Each burst time is pushing to List of Processes */ 
			link_new_process(&(*head), &(*tail), bt, i);
			++i;
		}
		return i;
	} else {
		return -1;
	}
}

/* Creates Circular Linked List with insert at end of each process information */
void link_new_process(process **head, process **tail, int burst_time, int id) {
	
	process *temp = new_list(burst_time, id);
	
	/* For First Link of 1st Process insertion */
	if(*head == NULL) {
		*head = *tail = temp;
		(*head)->next = (*tail)->next = *head;
	} /* For Second Link of 2nd insertion */
	else if((*head)->next == *head) {
		temp->next = *head;
		(*head)->next = temp;
		*tail = temp;
	} /* For nth Link of nth Insertion */ 
	else {
		temp->next = *head;
		(*tail)->next = temp;
		*tail = temp;
	}
}

/* FCFS Scheduling implementation for burst time & wait time of All processes arrives at same time*/
void fcfs_schedule(process **head, int list_count_result) {
	process *current = *head;
	int count = 0;
	float sum_tat, sum_wat;
	int prev_tat_time, turn_time, wait_time;

	/* breaks for total traversal of n numbered process nodes */
	while(count < list_count_result) {
		/* Identifying 1st process to make wait time as 0 */
		if(current->pid == 0) {
			prev_tat_time = turn_time = current->burst_time;
			wait_time = 0;
		} /* For all process Turn Around Time & Wait Time caluclation */ 
		else {
			turn_time = prev_tat_time + current->burst_time;
			wait_time = prev_tat_time;
			prev_tat_time = turn_time;
		}
		/* Final total evaluation for both times */
		sum_tat += turn_time; 
		sum_wat += wait_time;

		printf("p%d ",current->pid);
		current = current->next;
		++count;
	}

	printf("\nAverage wait time: %f \n", sum_wat / count);
	printf("\nAverage turnaround time: %f \n", sum_tat / count);
}

/* Round Robin Scheduling implementation for burst time & wait time of All processes arrives at same time*/
void round_robin_schedule(process **head, int list_count_result) {
	process *current = *head, *temp = *head;
	float sum_tat, sum_wat;
	int completed_process[list_count_result], total_time = 0, count = 0;

	/* Array of process done state evaluation for break conition on below loop */
	for(int i = 0; i < list_count_result; i++)
		completed_process[i] = 0;

	while(1) {
		if(current->tmp_bt == 0) {
			completed_process[current->pid] = 1;

			/* checks if all the process are done their timeSlice then breaks the loop */
			int rr = check_for_complete(completed_process, list_count_result);
			if(rr) {
				break;
			}
		} else {
			/* For any process timeslice less than 10 */
			if(current->tmp_bt < 10) {
				total_time += current->tmp_bt;
				current->tmp_bt = 0;
			} /* For any process with greater burst time than 10 to do time slice */
			else {
				total_time += 10;
				current->tmp_bt -=10;
			}

			current->tat = total_time;
			printf("p%d ",current->pid);			
		} 

		/* traversing through all over the list */
		current = current->next;
	}

	/* Caluclating total turn around & wait times for above round robin processes */
	while(count < list_count_result) {
		sum_tat += temp->tat;
		sum_wat += temp->tat - temp->burst_time;
		temp = temp->next;
		++count;
	}

	printf("\nAverage wait time: %f \n", sum_wat / list_count_result);
	printf("\nAverage turnaround time: %f\n", sum_tat / list_count_result);
}

/* here checks all processes burst time became 0 */
int check_for_complete(int a[], int n)
{ 
	while(--n>-1 && a[n] == 1);
    return n == -1;
}

/* Here checks for all possible user input arguments acceptance */ 
int check_input_args(int argc, char **argv) {
	return (argc == 3 && isdigit(*argv[1]) && strlen(argv[1]) == 1 && (*argv[1] -'0' == 0 ||  *argv[1] - '0' == 1)) ? *argv[1] - '0':-1;
}

/* Usuage Message to user for wrong input arguments */ 
void print_req_input() {
	printf("Usage: ./schedule <option> <filename>\nAvailable options: \n\t 0: FCFS \n\t 1: Round robin\n");
} 


/* Clear allocated memory of linked list in memory once the operation completed */
void free_circular_list(process **head, int list_count_result) {
	int count = 0;
	process *temp;
	while(count < list_count_result) {
		temp = *head;
		*head = (*head)->next;
		free(temp);
		++count;
	}
}

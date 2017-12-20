/*
 *  Author: hari-007 (Hari)
 *  Description: Creation of Custom shell with given conditions accepted as below:
 *               1. Shell should continue in a loop of its functionality till 'exit' and start by 'run' from input command
 *               2. Shell should halt parent process untill executes given one / many command[s] by child processes
 *               3. Shell should return obtained error number from child to show appropriate error messages
 */

#include <stdio.h>
#include <string.h>

#include <unistd.h> 
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>

/* Input Buffer to get User input multiple commands */
#define MAX_INPUT 255

int myshell_input();
void execute_command(int i, char *s);
int is_run(char *s);
int is_exit(char *s);


int main() {
    
    int status = 1;

    /* Infinite execution feature with do while */
    do {
        status = myshell_input();
    } while(status);

    return 0;
}

/* Isolated functionality to get & process User Input Commands */
int myshell_input() {
    int status = 1;
    char input[MAX_INPUT];
    
    printf("myshell> ");

    /* To read user input and stores in buffer */
    if(fgets(input, MAX_INPUT, stdin) != NULL) { 

        if(!strcmp(input, "\n") || !strcmp(input, " \n")) {
            goto done;
        }

        char *commands[20], *delimit = ";\n";
        int i = 0;

        /* Tokenizing given commands into words for evaluation */
        commands[i]=strtok(input,delimit);
        
        /* check for EXIT option in command */
        if(is_exit(commands[i])) {
            status = 0;
            goto done;
        } 

        /* check for RUN option in command */
        else if(is_run(commands[i])) {           

            while(commands[i]) {
                // Run whatever command we have from 2nd onwards
                execute_command(i, commands[i]);

                commands[++i]=strtok(NULL,delimit);
            }
                
            status = 1;
            goto done;
        } 
        // Not a valid command case
        else {
            printf("myshell: %s is not a valid command \n", strtok(input," "));
            status = 1;
            goto done;
        } 
    } else {
        goto done;
    }

    /* Return appropriate status to run / exit shell functionality to main*/
    done:
        return status;
}

void execute_command(int argc, char *s) {

    int status, i;
    char *command[10];
    
    pid_t child_id = fork();

    /* Parent Process block of functionality */
    if(child_id > 0) {
        printf("myshell: started child pid %d \n",child_id);
        
        /* hold the parent till child exited with status */
        wait(&status);

        /*  // Extra work to check syscall status returned from child to parent in pipe
        if(WIFEXITED(status))
            printf("myshell: child exited with errno of = %d, status = %d \n", WEXITSTATUS(status), status); 
        */  

    /* Child Process block of functionality */    
    } else if(child_id == 0) {
        i =0;

        /* Read the first word with spaced delimited Input in buffer */
        command[i] = strtok(s, " ");
        if(argc == 0) {
            command[0] = strtok(NULL, " ");
        }

        /* Read all the trailled Input from Buffer */
        while(command[i]) {
            command[++i] = strtok(NULL, " ");
        }
        
        /* System call execution of Linux commands from C library function */
        status = execvp(command[0], command);

        if(status == -1) {
            printf("myshell: %s %s \n",strerror(errno), command[0]);
        }
        exit(errno);
    /* Uncertain condition to handle child process creation failed error */
    } else {
        printf("myshell: Unable to create a child process, please try again \n ");
    }
}

/* Check whether given first input commands has 'run' */
int is_run(char *s) {
    char *result = strstr(s, "run");
    return result - s == 0 ? 1:0;
}

/* Check whether given first input commands has 'exit' */
int is_exit(char *s) {
    char *result = strstr(s, "exit");
    return result - s == 0 ? 1:0;
}

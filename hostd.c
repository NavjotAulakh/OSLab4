/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "queue.h"
#include "utility.h"
#include "hostd.h"

// Put macros or constants here using #define
#define MEMORY 1024

// Put global environment variables here
node_t *dispatch = NULL, *real = NULL, *p2 = NULL, *p3 = NULL, *p4 = NULL;
/*
int dispacherTime;
dispatcherTime = 0;
int dispacherNodes;
dispacherNodes = 0;
*/

void queue_up(node_t *queue);
int execute_process(proc proc);
// Define functions declared in hostd.h here

void queue_up(node_t *queue)
{
    node_t * current = queue->next; //skip the head

    while (current != NULL) {
        if (current->process.priority == 0)
            push(real, current->process);
        if (current->process.priority == 1)
            push(p2, current->process);
        if (current->process.priority == 2)
            push(p3, current->process);
        if (current->process.priority == 3)
            push(p4, current->process);
        //simply sorting them by priority
        current = current->next;

    }
}

int execute_process(proc proc)
{
	int status;
	pid_t pid;
    char *arg[256];

	if (!proc.suspended) { //check if it was suspended or not

		pid = fork(); //fork the child
		if (pid == 0) 
        { //CHILD
			execv("./process", arg);
			exit(0);
		} 

        else if (pid < 0) //FORK FAILED
			status = -1;
	
        else 
        { //PARENT

			proc.pid = pid; //set pid
			print_res(proc); //print stats about current process

			if (proc.priority == 0) { //realtime, complete first
				sleep(proc.duration); //sleep for the runtime
				kill(pid, SIGINT);
				waitpid(proc.pid, &status, WUNTRACED);
                printf("TERMINATED PID %d\n", proc.pid);
				free_mem(proc); //deallocate the memory, does not use resources

			} 
            
            else //not realtimes
            {
				sleep(1);
				proc.duration--; //decrease the runtime
				kill(pid, SIGTSTP); //suspend the operation
                proc.suspended = 1; //set int bool thing

				//shift to lower queue
            if (proc.priority == 1 || proc.priority == 2)
            {
                proc.priority++;
                if (proc.priority == 1)
                {
                    push(p3, proc);
                }
                if (proc.priority == 2)
                {
                    push(p4, proc);
                }

            }
            if (proc.priority == 3)
                {
                    push(p4, proc);
                }

			}

		}
	} //end of IF NOT SUSPENDED PROC 
    
    else 
    {
		print_res(proc); //spit out stats

		kill(proc.pid, SIGCONT); //signal the process to continue
		sleep(1);
		kill(proc.pid, SIGTSTP); //suspend

		if (proc.duration <= 1) { //kill process if finished
			kill(proc.pid, SIGINT);
			waitpid(proc.pid, &status, WUNTRACED);
            printf("TERMINATED PID %d\n", proc.pid);
			free_mem(proc); //deallocate the used memory
			free_res(proc); //free the resources

		}

        else 
        { //if still runtime left

			proc.duration--; //decrement runtime
            //add to the next list
            if (proc.priority == 1 || proc.priority == 2)
            {
                proc.priority++;
                if (proc.priority == 1)
                {
                    push(p3, proc);
                }
                if (proc.priority == 2)
                {
                    push(p4, proc);
                }

            }
            if (proc.priority == 3)
                {
                    push(p4, proc);
                }
		}

	}//end SUSPENDED block
   
}


int main(int argc, char *argv[])
{
    dispatch = malloc(sizeof(node_t));
 	dispatch->next = NULL; 
    
    real = malloc(sizeof(node_t));
 	real->next = NULL;

    p2 = malloc(sizeof(node_t));
 	p2->next = NULL;

    p3 = malloc(sizeof(node_t));
 	p3->next = NULL;

    p4 = malloc(sizeof(node_t));
 	p4->next = NULL;
     //setting up queues, freeing memory

    avail_res.printers = 2;
    avail_res.scanners = 1;
    avail_res.cds = 2;
    avail_res.modems = 1;
    //set up resources

    // Load the dispatchlist
    load_dispatch("dispatchlist", dispatch);
    //load_dispatch(argv[1], dispatch); //uncomment to work with the execute line ./hostd dispatchlist
    printf("DISPATCHING\n");
    queue_up(dispatch); //split everything up by priority
    
    node_t * current = real->next; //realtime first

    //below, we check our memory and execute the processes

    while (current != NULL) {
        if (alloc_mem(&current->process) && alloc_res(current->process)) //allocate
            execute_process(current->process);
        current = current->next;
    }

     current = p2->next; //priority 1

    while (current != NULL) {
        if (alloc_mem(&current->process) && alloc_res(current->process))
            execute_process(current->process);
        else
        {
          //  printf("NOT ENOUGH RESOURCES\n");
            push(p3, current->process); //push to end of lower
            pop(p2); //remove from current
        }
        current = current->next;
    } 

    current = p3->next; //priority 2

    while (current != NULL) {
        if (alloc_mem(&current->process) && alloc_res(current->process))
            execute_process(current->process);
        else
        {
           // printf("NOT ENOUGH RESOURCES\n");
            push(p4, current->process); //push to end of lower
            pop(p3); //remove from current
        }
        current = current->next;
    } 

    current = p4->next; //priority 3

    int c = 0;
    while (current != NULL) {
        if (alloc_mem(&current->process) && alloc_res(current->process))
            execute_process(current->process);
        else
        {
            //printf("NOT ENOUGH RESOURCES\n");
            push(p4, current->process); //push to end
            pop(p4); //remove from current
        }
        current = current->next;
        c++;
        if (c > 2000)
        {
            break;
        }
    }

        sleep(4); //to finish prints and all
        printf("COMPLETE.\n");
 
    return EXIT_SUCCESS;
}
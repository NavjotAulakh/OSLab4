/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
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
int dispatcherTime= 0;
int dispatcherNodes = 0;

// Define functions declared in hostd.h here

int main(int argc, char *argv[])
{
    // ==================== YOUR CODE HERE ==================== //
	proc tempProcess;
	resources res_avail;
	res_avail.printers = 2;
	res_avail.scanners = 1;
	res_avail.modem = 1;
	res_avail.drives = 2;
    node_q * priority1 = NULL;
	node_q * priority2 = NULL;
    node_q * priority3 = NULL;
    node_q * realTime = NULL;
	node_q * jobDispatcher = NULL;
	node_q * userJobQueue = NULL;

	jobDispatcher = malloc(sizeof(node_q));
    realTime = malloc(sizeof(node_q));
	userJobQueue = malloc(sizeof(node_q));
	priority1 = malloc(sizeof(node_q));
    priority2 = malloc(sizeof(node_q));
    priority3 = malloc(sizeof(node_q));
	
	// Initalise memory to 0
	for(int i = 0; i < MEMORY; i++){
		res_avail.available_memory[i] = 0;
	}

    // Load the dispatchlist
    FILE *fp = fopen("dispatchlist", "r");
	if(fp == NULL)
    {
		fprintf(stderr, "FILE OPEN ERROR \n");
		exit(1);
	}
	else
	{
		
	}
	
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    // Add each process structure instance to the job dispatch list queue
    while ((read = getline(&line, &len, fp)) != -1) 
	{
		tempProcess.pid = 0;

		char *token = NULL;
		token = strtok(line,",\n");
		tempProcess.arrivalTime = atoi(token);
		
		token = strtok(NULL,",\n");
		tempProcess.priority = atoi(token);

		token = strtok(NULL,",\n");
		tempProcess.duration = atoi(token);

		token = strtok(NULL,",\n");
		tempProcess.memory = atoi(token);

		token = strtok(NULL,",\n");
		tempProcess.printers = atoi(token);
		
		token = strtok(NULL,",\n");
		tempProcess.scanners = atoi(token);

		token = strtok(NULL,",\n");
		tempProcess.modem = atoi(token);

		token = strtok(NULL,",\n");
		tempProcess.drives = atoi(token);

		
		push(tempProcess, jobDispatcher);
		dispatcherNodes += 1;
	}

	fclose(fp);
	
	while(1)
	{
		dispatcherTime += 1;
		
		// Iterate through each item in the job dispatch list, add each process
		// to the appropriate queues
		if (jobDispatcher->next != NULL)
		{
			
			int count;
			count = dispatcherNodes;
			
			while (count > 0)
			{
				
				proc * pProcess = pop(jobDispatcher);
				if (pProcess == NULL){
					break;
				}
				tempProcess = *pProcess;
				if (tempProcess.arrivalTime <= dispatcherTime)
				{
					
					if (tempProcess.priority == 0)
					{
						push(tempProcess, realTime);
						dispatcherNodes--;
					}
					else 
					{
						push(tempProcess, userJobQueue);
						dispatcherNodes--;
					}
				}
				else
				{
					push(tempProcess, jobDispatcher);
				}
				count-= 1;
				
			}
			
			
		}
		// Allocate the resources for each process before it's executed
		if (realTime->next == NULL)
		{
			
			if (userJobQueue->next != NULL)
			{
				
				tempProcess = *pop(userJobQueue);
				if (allocateMemory(res_avail.available_memory, tempProcess, MEMORY)){
		
					if(allocateResources(tempProcess, res_avail))
					{
		
						if(tempProcess.priority == 1)
						{
		
							push(tempProcess, priority1);
						}
						if(tempProcess.priority == 2)
						{
							
							push(tempProcess, priority2);
						}
						if(tempProcess.priority == 3)
						{
					
							push(tempProcess, priority3);
						}
					}
					else
					{
						
						push(tempProcess, userJobQueue);
					}
				}
				else
				{
		
					push(tempProcess, userJobQueue);
				}
			}
		}

		// Execute the process binary using fork and exec
		// Perform the appropriate signal handling / resource allocation and de-alloaction
		else
		{
	
			tempProcess = *pop(realTime);
			int pid = fork();
			int status;
			if (pid == 0) //child
			{
				tempProcess.pid = getpid();
				tempProcess.duration--;
				displayProcess(&tempProcess);
				execv("./process", argv);
				exit(0);
			}
			else // parent
			{
				sleep(tempProcess.duration);
				kill(pid, SIGTSTP);
				kill(pid, SIGINT);
				waitpid(pid, &status, 0);
			}
		}

		if (priority1->next == NULL)
		{
			if (priority2->next == NULL)
			{
				if (priority3->next == NULL)
				{
					if (jobDispatcher->next == NULL)
						exit(0);
				}
				else 
				{
					
					tempProcess = *pop(priority3);
					tempProcess = handleProcess(tempProcess, argv);
					if (tempProcess.duration > 0){
						tempProcess.priority = 3;
						push(tempProcess, priority3);
					}
					
					
				}
			}
			else 
			{
				tempProcess = *pop(priority2);
				tempProcess = handleProcess(tempProcess, argv);
				if (tempProcess.duration > 0){
					tempProcess.priority = 3;
					push(tempProcess, priority3);
				}
			}

		}
		else 
		{
			tempProcess = *pop(priority1);
			tempProcess = handleProcess(tempProcess, argv);
			if (tempProcess.duration > 0){
				tempProcess.priority = 2;
				push(tempProcess, priority2);
			}
		}

		// Repeat until all processes have been executed, all queues are empty
		
	}

    return EXIT_SUCCESS;
}

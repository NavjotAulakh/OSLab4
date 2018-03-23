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
int dispacherTime;
dispatcherTime = 0;
int dispacherNodes;
dispacherNodes = 0;

// Define functions declared in hostd.h here

int main(int argc, char *argv[])
{
    // ==================== YOUR CODE HERE ==================== //
	proc tempProcess;
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
	
    // Load the dispatchlist
    FILE *fp = fopen("dispachlist", "r");
	if(fp == NULL)
    {
		fprintf(stderr, "FILE OPEN ERROR \n");
		exit(1);
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
		strcpy(tempProcess.arrivalTime,token);
		
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
		dispacherNodes += 1;
	}

   

	// Iterate through each item in the job dispatch list, add each process
    // to the appropriate queues
	if (jobDispatcher->next != NULL)
	{
		int count;
		count = dispacherNodes;
		
		while (count > 0){
			
			tempProcess = *pop(jobDispatcher);
			if (tempProcess.arrivalTime <= dispacherTime)
			{
				if (tempProcess.priority == 0)
				{
					push(tempProcess, realTime);
				}
				else 
				{
					push(tempProcess, userJobQueue);
				}
			}

			else
			{
				push(tempProcess, jobDispatcher);
			}
		}
		
	}

	
	while(1)
	{
		// Allocate the resources for each process before it's executed
		
		// Execute the process binary using fork and exec

		// Perform the appropriate signal handling / resource allocation and de-alloaction

		// Repeat until all processes have been executed, all queues are empty
				
	}

    return EXIT_SUCCESS;
}

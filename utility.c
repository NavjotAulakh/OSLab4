/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "utility.h"

// Define your utility functions here, you will likely need to add more...
void displayProcess(proc * process)
{
 if (process != NULL)
  {
    printf("\n | Arrivial Time: %10d| Priority: %10d | pid: %10d | Runtime: %10d | RAM: %10d \n", 
    process->arrivalTime, process -> priority, process -> pid, process -> duration, process->memory);
  }
}

int allocateMemory(int *memory, proc tempProcess, int size){
	int allocated = 0;
	int start = 64;

	for(int i = 0; i < size; i++){
		if(memory[i] == 1){
			allocated = 0;
			start = i + 1;
		}
		else{
			allocated++;
		}
		if(allocated == tempProcess.memory){
			tempProcess.address = start;

			for(int k = 0; k < tempProcess.memory; k++){
				memory[k + tempProcess.address] = 1;
			}
			return 1;
		}
	}
	return 0;
}


void deallocateMemory(int *memory, proc tempProcess){
	for(int i = 0; i < tempProcess.memory;i++){
		memory[i + tempProcess.address] = 0;
	}
}

int allocateResources(proc tempProcess, resources rec){
	if((tempProcess.printers <= rec.printers) 
    & (tempProcess.scanners <= rec.scanners) 
    & (tempProcess.modem <= rec.modem) 
    & (tempProcess.drives <= rec.drives))
    {
       rec.printers -= tempProcess.printers;
       rec.scanners -= tempProcess.scanners;
       rec.modem -= tempProcess.modem;
       rec.drives -= tempProcess.drives;
       return 1;
    }
    else{
        return 0;
    }
}

void deallocateResources(proc tempProcess, resources rec)
{
    rec.printers += tempProcess.printers;
    rec.scanners += tempProcess.scanners;
    rec.modem += tempProcess.modem;
    rec.drives += tempProcess.drives;
}

proc handleProcess(proc tempProcess, char* arg[])
{
    if (tempProcess.pid != 0)
    {
	
        tempProcess.duration = tempProcess.duration - 1;
        kill(tempProcess.pid, SIGCONT);
        sleep(1);
        if (tempProcess.duration > 0 )
        {
            kill(tempProcess.pid, SIGTSTP);
        }
        else 
        {
            kill(tempProcess.pid, SIGINT);
        }  
        
    }
    else 
    {
        int pid;
		pid = fork();
        if (pid == 0) //child
        {
            tempProcess.pid = getpid();
			displayProcess(&tempProcess);
            execv("./process", arg);
            exit(0);
        }
        else // parent
        {
			tempProcess.pid = pid;
			tempProcess.duration = tempProcess.duration - 1;
            sleep(1);
            if (tempProcess.duration > 0 )
            {
                kill(pid, SIGTSTP);
              
            }
            else 
            {
                kill(pid, SIGINT);
                
            }  
        }
    }
	return tempProcess;
    
}
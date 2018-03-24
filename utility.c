/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"
#include "queue.h"
int avail_mem[MEMORY] = {0};

// Define your utility functions here, you will likely need to add more...
int alloc_res(proc p)
{
 	if (p.suspended) { //if suspended
 		return 1;
 	}
 	//Check if the resources needs to be allocated, and if there is enough resources to allocate.
 	if (p.res.printers <= avail_res.printers && p.res.scanners <= avail_res.scanners && p.res.modems <= avail_res.modems && p.res.cds <= avail_res.cds) { //if we have enough resources
            avail_res.printers = avail_res.printers - p.res.printers;
            avail_res.scanners = avail_res.scanners - p.res.scanners;
            avail_res.modems = avail_res.modems - p.res.modems;
            avail_res.cds = avail_res.cds - p.res.cds; //allocate 'em'
        return 1; //allocated
 	} 
    else 
 		return 0;
}

void free_res(proc p)
{
    avail_res.printers = avail_res.printers + p.res.printers;
    avail_res.scanners = avail_res.scanners + p.res.scanners;
    avail_res.modems = avail_res.modems +p.res.modems;
    avail_res.cds = avail_res.cds + p.res.cds; 
     //free 'em
}

 void free_mem(proc p) 
{
	for (int i = 0; i < p.memory; i++) {
		avail_mem[i + p.address] = 0;
	}
    //free memory
}

int alloc_mem(proc *p) 
{
	//already has allocated memory if suspended
	if (p->suspended)
		return 1; //allocated
	//sleep(1); // delay output, clean print
	int allocated = 0; //the number of allocated spots
	int start = 0; //the starting position of a valid address for a proc
	int a = 0; //counter thing

	 if (p->priority > 0) {
	 	a = 64;
	 	start = 64; //always keep 64 memory free, according to requirements
	 }
	//iterate over array and check each number for unallocated space
	 for (int i = a; i < MEMORY; i++) 
     {
	 	if (avail_mem[i] == 1) 
        {
	 		allocated = 0;
			start = i + 1; //reset the start address

		} 
        else 
			allocated++;

		//check if the total space is free
		if (allocated == p->memory) 
        {
			p->address = start; //set the address 

			//allocate the memory 
			for (int k = 0; k < p->memory; k++) 
				avail_mem[k + p->address] = 1;
			
			 return 1;
		}

	    }
	
		return 0; //failed to allocate
}

void print_res(proc p)
{

    printf("| Arrival: %d\t| processing time: %d \t| priority: %d \t| memory: %d \t| pid: %d \t| address: %d \t| RESOURCES | printers: %d \t| cds: %d \t| scanners: %d \t| modems: %d \t|\n", 
    p.arrivalTime, p.duration, p.priority, p.memory, p.pid, p.address, p.res.printers, p.res.cds, p.res.scanners, p.res.modems);

}

void print_queue(node_t *queue)
{
    node_t * current = queue->next; //skip the head

    while (current != NULL) {
        print_res(current->process); //iterate
        current = current->next;
    }
}

void load_dispatch(char *dispatch_file, node_t *queue)
{
    FILE *f;
    char *pt, buff[256] = {0};
    f = fopen(dispatch_file, "r");

    proc p;
    while (fgets(buff, sizeof(buff), f)) { 

        pt = strtok (buff,",");
        p.arrivalTime = atoi(pt);
 
        pt = strtok (NULL, ",");
        p.priority = atoi(pt);

        pt = strtok (NULL, ",");
        p.duration = atoi(pt);

        pt = strtok (NULL, ",");
        p.memory = atoi(pt);

        pt = strtok (NULL, ",");
        p.res.printers = atoi(pt);

        pt = strtok (NULL, ",");
        p.res.scanners = atoi(pt);

        pt = strtok (NULL, ",");
        p.res.modems = atoi(pt);

        pt = strtok (NULL, ",");
        p.res.cds = atoi(pt);
        
        p.suspended = 0;
        p.pid = 0;
        p.address = 0;
        //gather, split, and populate temp proc
        push(queue, p); //push to dispatcher 
        //print_res(p[lines]);

    }

    fclose(f);
}

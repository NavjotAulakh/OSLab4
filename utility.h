/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#ifndef UTILITY_H_
#define UTILITY_H_

// The amount of available memory
#define MEMORY 1024

// Resources structure containing integers for each resource constraint and an
// array of 1024 for the memory
typedef struct {
    int memory[MEMORY];
	int printers;
    int scanners;
    int modem;
    int drives;
} resources;


// Processes structure containing all of the process details parsed from the 
// input file, should also include the memory address (an index) which indicates
// where in the resources memory array its memory was allocated
typedef struct{
    int pid;
    int arrivalTime;
	int priority;
	int address;
	int duration;
	int memory;
	int printers;
    int scanners;
    int modem;
    int drives;
} proc;

typedef struct node {
   proc data; 
   struct node *next;
   struct node *prev;
}node_q;

// Include your relevant functions declarations here they must start with the 
// extern keyword such as in the following examples:

// Function to allocate a contiguous chunk of memory in your resources structure
// memory array, always make sure you leave the last 64 values (64 MB) free, should
// return the index where the memory was allocated at
// extern int alloc_mem(resources res, int size);

// Function to free the allocated contiguous chunk of memory in your resources
// structure memory array, should take the resource struct, start index, and 
// size (amount of memory allocated) as arguments
// extern free_mem(resources res, int index, int size);

// Function to parse the file and initialize each process structure and add
// it to your job dispatch list queue (linked list)
// extern void load_dispatch(char *dispatch_file, node_t *queue);

extern proc * pop(node_q *queue);
extern void push(proc process, node_q *queue);
extern void displayProcess(proc *pro);
extern void printList(node_q *list);
extern bool allocateMemory(int *memory, int size);
extern void deallocateMemory(int *memory);


#endif /* UTILITY_H_ */
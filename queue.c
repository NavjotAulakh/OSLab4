/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// Define your FIFO queue functions here, these will most likely be the
// push and pop functions that you declared in your header file

proc * pop(node_q * queue){
    proc tempProcess;

	if(queue -> next == NULL){
		return NULL;
	}
	node_q * nextNode = NULL;
	nextNode = queue -> next -> next;
	node_q * currentNode = NULL;
    currentNode = queue -> next;
 	tempProcess = currentNode -> process;
	queue -> next = nextNode;
    free(currentNode);

	return &tempProcess;
}

void push (proc process, node_q  * queue){
	node_q *current = queue;

	while(current -> next != NULL){
		current = current -> next;
	}

	current -> next =  (node_q *) malloc(sizeof(node_q));
	current -> next -> process = process;
    current->next->next = NULL;
	
}

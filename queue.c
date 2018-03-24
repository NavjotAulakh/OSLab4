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

node_t *push(node_t *head, proc process)
{
    node_t * current = head;
    while (current->next != NULL) {
        current = current->next; //find last node
    }

    /* now we can add a new variable */
    current->next = malloc(sizeof(node_t));
    current->next->process = process; //cofee-pasta process
    current->next->next = NULL;

    return current;
}

node_t *pop(node_t *head)
{
	node_t * next_node = NULL, *temp = NULL;

    if (head == NULL) {
        return temp; //if empty
    }

    next_node = (head)->next->next; //set to next 
    temp = (head);
     
    free(head->next); //free head
    head->next = next_node; //set head to next

    return temp;
}

/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#ifndef QUEUE_H_
#define QUEUE_H_

#include "utility.h"

// Include your relevant FIFO queue functions declarations here they must start 
// with the extern keyword such as in the following examples:
extern proc * pop(node_q *queue);
extern void push(proc process, node_q *queue);
extern void printList(node_q *list);

#endif /* QUEUE_H_ */
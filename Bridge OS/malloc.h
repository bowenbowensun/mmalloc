//
//  malloc.h
//  Bridge OS
//
//  Created by Bowen Sun on 11/14/18.
//  Copyright © 2018 Bowen Sun. All rights reserved.
//


#ifndef malloc_h
#define malloc_h

#include <stdbool.h>

void * mmalloc(int bytes);
void mfree(void *ptr);
void initialize(void *heapStart, int heapSize);

void printLinkedListEnd(void);
void printLinkedListTail(void);
void printLastAssignedMemAddr(void);
void printLinkedList(void);

struct node
{
    bool free; //If the address this node points to is freed by mfree().
    bool canOverride; //whether the memory space occupied by this node can be reused for a new node.
    int size;
    struct node * prev;
    struct node * next;
    void * heapAddr;
};

#endif /* malloc_h */

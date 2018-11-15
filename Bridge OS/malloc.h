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

void printLinkedListEnd();
void printLastAssignedMemAddr();

struct node
{
    bool free;
    int size;
    struct node * next;
    void * heapAddr;
};

#endif /* malloc_h */

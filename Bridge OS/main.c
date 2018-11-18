//
//  main.c
//  Bridge OS
//
//  Created by Bowen Sun on 11/14/18.
//  Copyright © 2018 Bowen Sun. All rights reserved.
//

#include <stdio.h>
#include "malloc.h"
#include <stdbool.h>

void printAddr(void * ptr, int id);
void testBasicAllocNFree(void);
void testBigAllocation(void);
void testIterativeAllocation(void);

//Use the main function to test mmalloc.
int main(int argc, const char * argv[]) {
    
    testBasicAllocNFree();
    testBigAllocation();
    testIterativeAllocation();
    return 0;
}

void testBasicAllocNFree()
{
    printf("------ Test Basics ------\n");
    char space[65536];
    initialize(space, 65536);
    
    //alloc 1 byte.
    void * first = mmalloc(1);
    printAddr(first, 1);   //The address of the byte should be the end of heap.
    printLastAssignedMemAddr();  //Last assigned mem addr should = first.
    printLinkedListEnd();   //tail of linkedList should grow by one node size: 32 bytes.
    
    
    //alloc 2 bytes.
    void * second = mmalloc(2);
    printAddr(second, 2);
    printLastAssignedMemAddr();//Last assigned mem addr should = second.
    printLinkedListEnd();   //tail of linkedList should grow by one node size: 36 bytes.
    
    //free second pointer.
    mfree(second);
    void * third = mmalloc(1);
    printAddr(third, 3);    //Address should be the same as second beacuse second is freed and bigger.
    
    void * forth = mmalloc(1);
    printAddr(forth, 4);    //Address should be third + 1 becuase it uses the second half of freed second.
    
    //Finally free first, third and forth pointer, the space of 1-3 should be continuous and available to use.
    mfree(first);
    mfree(third);
    mfree(forth);
    void * fifth = mmalloc(3);
    printAddr(fifth, 5); //The address of fifth == addr of forth because of continuous space.
}

void testBigAllocation()
{
    printf("------ Test Big Byte Allocations ------\n");
    char space[65536];
    initialize(space, 65536);
    
    //allocate 20000 bytes three times.
    void * first = mmalloc(20000);
    void * second = mmalloc(20000);
    void * third = mmalloc(20000);
    
    //allocate another 20000 bytes, should return NULL as we are out of space.
    void * nullPointer = mmalloc(20000);
    printAddr(nullPointer, 0);
    //Now we free the first three, there should be a continuous 60000 bytes for allocation.
    mfree(first);
    mfree(second);
    mfree(third);
    void * bigBytes = mmalloc(60000);
    
    //The address of the third pointer should be the same as pointer bigBytes as third is the beginning of the continuous freed 600000 bytes.
    printAddr(third, 3);
    printAddr(bigBytes, 4);
    
}

void testIterativeAllocation()
{
    printf("------ Test Interative Allocations ------\n");
    char space[65536];
    initialize(space, 65536);
    void * ptrs [1000];
    int i;
    for (i=0; i<1000; i++) {
        void * ptr = mmalloc(30);
        ptrs[i] = ptr;
    }
    
    //After we allocate 30 bytes 1000 times, the linkedList overhead will be 32 * 1000. We don't have space less than 10000 and mmalloc will return NULL.
    void * nilPtr = mmalloc(10000);
    printAddr(nilPtr, 0);
    
    for (i=0; i<1000; i++) {
        void * ptr = ptrs[i];
        mfree(ptr);
    }
    //After we free the allocated pointers we should have continuous space 30000.
    void * largeChunk = mmalloc(30000);
    printAddr(largeChunk, 1);
}

void printAddr(void * ptr, int id)
{
    printf("\n%d:  %p\n", id, ptr);
}

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

void printAddr(void * ptr, int num);

//Use the main function to test mmalloc.
int main(int argc, const char * argv[]) {
    char space[65536];
    initialize(space, 65536);
    
    //alloc 1 byte.
    void * first = mmalloc(1);
    printAddr(first, 1);   //The address of the byte should be the end of heap.
    printLastAssignedMemAddr();  //Last assigned mem addr should = first.
    printLinkedListEnd();   //tail of linkedList should grow by one node size: 36 bytes.
    
    
    //alloc 2 bytes.
    void * second = mmalloc(2);
    printAddr(second, 2);
    printLastAssignedMemAddr();//Last assigned mem addr should = second.
    printLinkedListEnd();   //tail of linkedList should grow by one node size: 36 bytes.
    
    //free second pointer.
    mfree(second);
    void * third = mmalloc(1);
    printAddr(third, 3);    //Address should be the same as second beacuse second is freed and bigger.
    
    return 0;
}

void printAddr(void * ptr, int num)
{
    printf("\n%d:  %p\n", num, ptr);
}

/*
 * malloc.c
 *
 *  Created on: Nov 6, 2018
 *      Author: bowen
 */


#include <stdio.h>

#include "malloc.h"

//Pointer to the heap with size 2^16 bytes.



//main()
//{
//    //initialize with a dummy head node.
//    struct node dummy = {false, 0, NULL, NULL};
//    *(struct node **)heap = (struct node *)(heap + 16);
//    *(struct node *)(heap + 16) = dummy;
//
//}

void * heap;

//Initialize the heap with the beginning pointer and heap size.
void initialize(void *heapStart, int heapSize)
{
    heap = heapStart;
    //First 8 bytes(assuming 64bit machine) stores address of the end of linkedList.
    //The next 8 bytes stores address of last assigned memory address from the end of the heap.
    *(struct node **)heap = (struct node *)(heap + 16);
    struct node dummy = {false, 0, NULL, NULL};
    *(struct node *)(heap + 16) = dummy;
    
    *(void**) (heap + 8) = (void *) (heap + heapSize);
    printf("initialized\n");
    printf("heapStart: %p\n", heap);
    printf("heapEnd: %p\n", heap + heapSize - 1);
    printLinkedListEnd();
    printLastAssignedMemAddr();
    
}

void printLinkedListEnd()
{
    printf("end of linkedList: %p\n", *(struct node **)heap);
}
void printLastAssignedMemAddr()
{
    printf("last assigned memroy address: %p\n", *(void **) (heap + 8));
}

void * mmalloc(int bytes)
{
	struct node * tail = *(struct node **)heap; //Dereference to get the value in heap pointer which is the address of tail of linkedList.
	void * mem = *(void **) (heap + 8); //Get the address of last assigned memory address.

	struct node * head = heap + 16; //Address of first linkedList node.

	//Loop through the linkedList to find freed space that can be used.
	while (head != NULL) {
		if (head -> free && head -> size >= bytes) {
			head -> free = false;
			return head -> heapAddr;
		}
        head = head -> next;
	}

	//If there isn't enough space between the last node and last assigned memory address, return NULL;
	int nodeSize = sizeof(struct node);
	if ((long)mem - (long)tail - nodeSize < bytes + nodeSize) {
		return NULL;
	}

	//Allocate new space and create a new node.
	mem -= bytes;
	struct node newNode = {false, bytes, NULL, mem};
	struct node * newNodeAddr = tail + nodeSize;
	tail -> next = newNodeAddr;
	*newNodeAddr = newNode;
	*(struct node **) heap = newNodeAddr;
	*(void **) (heap + 8) = mem;
	return mem;
}

void mfree(void *ptr)
{
	//If no linkedList node is created, there's nothing to free.
	if (*(struct node **)heap == 0) {
		return;
	}

	struct node * head = heap + 16; //Address of first linkedList node.

	//Loop through the linked list to find one with matching heap address value.
	while (head != NULL) {
		if (head -> heapAddr == ptr) {
			head -> free = true;
			break;
		}
		head = head -> next;
	}
}



/*
 * malloc.c
 *
 *  Created on: Nov 6, 2018
 *      Author: bowen
 */


#include <stdio.h>

#include "malloc.h"

//Pointer to the heap.
void * heap;
struct node * getNewNodeSpace(void);

//Initialize the heap with the beginning pointer and heap size.
void initialize(void *heapStart, int heapSize)
{
    heap = heapStart;
    //First 8 bytes(assuming 64bit machine) stores address of the end location of linkedList.
    //The next 8 bytes stores address of last assigned memory address from the end of the heap.
    //The next 8 bytes stores address of linkedList tail
    *(struct node **)heap = (struct node *)(heap + 24);
    *(void**) (heap + 8) = (void *) (heap + heapSize);
    *(struct node **) (heap + 16) = (struct node *)(heap + 24);
    struct node dummy = {false, false, 0, NULL, NULL, NULL};
    *(struct node *)(heap + 24) = dummy;
    
    printf("initialized\n");
    printf("heapStart: %p\n", heap);
    printf("heapEnd: %p\n", heap + heapSize - 1);
    printLinkedListEnd();
    printLinkedListTail();
    printLastAssignedMemAddr();
    
}

void printLinkedListEnd(void)
{
    printf("end of linkedList: %p\n", *(struct node **)heap);
}
void printLastAssignedMemAddr(void)
{
    printf("last assigned memroy address: %p\n", *(void **) (heap + 8));
}
void printLinkedListTail(void)
{
    printf("tail of linkedList: %p\n", *(struct node **)(heap + 16));
}

void * mmalloc(int bytes)
{
	struct node * end = *(struct node **)heap; //Dereference to get the value in heap pointer which is the address of tail of linkedList.
	void * mem = *(void **) (heap + 8); //Get the address of last assigned memory address.

	struct node * head = heap + 24; //Address of first linkedList node.

	//Loop through the linkedList to find freed space that can be used.
	while (head != NULL) {
		if (head -> free && head -> size >= bytes) {
            //if equal size, occupy and return. Else return split extra space to insert newNode.
            if (head -> size == bytes) {
                head -> free = false;
                return head -> heapAddr;
            } else {
                struct node * next = head -> next;
                struct node newNode = {true, false, head -> size - bytes, head, next, head -> heapAddr + bytes};
                struct node * newNodeAddr = getNewNodeSpace();
                head -> free = false;
                head -> next =  newNodeAddr;
                *newNodeAddr = newNode;
                return head -> heapAddr;
            }
		}
        head = head -> next;
	}

    //None of the freed space is enough.
	//If there isn't enough space between the last node and last assigned memory address, return NULL;
	int nodeSize = sizeof(struct node);
	if ((long)mem - (long)end - nodeSize < bytes + nodeSize) {
		return NULL;
	}

	//Allocate new space and create a new node.
    struct node * tail = *(struct node **) (heap + 16);
	mem -= bytes;
	struct node newNode = {false, false, bytes, tail, NULL, mem};
    struct node * newNodeAddr = getNewNodeSpace();
	tail -> next = newNodeAddr;
	*newNodeAddr = newNode;
	*(struct node **) (heap + 16) = newNodeAddr;
	*(void **) (heap + 8) = mem;
	return mem;
}

struct node * getNewNodeSpace(void) {
    struct node * head = heap + 24; //Address of first linkedList node.
    struct node * end = *(struct node **)heap;
    while ((long) head <= (long) end) {
        if (head -> canOverride) return head;
        head += 1;
    }
    *(struct node **) heap = head;
    return head;
}

void mfree(void *ptr)
{
	//If no linkedList node is created, there's nothing to free.
	if (*(struct node **)heap == 0) {
		return;
	}

	struct node * head = heap + 24; //Address of first linkedList node.

	//Loop through the linked list to find one with matching heap address value.
	while (head != NULL) {
		if (head -> heapAddr == ptr) {
            //mark as free and canOverride.
			head -> free = true;
            head -> canOverride = true;
            //Look up prev and next to combine continuous freed memory slots.
            int sizeCnt = head -> size;
            struct node * start = head;
            while (start -> prev -> free) {
                start -> prev -> canOverride = true;
                sizeCnt += start -> prev -> size;
                start = start -> prev;
            }
            struct node * end = head;
            while (end -> next != NULL && end -> next -> free) {
                end -> next -> canOverride = true;
                sizeCnt += end -> next -> size;
                end = end -> next;
            }
            //Now create the new freed node at start, the nodes following start until end is now marked at canOverride.
            struct node newNode = {true, false, sizeCnt, start -> prev, end -> next, end -> heapAddr};
            *start = newNode;
            if (end -> next != NULL) end -> next -> prev = start;
			break;
		}
		head = head -> next;
	}
}

void printLinkedList()
{
    struct node * head = heap + 24; //Address of first linkedList node.
    printf("Beginning of linkedList ---\n");
    //Loop through the linked list to find one with matching heap address value.
    while (head != NULL) {
        printf("node address: %p\n", head);
        printf("free: %d\n", head -> free);
        printf("canOverride: %d\n", head -> canOverride);
        printf("address: %p", head -> heapAddr);
        printf("size: %d\n\n", head -> size);
        head = head -> next;
    }
    printf("End of linkedList ---\n");
}

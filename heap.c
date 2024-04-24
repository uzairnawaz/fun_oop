#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#define HEAP_SIZE (1 << 20)
long mCount = 0;
long fCount = 0;
long heap_size = HEAP_SIZE;
long the_heap[HEAP_SIZE / sizeof(long)];
long *freeStart = (long*) the_heap;
bool initialized = false;
void* fun_malloc(size_t bytes) {
    //align bytes to 16 byte value
    if (bytes % 16 != 0) {
        bytes = bytes + (16 - (bytes % 16));
    }
    //initialize heap for the first time
    if (!initialized) {
        freeStart = freeStart + 1;
        long heapLongs = heap_size/8;
        the_heap[0] = 0; //padding for 16 byte alignment with 8 byte header
        the_heap[1] = -1 * (heap_size - 16);
        the_heap[2] = 0;
        the_heap[3] = 0;
        the_heap[heapLongs - 2] = -1 * (heap_size - 16);
        the_heap[heapLongs - 1] = 0; //end of padding
        initialized = true;
    }
    mCount += 1;
    if (bytes == 0) {
        //unsuccessful malloc (doesn't do anything)
        return 0;
    }
    long *memoryPoint = freeStart;
    bool memoryAllocated = false;
    int found_fit = 0;
    bool can_fit = false;
    //loop through free list to check for space
    memoryPoint = loopThroughLinkedList(memoryPoint, &memoryAllocated, &found_fit, &can_fit, bytes);
    if (memoryAllocated) {
        return memoryPoint + 1; // + 1 for alignment
    }
    //no space left
    if (!can_fit) {
        return 0;
    } else {
        while (!memoryAllocated) {
            memoryPoint = freeStart;
            memoryPoint = loopThroughLinkedList(memoryPoint, &memoryAllocated, &found_fit, &can_fit, bytes);
        }
        return memoryPoint + 1;
    }
}
long* loopThroughLinkedList(long* memoryPoint, bool* memoryAllocated, int* found_fit, bool* can_fit, size_t bytes) {
    long size;
    long *prevFree;
    long *nextFree;
    while (memoryPoint != 0 && !*memoryAllocated) {
        size = -1 * memoryPoint[0];
        prevFree = (long*)memoryPoint[1];
        nextFree = (long*)memoryPoint[2];
        if (size >= bytes + (2 * sizeof(long))) {
            *found_fit += 1;
            if (!*can_fit) {
                *can_fit = true;
            }
        }
        if (size >= bytes + (6 * sizeof(long)) && *found_fit == 5) {
            //split free block
            long numLongs = bytes/8;
            long sizeLongs = size/8;
            long firstPartBytes = bytes + (2 * sizeof(long));
            long secondPartBytes = size - firstPartBytes;
            if (memoryPoint == freeStart) {
                freeStart = memoryPoint + 2 + numLongs;
            }
            //update metadata for split block
            memoryPoint[0] = firstPartBytes;
            memoryPoint[1 + numLongs] = firstPartBytes;
            memoryPoint[2 + numLongs] = -1 * secondPartBytes;
            memoryPoint[3 + numLongs] = (long) prevFree;
            memoryPoint[4 + numLongs] = (long) nextFree;
            memoryPoint[sizeLongs - 1] = -1 * secondPartBytes;
            //what if prevFree is 0?
            if (prevFree != 0 && memoryPoint + 2 + numLongs != freeStart) {
                prevFree[2] = (long) (memoryPoint + 2 + numLongs);
            }
            //what if nextFree is 0?
            if (nextFree != 0) {
                nextFree[1] = (long) (memoryPoint + 2 + numLongs);
            }
            *memoryAllocated = true;
        } else if (size >= bytes + (2 * sizeof(long)) && *found_fit == 5) {
            //cut free block
            if (memoryPoint == freeStart) {
                freeStart = (long*)memoryPoint[2]; //move start of free list rightward
            } else if (memoryPoint[2] == 0) {
                prevFree[2] = 0; //cut off the tail of free list
            } else {
                //cut out this intermediate link
                long *prevBlock = prevFree;
                long *nextBlock = nextFree;
                prevBlock[2] = (long) nextBlock;
                nextBlock[1] = (long) prevBlock;
            }
            long sizeLongs = size/8;
            memoryPoint[0] = size;
            memoryPoint[sizeLongs - 1] = size;
            *memoryAllocated = true;
        } else {
            //keep looking
            memoryPoint = (long*) memoryPoint[2];
        }
    }
    return memoryPoint;
}
void fun_free(void* p) {
    fCount += 1;
    if (!p || mCount == 0) {
        //invalid pointer or nothing has been malloced
        return;
    }
    long* freedBlock = ((long*) p) - 1;
    if (freeStart != 0) {
        //search for nearest free block
        long size = freedBlock[0];
        long sizeLongs = size/8;
        freedBlock[0] = -1 * size;
        freedBlock[sizeLongs - 1] = -1 * size;
        long* currentFree = freeStart;
        while (currentFree[2] != 0 && freedBlock > currentFree) {
            currentFree = (long*)currentFree[2];
        }
        //insert new free block into linked list
        if (currentFree[2] == 0 && freedBlock > currentFree) {
            //after last free
            currentFree[2] = (long) freedBlock;
            freedBlock[1] = (long)currentFree;
            freedBlock[2] = 0;
        } else if (freedBlock < freeStart) {
            //before first free
            freeStart = freedBlock;
            currentFree[1] = (long) freedBlock;
            freedBlock[2] = (long)currentFree;
        } else {
            //general case
            long* oldCurrentPrev = (long*)currentFree[1];
            currentFree[1] = (long) freedBlock;
            freedBlock[2] = (long)currentFree;
            oldCurrentPrev[2] = (long) freedBlock;
            freedBlock[1] = (long)oldCurrentPrev;
        }
        //coalesce blocks
        leftMerge(freedBlock);
        rightMerge(freedBlock + sizeLongs - 1);
    } else {
        //create brand new free chain (heap was previously full)
        freeStart = freedBlock;
        long size = freedBlock[0];
        long sizeLongs = size/8;
        freedBlock[0] = -1 * size;
        freedBlock[sizeLongs - 1] = -1 * size;
    }   
}

//if left block is freed, stitch blocks together and update linked list + metadata
void leftMerge(long* current) {
    if (current != (long*) the_heap + 1 && current[-1] < 0) {
        //mergeable
        long* leftEnd = (long*)(((char*) current) + current[-1]);
        long newBlockSize = leftEnd[0] + current[0];
        leftEnd[0] = newBlockSize;
        long numLongs = (newBlockSize/8) * - 1;
        leftEnd[numLongs - 1] = newBlockSize;
        long* nextBlock = (long*) current[2];
        if (nextBlock != 0) {
            //edge case at end of list
            nextBlock[1] = (long) leftEnd;
        }
        leftEnd[2] = current[2];
    }
}
void rightMerge(long* current) {
    if (current != (long*) the_heap + ((heap_size/8) - 2) && current[1] < 0) {
        //mergeable
        long* rightEnd = current + 1;
        long newBlockSize = rightEnd[0] + current[0];
        current = current + (current[0]/8 + 1);
        current[0] = newBlockSize;
        long numLongs = (newBlockSize/8) * - 1;
        current[numLongs - 1] = newBlockSize;
        long* nextBlock = (long*) rightEnd[2];
        if (nextBlock != 0) {
            //edge case at end of list
            nextBlock[1] = (long) current;
        }
        current[2] = rightEnd[2];
    }
}

//print out free chain and all of the heap's contents by 8-byte increments
void heap_check() {
    printf("Free chain\n");
    long *memoryPoint = freeStart;
    long size;
    long *prevFree;
    long *nextFree;
    int i = 1;
    while (memoryPoint != 0) {
        size = -1 * memoryPoint[0];
        prevFree = (long*)memoryPoint[1];
        nextFree = (long*)memoryPoint[2];
        printf("Free Block %d\n", i);
        printf("Memory Point %ld\n", (long) memoryPoint);
        printf("Size Beginning %ld\n", size);
        printf("Prev Free %ld\n", (long) prevFree);
        printf("Next Free %ld\n", (long) nextFree);
        i++;
        memoryPoint = (long*) memoryPoint[2];
    }
    long totalSize = 0;
    memoryPoint = (long*)the_heap;
    i = 0;
    while (totalSize <= heap_size - 16) {
        printf("%ld\n", memoryPoint[i]);
        totalSize += 8;
        i++;
    }
    printf("-----------------\n");
}

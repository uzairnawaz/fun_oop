#ifndef _HEAP_H_
#define _HEAP_H_
#include <stdbool.h>
#include <stddef.h>
// heap size in bytes, defined by the test
extern long heap_size;
// memory for the heap, defined by the test
// must contain heap_size bytes
extern long the_heap[];

// number of calls to malloc, defined and updated by the heap implementation
extern long mCount;

// number of calls to free, defined and updated by the heap implementation
extern long fCount;
void rightMerge(long* current);
void leftMerge(long* current);
void heap_check();
long* loopThroughLinkedList(long* memoryPoint, bool* memoryAllocated, int* found_fit, bool* can_fit, size_t bytes);
void updateParents(long heap_index);
void updateChildren(long heap_index);
bool firstRowSpace();
bool secondRowSpace();
bool thirdRowSpace();
void buddy_check();
#endif

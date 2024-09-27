#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <unistd.h>
#include <stdint.h>

// Info about free memory
#define FREE_MEM_START 0x0000000000600000 // Data from Pure64 Manual
#define TOTAL_HEADER_UNITS 8388608 + 1

// List node state
#define FREE 0
#define OCCUPIED 1


// Initializes memory list
void mm_init(uint64_t memory_start, uint64_t size);

// Returns a pointer to the memory block
void * mm_malloc(unsigned int nbytes);

// Returns a pointer to a new memory block of size new_size, reallocating the info contained in ptr
void * mm_realloc(void * ptr, uint64_t new_size); 

// Frees a memory block, starting in the provided pointer
void mm_free(void * ptr);

// Returns free memory size
int mm_unused_space();

// Returns used memory size
int mm_occupied_space();

// Returns total memory size
int mm_total_space();

// Returns header size
int mm_header_size();


#endif
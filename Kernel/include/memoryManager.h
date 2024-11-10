#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <unistd.h>
#include <stdint.h>

// Info about free memory
#define FREE_MEM_START 0x0000000000600000 // Data from Pure64 Manual
#define TOTAL_HEADER_UNITS (8388608 + 1)

#ifdef BUDDY_SYSTEM 

#define MEM_SIZE 0x800000 // 8MB
#define MEM_MIN 0x800 // 2k
#define MEM_BLOCKS MEM_SIZE/MEM_MIN
#define CANT_NODES MEM_BLOCKS*2 - 1 

#define LEFT_CHILD(x) (2 * (x) + 1)
#define RIGHT_CHILD(x) (2 * (x) + 2)

static void * mem;
static mem_states[CANT_NODES];

#endif

typedef enum States {FREE=0, HALF_OCCUPIED, OCCUPIED} States;


// Initializes memory list
void mm_init(uint64_t memory_start, uint64_t size);

// Returns a pointer to the memory block
void * mm_malloc(uint64_t nbytes);

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
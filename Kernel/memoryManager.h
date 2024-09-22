#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <unistd.h>

extern char endOfBinary;

#define FREE_MEM_START 0x50000 // datos sacados del Pure64 Manual
//#define FREE_MEM_END 0xFFFFFFFFFFFFFFFF
#define FREE_MEM_END 0x9FFFF

void mm_init();
void * mm_malloc(unsigned int nbytes);
void mm_free(void * ptr);
int mm_free_space();


#endif
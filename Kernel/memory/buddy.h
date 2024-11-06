#ifndef BUDDY_H
#define BUDDY_H

// #include <stdint.h>
// #include <stdlib.h>

// #define FREE_MEM_START 0x0000000000600000 // Data from Pure64 Manual
// #define MEM_SIZE 0x10000 // 64k
// #define MEM_MIN 0x1000 // 4k
// #define MEM_BLOCKS MEM_SIZE/MEM_MIN // 16
// #define CANT_NODES MEM_BLOCKS*2 - 1 // 31

// #define LEFT_CHILD(x) x/2
// #define RIGHT_CHILD(x) x + x/2 +1

// typedef enum States {FREE=0, HALF_OCCUPIED, OCCUPIED} States;

// // Inicializa la lista de nodos de memoria
// void mm_init(uint64_t memory_start, uint64_t size);

// // Devuelve un puntero al bloque de memoria reservada
// void * mm_malloc(uint64_t nbytes);

// void * mm_realloc(void * ptr, uint64_t new_size);

// // Libera un bloque de memoria usado, cuya dirección de memoria es la indicada
// void mm_free(void * ptr);

// // Retorna la cantidad de espacio libre
// int mm_unused_space();

// // Retorna la cantidad de espacio utilizado
// int mm_occupied_space();

// // Retorna el espacio total de memoria, ocupado o no ocupado
// int mm_total_space();


#include <unistd.h>
#include <stdint.h>

// Info about free memory
#define FREE_MEM_START 0x0000000000600000 // Data from Pure64 Manual
#define TOTAL_HEADER_UNITS (8388608 + 1)

// List node state
// #define FREE 0
// #define OCCUPIED 1

#define MEM_SIZE 0x10000 // 64k
#define MEM_MIN 0x1000 // 4k
#define MEM_BLOCKS MEM_SIZE/MEM_MIN 
#define CANT_NODES MEM_BLOCKS*2 - 1

// #define LEFT_CHILD(x) x/2
// #define RIGHT_CHILD(x) x + x/2 +1
#define LEFT_CHILD(x) (2 * (x) + 1)
#define RIGHT_CHILD(x) (2 * (x) + 2)

static void * mem;
static uint8_t mem_states[CANT_NODES];

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


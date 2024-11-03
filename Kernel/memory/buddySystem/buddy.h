#ifndef BUDDY_H
#define BUDDY_H

#include <stdint.h>
#include <stdlib.h>

#define FREE_MEM_START 0x0000000000600000 // Data from Pure64 Manual
#define MEM_SIZE 0x10000 // 64k
#define MEM_MIN 0x1000 // 4k
#define MEM_BLOCKS MEM_SIZE/MEM_MIN // 16
#define CANT_NODES MEM_BLOCKS*2 - 1 // 31

#define LEFT_CHILD(x) x/2
#define RIGHT_CHILD(x) x + x/2 +1

typedef enum States {FREE=0, HALF_OCCUPIED, OCCUPIED} States;

uint8_t mem_states[CANT_NODES];

// Inicializa la lista de nodos de memoria
void mm_init(uint64_t memory_start, uint64_t size);

// Devuelve un puntero al bloque de memoria reservada
void * mm_malloc(uint64_t nbytes);

// Libera un bloque de memoria usado, cuya dirección de memoria es la indicada
void mm_free(void * ptr);

// Retorna la cantidad de espacio libre
int mm_unused_space();

// Retorna la cantidad de espacio utilizado
int mm_occupied_space();

// Retorna el espacio total de memoria, ocupado o no ocupado
int mm_total_space();


#endif 

#ifndef BUDDY_H
#define BUDDY_H

#include <stdint.h>
#include <stdlib.h>

#define MEM_SIZE 0x4000000 // 64M
#define MEM_MIN 0x1000000 // 1M
#define MEM_BLOCKS MEM_SIZE/MEM_MIN // 64
#define CANT_NODES MEM_BLOCKS*2 - 1 // 127

#define LEFT_CHILD(x) 2*x + 1
#define RIGHT_CHILD(x) 2*x + 2

const enum {FREE=0, SPLIT, OCCUPIED};



// Inicializa la lista de nodos de memoria
void mm_init(uint64_t memory_start, uint64_t size);

// Devuelve un puntero al bloque de memoria reservada
void * mm_malloc(unsigned int nbytes);

// Libera un bloque de memoria usado, cuya dirección de memoria es la indicada
void mm_free(void * ptr);

// Retorna la cantidad de espacio libre
int mm_unused_space();

// Retorna la cantidad de espacio utilizado
int mm_occupied_space();

// Retorna el espacio total de memoria, ocupado o no ocupado
int mm_total_space();


void print_mem_states();

#endif

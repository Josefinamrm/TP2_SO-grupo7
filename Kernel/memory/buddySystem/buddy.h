#ifndef BUDDY_H
#define BUDDY_H

#include <stdint.h>
#include <stdlib.h>

#define MEM_SIZE 0x4000000 // 64M
#define MEM_MIN 0x1000000 // 1M
#define MEM_BLOCKS MEM_SIZE/MEM_MIN // 64
// #define CANT_NODES MEM_BLOCKS*2 - 1 // 127

#define CANT_NODES 31

#define LEFT_CHILD(x) x/2
#define RIGHT_CHILD(x) x + x/2 +1

const enum states {FREE=0, HALF_OCCUPIED, OCCUPIED};

uint8_t mem_states[CANT_NODES];

// Inicializa la lista de nodos de memoria
void mm_init(uint64_t memory_start, uint64_t size);

// Devuelve un puntero al bloque de memoria reservada
uint64_t mm_malloc(uint64_t nbytes);

// Libera un bloque de memoria usado, cuya dirección de memoria es la indicada
void mm_free(int pos);

// Retorna la cantidad de espacio libre
int mm_unused_space();

// Retorna la cantidad de espacio utilizado
int mm_occupied_space();

// Retorna el espacio total de memoria, ocupado o no ocupado
int mm_total_space();


#endif

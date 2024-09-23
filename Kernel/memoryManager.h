#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <unistd.h>

// Información de la memoria libre
#define FREE_MEM_START 0x0000000000600000 // datos sacados del Pure64 Manual
#define TOTAL_HEADER_UNITS 4096 + 1

// Estado de un nodo de la lista
#define FREE 0
#define OCCUPIED 1


// Inicializa la lista de nodos de memoria
void mm_init();

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

// Tamaño del header
int mm_header_size();


#endif
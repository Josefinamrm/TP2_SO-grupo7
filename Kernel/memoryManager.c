#include "memoryManager.h" 

typedef long Align;

typedef union header{
    struct{
        union header *  next;
        unsigned size;
    } node;
} Header;

static Header * free_mem; // ACÁ VA A ESTAR TODA LA LISTA


// hacer un init
void mm_init(){
    
    // ASIGNAR EL free_mem
    //free_mem = (Header *)endOfBinary;
    free_mem = (Header *)FREE_MEM_START; // datos sacados del Pure64 Manual
    free_mem->node.size = (FREE_MEM_END - FREE_MEM_START) - sizeof(Header); // tamaño de la memoria libre
    free_mem->node.next = NULL;

    
}


// MALLOC
void * mm_malloc(unsigned int nbytes){
   
    unsigned nuints = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;

    Header *previous;
    previous = free_mem;

    // busco nodo con tamaño <FIRST FIT>
    Header * current;
    for(current = previous->node.next; ; previous = current, current = current->node.next){ // recorro la lista  VER!!!!!!
        if(current->node.size >= nuints){ // si es más grande
            if(current->node.size == nuints){
                previous->node.next = current->node.next; // si es lo que necesito, es
            }else{
                current->node.size -= nuints;
                current += current->node.size;
                current->node.size = nuints;
            }

            return (void *)(current + 1);
        }

        if(current == free_mem){ // si llego al final
            return NULL; // no hay nada libre
        }   
    }
}


// FREE vuelve a meter al bloque en la lista
void mm_free(void * toFree){
    Header *bp, *current;

    bp = (Header *)toFree - 1; // -1 porque "saco el header"

    // veo donde va
    for(current = free_mem; !(bp > current && bp < current->node.next); current = current->node.next){
        if(current >= current->node.next /*no circular*/ && (bp > current || bp < current->node.next) /* en el medio de los nodos*/){
            break; // encontré la posición
        }
    }

    //COALESCE:
    if(bp + bp->node.size == current->node.next){ // si son contiguos
        bp->node.size += current->node.next->node.size;
        bp->node.next = current->node.next->node.next;
    } else {
        bp->node.next = current->node.next;
    }

    if(current + current->node.size == bp){ // lo meto
        current->node.size += bp->node.size;
        current->node.next = bp->node.next;
    } else{
        current->node.next = bp;
    }

    free_mem = current;
}


// consultar cuánto espacio libre tengo
int mm_free_space(){
    Header * current;
    int total = 0;
    for(current = free_mem; current != NULL; current = current->node.next){
        total += current->node.size;
    }
    return total;
}
/* 

    Se mantiene una lista con todos los nodos de memoria, libre o ocupada

 */


#include "memoryManager.h" 

typedef long Align;

typedef union header {
    struct{
        union header * next;
        unsigned size;
        unsigned state;
    } s;
    Align x;
}Header;

static int header_size = sizeof(Header);

// Puntero al inicio de la lista
static Header * start; 



// Inicializa la lista y la cantidad de nodos libres
void mm_init(){
    start = (Header *) FREE_MEM_START;
    start->s.next = NULL;
    start->s.size = TOTAL_HEADER_UNITS - 1;
    start->s.state = FREE;
}


// MALLOC
void * mm_malloc(unsigned int nbytes){
    Header *current;
    unsigned nuints;

    nuints = (nbytes + header_size - 1)/header_size + 1;

    for(current = start; current != NULL; current = current->s.next){
        if(current->s.state == FREE && current->s.size >= nuints){
            if(current->s.size == nuints){
                current->s.state = OCCUPIED;
                return (void *) (current + 1);
            }
            else{
                Header * to_return;
                to_return->s.size = nuints;
                to_return->s.next = current->s.next;
                to_return->s.state = OCCUPIED;
                current->s.size -= nuints; 
                current->s.next = to_return;
                to_return = current + current->s.size;
                return (void *)(to_return + 1);
            }
        }
    }

    return NULL;
}


// 
void mm_free(void * ptr){
    Header *to_free, *p, *previous = NULL;

    // Apunta al header del bloque
    to_free = (Header *)ptr - 1;

    if(to_free->s.state == FREE)
        return;
    
    for(p = start; p != to_free && p != NULL ; previous = p, p = p->s.next);

    if(p == NULL)
        return;

    if(p->s.next != NULL && p->s.next == FREE){
        p->s.size += p->s.next->s.size;
        p->s.next = p->s.next->s.next;
    }

    if(previous != NULL && previous->s.state == FREE){
        previous->s.size += p->s.size;
        previous->s.next = p->s.next;
    }else{
        p->s.state = FREE;
    }
}


// Función auxiliar usada por mm_occupied_space y por mm_free_space
static int calculate_space(int condition){
    Header * current;
    int total = 0;
    for(current = start; current != NULL; current = current->s.next){
        if(current->s.state == condition)
            total += (current->s.size * header_size);
    }
    return total;
}


// Consultar espacio ocupado
int mm_occupied_space(){
    return calculate_space(OCCUPIED);
}


// Consultar cuánto espacio libre tengo
int mm_unused_space(){
    return calculate_space(FREE);
}


// Memoria total disponible
int mm_total_space(){
    return sizeof(Header)*(TOTAL_HEADER_UNITS - 1);
}


// es auxiliar para ahora
int mm_header_size(){
    return header_size;
}
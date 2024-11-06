// // This is a personal academic project. Dear PVS-Studio, please check it.
// // PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// /* 

//     Se mantiene una lista con todos los nodos de memoria, libre o ocupada

//  */


// #include "memoryManager.h" 

// typedef long Align;

// typedef union header {
//     struct{
//         union header * next;
//         unsigned size;
//         unsigned state;
//     } s;
//     Align x;
// }Header;

// static int header_size = sizeof(Header);

// // Puntero al inicio de la lista
// static Header * start; 



// // Inicializa la lista y la cantidad de nodos libres
// void mm_init(uint64_t memory_start, uint64_t size){
//     start = (Header *) memory_start;
//     start->s.next = NULL;
//     start->s.size = size;
//     start->s.state = FREE;
// }


// // MALLOC
// void * mm_malloc(uint64_t nbytes){
//     Header *current;
//     uint64_t nuints;

//     nuints = (nbytes + header_size - 1)/header_size + 1;

//     for(current = start; current != NULL; current = current->s.next){
//         if(current->s.state == FREE && current->s.size >= nuints){
//             if(current->s.size == nuints){
//                 current->s.state = OCCUPIED;
//                 return (void *) (current + 1);
//             }
//             else{
//                 Header * to_return;
//                 current->s.size -= nuints; 
//                 to_return = current + current->s.size;
//                 to_return->s.size = nuints - 1;
//                 to_return->s.next = current->s.next;
//                 to_return->s.state = OCCUPIED;
//                 current->s.next = to_return;
//                 return (void *)(to_return + 1);
//             }
//         }
//     }

//     return NULL;
// }


// // 
// void mm_free(void * ptr){
//     Header *to_free, *p, *previous = NULL;

//     // Apunta al header del bloque
//     to_free = (Header *)ptr - 1;

//     if(to_free->s.state == FREE)
//         return;
    
//     for(p = start; p != to_free && p != NULL ; previous = p, p = p->s.next);

//     if(p == NULL)
//         return;

//     if(p->s.next != NULL && p->s.next->s.state == FREE){
//         p->s.size += (p->s.next->s.size + 1);
//         p->s.next = p->s.next->s.next;
//     }

//     if(previous != NULL && previous->s.state == FREE){
//         previous->s.size += (p->s.size + 1);
//         previous->s.next = p->s.next;
//     }else{
//         p->s.state = FREE;
//     }
// }

// void * mm_realloc(void * ptr, uint64_t new_size){
//     char * to_return = (void *) mm_malloc(new_size);

//     if(to_return == NULL){
//         return NULL;
//     }

//     // Obtain block memory size
//     Header * ptr_hd = (Header *) ptr - 1;
//     int size = ptr_hd->s.size * sizeof(Header);

//     // Copy data from old pointer to new pointer
//     for (int i = 0; i < size; i++){
//         to_return[i] = ((char *) ptr)[i];
//     }

//     mm_free(ptr);
//     return (void *) to_return;
// }


// // auxiliary function used in mm_occupied_space and mm_free_space
// static int calculate_space(int condition){
//     Header * current;
//     int total = 0;
//     for(current = start; current != NULL; current = current->s.next){
//         if(current->s.state == condition)
//             total += (current->s.size * header_size);
//     }
//     return total;
// }


// // Consultar espacio ocupado
// int mm_occupied_space(){
//     return calculate_space(OCCUPIED);
// }


// // Consultar cuánto espacio libre tengo
// int mm_unused_space(){
//     return calculate_space(FREE);
// }


// // Memoria total disponible
// int mm_total_space(){
//     return sizeof(Header)*(TOTAL_HEADER_UNITS - 1);
// }


// // es auxiliar para ahora
// int mm_header_size(){
//     return header_size;
// }
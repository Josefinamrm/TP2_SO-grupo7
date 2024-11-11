// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// // This is a personal academic project. Dear PVS-Studio, please check it.
// // PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com


// #include "buddy.h"
// #include <stdio.h>


// uint8_t mem_states[CANT_NODES];


#include <memoryManager.h>  


// #ifdef BUDDY_SYSTEM

// uint64_t used_space = 0;
// uint64_t total_space = 0;

// void mm_init(uint64_t memory_start, uint64_t size){
//     printArray("BUDDY\n");
//     for(int i = 0; i < CANT_NODES; i++){
//         mem_states[i] = FREE;
//     }
//     total_space = size;
//     mem = (void *) memory_start;
// }

// uint64_t offset = 0;
// static uint64_t rec_malloc(uint64_t pos, uint64_t nbytes, uint64_t node_size){

//     if(mem_states[pos] == OCCUPIED){
//         return -1;
//     }


//     if((node_size/2) >= nbytes && (node_size/2) >= MEM_MIN){
//         uint64_t left = rec_malloc(LEFT_CHILD(pos), nbytes, node_size/2);
//         if(left == -1){
//             uint64_t right = rec_malloc(RIGHT_CHILD(pos), nbytes, node_size/2);
//                 if(right == node_size/2){
//                     if(mem_states[pos] == HALF_OCCUPIED)
//                         mem_states[pos] = OCCUPIED;
//                 }
//                 offset += node_size/2;
//                 return right;
            
//         }
            
//         else{
//             if(left == node_size/2){
//                 if(mem_states[pos] == FREE)
//                     mem_states[pos] = HALF_OCCUPIED;
//             }
//             return left;
            
//         }
        
//     }
//     else{
//         if(mem_states[pos] == FREE){
//             mem_states[pos] = OCCUPIED;
//             used_space += node_size;
//             return node_size;
//         }
//     }
// }

 
// void * mm_malloc(uint64_t nbytes){
//     offset = 0;
    
//     uint64_t rec = rec_malloc(0, nbytes, total_space);
//     if(rec == -1){
//         return NULL;
//     }

//     return (void *)(mem + offset);
// }

// void * mm_realloc(void * ptr, uint64_t new_size){
//     void * to_return = mm_malloc(new_size);

//     if(to_return == NULL){
//         return NULL;
//     }

//     for(int i = 0; i < new_size; i++){
//         ((char *) to_return)[i] = ((char *) ptr)[i];
//     }

//     mm_free(ptr);
//     return to_return;
// }


// static uint64_t rec_free(uint64_t pos, uint64_t offset, uint64_t node_size) {
//     if (node_size < MEM_MIN) {
//         return -1;
//     }

//     if (offset < node_size / 2) {
//         uint64_t left = rec_free(LEFT_CHILD(pos), offset, node_size / 2);
//         if (left == 0) { // coalesing
//             if (mem_states[RIGHT_CHILD(pos)] == FREE) {
//                 mem_states[pos] = FREE;
//             } else {
//                 mem_states[pos] = HALF_OCCUPIED;
//             }
//         }
//     } else {
//         uint64_t right = rec_free(RIGHT_CHILD(pos), offset - node_size/2, node_size / 2);
//         if (right == 0) { // coalesing
//             if (mem_states[LEFT_CHILD(pos)] == FREE) {
//                 mem_states[pos] = FREE;
//             } else {
//                 mem_states[pos] = HALF_OCCUPIED;
//             }
//         }
//     }

//     if (offset == 0 && node_size >= MEM_MIN && mem_states[pos] == OCCUPIED) {
//         mem_states[pos] = FREE;
//         used_space -= node_size;
//         return 0;
//     }

//     return -1; 
// }


// void mm_free(void *ptr) {
//     uint64_t offset = (uint64_t) (ptr - mem);
//     uint64_t node_size = total_space;
//     uint64_t pos = 0;

//     rec_free(pos, offset, node_size);
// }


// int mm_unused_space(){
//     return total_space - used_space;
// }

// int mm_occupied_space(){
//     return used_space;
// }

// int mm_total_space(){
//     return total_space;
// }

// #endif
// // // // // // This is a personal academic project. Dear PVS-Studio, please check it.
// // // // // // PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// // // // // #include "buddy.h"
// // // // // #include <stdio.h>



// // // // // uint8_t * mem_states[CANT_NODES] = {FREE}; 

// // // // // void mm_init(uint64_t memory_start, uint64_t size){
// // // // //     for(int i = memory_start; i < memory_start + size; i++){
// // // // //         mem_states[i] = FREE;
// // // // //     }
// // // // // }

// // // // // void print_mem_states(){
// // // // //     int mem = 64;
// // // // //     for(uint32_t i =0; i < CANT_NODES; i++){
// // // // //         printf("%d (%hhn)", mem, mem_states[i]);

// // // // //         mem = mem/2;
// // // // //     }
// // // // // }


// // // // // static uint32_t rec_malloc(unsigned int nbytes, int position, int node_size){

// // // // //     if(*mem_states[position] == OCCUPIED){
// // // // //         return -1;
// // // // //     }

// // // // //     if(*mem_states[position] == SPLIT){
// // // // //         if(rec_malloc(nbytes, LEFT_CHILD(position), node_size/2) == -1){
// // // // //             return rec_malloc(nbytes, RIGHT_CHILD(position), node_size/2);
// // // // //         }

// // // // //     } else {
// // // // //         if(nbytes < 2*node_size){
// // // // //             if(rec_malloc(nbytes, LEFT_CHILD(position), node_size/2) == -1)
// // // // //                 if(rec_malloc(nbytes, RIGHT_CHILD(position), node_size/2) == -1);
// // // // //                     return -1;
// // // // //             *mem_states[position] = OCCUPIED;        
// // // // //             return position;
// // // // //         }
// // // // //     }
// // // // // }


// // // // // void * mm_malloc(unsigned int nbytes){
// // // // //     if(rec_malloc(nbytes, 0, MEM_SIZE) == -1){
// // // // //         return NULL;
// // // // //     }
// // // // // }

// // // // // void mm_free(void * ptr){
    
// // // // // }


// // // // // "arbol de memoria"

// #include <memoryManager.h>
// // #include "buddy.h"
// // #include <stdio.h>


// // uint8_t mem_states[CANT_NODES];

// uint64_t used_space = 0;
// uint64_t total_space = 0;

// void mm_init(uint64_t memory_start, uint64_t size){
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
    
//     uint64_t pos = rec_malloc(0, nbytes, total_space);
//     if(pos == -1){
//         return NULL;
//     }
//     return (void *)(mem + offset);
// }

// void * mm_realloc(void * ptr, uint64_t new_size){
//     // printArray("realloc\n");
// }

// static void rec_free(uint64_t pos, uint64_t node_size){

//     if(mem_states[pos] == FREE){
//         return;
//     }

//     if(mem_states[pos] == HALF_OCCUPIED){
//         rec_free(LEFT_CHILD(pos), node_size/2);
//         rec_free(RIGHT_CHILD(pos), node_size/2);
//         mem_states[pos] = FREE;
//     }
//     else{
//         mem_states[pos] = FREE;
//         used_space -= node_size;
//     }

// }

// void mm_free(void * ptr){
//     printArray("free\n");
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

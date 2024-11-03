// // This is a personal academic project. Dear PVS-Studio, please check it.
// // PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// #include "buddy.h"
// #include <stdio.h>



// uint8_t * mem_states[CANT_NODES] = {FREE}; 

// void mm_init(uint64_t memory_start, uint64_t size){
//     for(int i = memory_start; i < memory_start + size; i++){
//         mem_states[i] = FREE;
//     }
// }

// void print_mem_states(){
//     int mem = 64;
//     for(uint32_t i =0; i < CANT_NODES; i++){
//         printf("%d (%hhn)", mem, mem_states[i]);

//         mem = mem/2;
//     }
// }


// static uint32_t rec_malloc(unsigned int nbytes, int position, int node_size){

//     if(*mem_states[position] == OCCUPIED){
//         return -1;
//     }

//     if(*mem_states[position] == SPLIT){
//         if(rec_malloc(nbytes, LEFT_CHILD(position), node_size/2) == -1){
//             return rec_malloc(nbytes, RIGHT_CHILD(position), node_size/2);
//         }

//     } else {
//         if(nbytes < 2*node_size){
//             if(rec_malloc(nbytes, LEFT_CHILD(position), node_size/2) == -1)
//                 if(rec_malloc(nbytes, RIGHT_CHILD(position), node_size/2) == -1);
//                     return -1;
//             *mem_states[position] = OCCUPIED;        
//             return position;
//         }
//     }
// }


// void * mm_malloc(unsigned int nbytes){
//     if(rec_malloc(nbytes, 0, MEM_SIZE) == -1){
//         return NULL;
//     }
// }

// void mm_free(void * ptr){
    
// }


// "arbol de memoria"

#include "buddy.h"
#include <stdio.h>  
// #include <memoryManager.h>

uint8_t mem_states[CANT_NODES];
void * mem = FREE_MEM_START;

uint32_t used_space = 0;
uint32_t total_space = 0;


void mm_init(uint64_t memory_start, uint64_t size){
    for(int i = 0; i < CANT_NODES; i++){
        mem_states[i] = FREE;
    }
    total_space = size;
}

static uint64_t rec_malloc(uint64_t pos, uint64_t cant, uint64_t node_size){

    printf("pos: %ld - cant: %ld - node_size:%ld\n", pos, cant, node_size);

    if(cant > node_size || node_size < MEM_MIN){
        return -1;
    }

    if(mem_states[pos] == OCCUPIED){
        return -1;
    }

    if(cant < node_size/2){
        
        uint64_t left = rec_malloc(LEFT_CHILD(pos), cant, node_size/2);
        if(left == -1){
            uint64_t right = rec_malloc(RIGHT_CHILD(pos), cant, node_size/2);
            if(right == -1){
                return -1;
            }
            mem_states[pos] = OCCUPIED;
            return right;
        }
        mem_states[pos] = HALF_OCCUPIED;
        return left;

    }
    used_space += node_size;
    mem_states[pos] = OCCUPIED;
    return pos;
}

void * mm_malloc(uint64_t nbytes){
    printf("cant_nodes: %d\n", CANT_NODES);
    uint64_t pos = rec_malloc((CANT_NODES)/2, nbytes, total_space);
    if(pos == -1){
        return NULL;
    }
    printf("offset: %ld\n", (pos*nbytes));
    return (void *) (mem + pos*nbytes);
}


// 0 si liberó, -1 si no liberó
static uint32_t rec_free(uint64_t i,  uint64_t pos, uint64_t node_size){

    if(i == pos){
        if(mem_states[i] == OCCUPIED){
            mem_states[i] = FREE;
            used_space -= node_size;
            return 0;
        }
        else if(mem_states[i] == FREE || mem_states[i] == HALF_OCCUPIED){
            return -1;
        }
    }
    if(pos < i){

        if(rec_free(LEFT_CHILD(i), pos, node_size/2) == 0){
            if(mem_states[i] == OCCUPIED)
                mem_states[i] = HALF_OCCUPIED;
            else if(mem_states[i] == HALF_OCCUPIED)
                mem_states[i] = FREE;
            return 0;
        }
        return -1;
    }
    else{

    if(rec_free(RIGHT_CHILD(i), pos, node_size/2)== 0){
        if(mem_states[i] == OCCUPIED)
                mem_states[i] = HALF_OCCUPIED;
        else if(mem_states[i] == HALF_OCCUPIED)
                mem_states[i] = FREE;
        return 0;
    }
    return -1;    
    }
}

void mm_free(void * ptr){
    if(ptr ==  NULL){
        return;
    }
    else{
        uint64_t pos = (ptr - mem)/MEM_MIN;
        rec_free(LEFT_CHILD(CANT_NODES), pos, total_space);
        return;
    }
}

int mm_unused_space(){
    return total_space - used_space;
}

int mm_occupied_space(){
    return used_space;
}

int mm_total_space(){
    return total_space;
}

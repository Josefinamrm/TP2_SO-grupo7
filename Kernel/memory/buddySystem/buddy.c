#include "buddy.h"
#include <stdio.h>


// "arbol de memoria"

uint8_t * mem_states[CANT_NODES] = {FREE}; 

void mm_init(uint64_t memory_start, uint64_t size){
    for(int i = memory_start; i < memory_start + size; i++){
        mem_states[i] = FREE;
    }
}

void print_mem_states(){
    int mem = 64;
    for(uint32_t i =0; i < CANT_NODES; i++){
        printf("%d (%hhn)", mem, mem_states[i]);

        mem = mem/2;
    }
}


static uint32_t rec_malloc(unsigned int nbytes, int position, int node_size){

    if(*mem_states[position] == OCCUPIED){
        return -1;
    }

    if(*mem_states[position] == SPLIT){
        if(rec_malloc(nbytes, LEFT_CHILD(position), node_size/2) == -1){
            return rec_malloc(nbytes, RIGHT_CHILD(position), node_size/2);
        }

    } else {
        if(nbytes < 2*node_size){
            if(rec_malloc(nbytes, LEFT_CHILD(position), node_size/2) == -1)
                if(rec_malloc(nbytes, RIGHT_CHILD(position), node_size/2) == -1);
                    return -1;
            *mem_states[position] = OCCUPIED;        
            return position;
        }
    }
}


void * mm_malloc(unsigned int nbytes){
    if(rec_malloc(nbytes, 0, MEM_SIZE) == -1){
        return NULL;
    }
}

void mm_free(void * ptr){
    
}
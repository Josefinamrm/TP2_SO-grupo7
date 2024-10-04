#include "processManager.h"

typedef struct {

    uint8_t * name;
    
    // process managment
    uint32_t * registers;

    uint64_t pid;
    uint64_t ppid;
    uint8_t priority;
    uint8_t state;

    // memory managment PUNTERO A FUNCION - RIP
    uint64_t * text_pointer;
    uint64_t * data_pointer;  
    uint64_t * stack_pointer;

    uint64_t * parameters;

    // file managment
    uint32_t * file_descriptors;

} p ;

process process_list[MAX_PROCESS];
uint8_t process_counter = 0;

uint64_t my_getpid(){
    for(int i = 0; i < MAX_PROCESS; i++){
        if(process_list[i]->state == RUNNING){
            return process_list[i]->pid;
        }
    }
    return -1;
}

static queue add_to_ready_queue(process p){ 
    queue new = (queue) mm_malloc(sizeof(node));
    new->p = p;
    new->next = NULL;

    queue aux = ready_queue;
    while(aux->next != NULL && aux->p->priority > p->priority){ //  no se si tengo que ordenarlo segun la prioridad??????
        aux = aux->next;
    }
    queue aux2 = aux->next;
    aux->next = new;
    new->next = aux2;

    return ready_queue;
}


uint64_t my_create_process(uint8_t * name, uint64_t ppid, uint64_t priority, uint64_t argc, uint8_t ** argv){
    if(process_counter == MAX_PROCESS){
        return -1;
    }
    process new_process = (process) mm_malloc(sizeof(p));
    new_process->name = name;
    new_process->pid = ++process_counter;
    new_process->ppid = ppid;
    new_process->priority = priority;
    new_process->state = READY;

    new_process->stack_pointer = mm_malloc(PROCESS_STACK_SIZE); 

    // FALTA ASIGNARLE LOS PUNTEROS A FUNCIONES

    for(int i = 0; i < MAX_PROCESS; i++){
        if(process_list[i]->pid == ppid){
            new_process->file_descriptors = process_list[i]->file_descriptors;
            break;
        }
    }

    for(int i=0; i< argc; i++){ // #########################################################################333
        new_process->parameters[i] = argv[i];
    }

    process_list[process_counter] = new_process;

    ready_queue = add_to_ready_queue(new_process);
    
    return process_counter;
}

uint64_t my_nice(uint64_t pid, uint64_t newPrio){
    process_list[pid]->priority = newPrio;
    // ##################### si la lista está ordenada por prioridad, hay que reordenarla #####################
}

uint32_t my_kill(uint64_t pid){
    process p = process_list[pid];
    p->state = KILLED;
    free(p);
}

uint32_t my_block(uint64_t pid){
    process_list[pid]->state = BLOCKED;
}

uint32_t my_unblock(uint64_t pid){
    process_list[pid]->state = READY;
    ready_queue = add_to_ready_queue(process_list[pid]);
}

uint32_t my_yield(){
    process_list[my_getpid()]->state = READY;
    ready_queue = add_to_ready_queue(process_list[my_getpid()]);
}

uint32_t my_wait(int64_t pid){

    process_list[my_getpid()]->state = BLOCKED;
    while(process_list[pid]->state != RUNNING);
    process_list[my_getpid()]->state = READY;

    return 0;
}

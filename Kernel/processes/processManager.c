#include "processManager.h"

process process_list[MAX_PROCESS];
uint8_t process_counter = 0;

uint64_t my_getpid(){
    return current_process->pid;
}

static queue add_to_ready_queue(process p){ 
    queue new = (queue) mm_malloc(sizeof(node));
    new->p = p;
    new->next = ready_queue;
    return new;
}


uint64_t my_create_process(uint8_t * name, uint64_t function, uint64_t ppid, uint64_t priority, uint64_t argc, uint8_t ** argv){
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

    // FALTA ASIGNARLE LOS PUNTEROS A FUNCIONES ###############################################################################

    if(ppid == 0){ // if proc is init
        new_process->file_descriptors[STDIN] = STDIN;
        new_process->file_descriptors[STDOUT] = STDOUT;
        new_process->file_descriptors[STDERR] = STDERR;
    }
    for(int i = 0; i < MAX_PROCESS; i++){
        if(process_list[i]->pid == ppid){
            new_process->file_descriptors = process_list[i]->file_descriptors;
            break;
        }
    }

    for(int i=0; i< argc; i++){ // ###########################################################################################
        new_process->parameters[i] = argv[i];
    }

    process_list[process_counter] = new_process;

    for(uint32_t i=0; i < priority; i++)
        ready_queue = add_to_ready_queue(new_process);
    
    // FORZAR LA LLAMA A SCHEDULER // TIMERTICK ################################################################################################

    return process_counter;
}

 

queue remove_from_ready_queue(uint32_t pid){
    if(ready_queue->p->pid == pid){
        queue aux = ready_queue;
        ready_queue = ready_queue->next;
        free(aux->p);
        free(aux);
        return ready_queue;
    }
    ready_queue->next = remove_from_ready_queue(pid);
    return ready_queue;
}


uint64_t my_nice(uint64_t pid, uint64_t newPrio){
    if(process_list[pid]->priority < newPrio){ // upgrade
        for(uint32_t i = process_list[pid]->priority; i < newPrio - process_list[pid]->priority; i++){
            ready_queue = add_to_ready_queue(process_list[pid]);
        }
    }
    else if(process_list[pid]->priority > newPrio){ // downgrade
        for(uint32_t i = process_list[pid]->priority; i > newPrio - process_list[pid]->priority; i--){
            ready_queue = remove_from_ready_queue(process_list[pid]->pid);
        }
    }
    process_list[pid]->priority = newPrio;
}

uint64_t my_kill(uint64_t pid){
    process p = process_list[pid];
    p->state = KILLED;
    for(int32_t i =0; i < p->priority; i++)
        ready_queue = remove_from_ready_queue(pid);
    free(p);
}

uint64_t my_block(uint64_t pid){
    process_list[pid]->state = BLOCKED;
    for(int32_t i =0; i < process_list[pid]->priority; i++)
        ready_queue = remove_from_ready_queue(process_list[pid]->pid);
}

uint64_t my_unblock(uint64_t pid){
    process_list[pid]->state = READY;
    for(int32_t i =0; i < process_list[pid]->priority; i++)
        ready_queue = add_to_ready_queue(process_list[pid]);
}

uint64_t my_yield(){
    process_list[my_getpid()]->state = READY;
    for(int32_t i =0; i < process_list[my_getpid()]->priority; i++)
        ready_queue = add_to_ready_queue(process_list[my_getpid()]);
    // FORZAR A SCHEDULE // TIMERTICK ################################################################################################
}

uint64_t my_wait(int64_t pid){
    process_list[my_getpid()]->state = BLOCKED;
    for(int32_t i =0; i < process_list[pid]->priority; i++)
        ready_queue = remove_from_ready_queue(process_list[pid]->pid);

    while(process_list[pid]->state != RUNNING);
    for(int32_t i =0; i < process_list[pid]->priority; i++)
        process_list[my_getpid()]->state = READY;

    return 0;
}

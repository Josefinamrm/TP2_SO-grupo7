#include "processManager.h"


typedef struct{
    uint64_t ss;
    uint64_t rsp;
    uint64_t rflags;
    uint64_t cs;
    uint64_t rip;
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
}initial_stack;

typedef initial_stack * initial_stack_ptr;


process process_array[MAX_PROCESS];
uint8_t process_counter = 0;





uint64_t my_getpid(){
    return current_process->pid;
}


queue_element add_to_ready_queue(process p){ 
    queue_element new = (queue_element) mm_malloc(sizeof(node));
    new->p = p;
    new->next = ready_queue;
    return new;
}

queue_element remove_from_ready_queue(uint64_t pid){
    if(ready_queue->p->pid == pid){
        queue_element aux = ready_queue;
        ready_queue = ready_queue->next;
        free(aux->p);
        free(aux);
        return ready_queue;
    }
    ready_queue->next = remove_from_ready_queue(pid);
    return ready_queue;
}


/* uint64_t my_create_process(uint8_t * name, uint64_t function, uint64_t ppid, uint64_t priority, uint64_t argc, uint8_t ** argv){
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
        if(process_array[i]->pid == ppid){
            new_process->file_descriptors = process_array[i]->file_descriptors;
            break;
        }
    }

    for(int i=0; i< argc; i++){ // ###########################################################################################
        new_process->parameters[i] = argv[i];
    }

    process_array[process_counter] = new_process;

    for(uint32_t i=0; i < priority; i++)
        ready_queue = add_to_ready_queue(new_process);
    
    // FORZAR LA LLAMA A SCHEDULER // TIMERTICK ################################################################################################

    return process_counter;
} */


static uint64_t setup_stack_structure(uint64_t function, uint64_t argc, uint8_t ** argv){
    uint64_t * initial_rsp = (uint64_t *) mm_malloc(PROCESS_STACK_SIZE);
    initial_rsp += PROCESS_STACK_SIZE / sizeof(uint64_t);

    initial_stack_ptr dummy_filler = (initial_stack_ptr) (initial_rsp - sizeof(initial_stack) / sizeof(uint64_t));
    dummy_filler->ss = 0;
    dummy_filler->rsp = (uint64_t) initial_rsp;
    dummy_filler->rflags = 0x202;
    dummy_filler->cs = 0x08;
    dummy_filler->rip = function;
    dummy_filler->rax = 0;
    dummy_filler->rbx = 0;
    dummy_filler->rbp = (uint64_t) initial_rsp;
    
    dummy_filler->rdi = argc;
    dummy_filler->rsi = (uint64_t) argv;
    dummy_filler->rdx = 0;
    dummy_filler->rcx = 0;
    dummy_filler->r8 = 0;
    dummy_filler->r9 = 0;

    dummy_filler->r10 = 0;
    dummy_filler->r11 = 0;
    dummy_filler->r12 = 0;
    dummy_filler->r13 = 0;
    dummy_filler->r14 = 0;
    dummy_filler->r15 = 0;

    return (uint64_t) dummy_filler;
}

// mov rsp, dummy_filler

// después veo que hago en el caso border  ###############################
uint64_t my_create_process(uint8_t * name, uint64_t function, uint64_t ppid, uint64_t priority, uint64_t argc, uint8_t ** argv){
    if(process_counter != MAX_PROCESS){
        process new_process = (process) mm_malloc(sizeof(p));
        new_process->name = name;
        new_process->pid = ++process_counter;
        new_process->ppid = ppid;
        new_process->priority = priority;
        new_process->state = READY;
        // scheduler job? rsp
        new_process->stack_pointer = (uint64_t *)setup_stack_structure(function, argc, argv);

        // Add to ready queue and force scheduler
        ready_queue = add_to_ready_queue(new_process);
        current_process = ready_queue->p;
        // int_20 (?)

        return process_counter;
    }
}



uint64_t my_nice(uint64_t pid, uint64_t newPrio){
    if(process_array[pid]->priority < newPrio){ // upgrade
        for(uint32_t i = process_array[pid]->priority; i < newPrio - process_array[pid]->priority; i++){
            ready_queue = add_to_ready_queue(process_array[pid]);
        }
    }
    else if(process_array[pid]->priority > newPrio){ // downgrade
        for(uint32_t i = process_array[pid]->priority; i > newPrio - process_array[pid]->priority; i--){
            ready_queue = remove_from_ready_queue(process_array[pid]->pid);
        }
    }
    process_array[pid]->priority = newPrio;
}



uint64_t my_kill(uint64_t pid){
    process p = process_array[pid];
    p->state = KILLED;
    for(int32_t i =0; i < p->priority; i++)
        ready_queue = remove_from_ready_queue(pid);
    free(p);
}



uint64_t my_block(uint64_t pid){
    process_array[pid]->state = BLOCKED;
    for(int32_t i =0; i < process_array[pid]->priority; i++)
        ready_queue = remove_from_ready_queue(process_array[pid]->pid);
}



uint64_t my_unblock(uint64_t pid){
    process_array[pid]->state = READY;
    for(int32_t i =0; i < process_array[pid]->priority; i++)
        ready_queue = add_to_ready_queue(process_array[pid]);
}



uint64_t my_yield(){
    process_array[my_getpid()]->state = READY;
    for(int32_t i =0; i < process_array[my_getpid()]->priority; i++)
        ready_queue = add_to_ready_queue(process_array[my_getpid()]);
    // FORZAR A SCHEDULE // TIMERTICK ################################################################################################
}



uint64_t my_wait(int64_t pid){
    process_array[my_getpid()]->state = BLOCKED;
    for(int32_t i =0; i < process_array[pid]->priority; i++)
        ready_queue = remove_from_ready_queue(process_array[pid]->pid);

    while(process_array[pid]->state != RUNNING);
    for(int32_t i =0; i < process_array[pid]->priority; i++)
        process_array[my_getpid()]->state = READY;

    return 0;
}

#include <processManager.h>

#define INIT_PID 1

process current_process;
queue_element ready_queue;
process idle_process;
uint32_t ready_count;

process process_array[MAX_PROCESS];
uint8_t process_counter = 0;

uint64_t my_getpid(){
    return current_process->pid;
}

static uint32_t add_to_ready_queue(process p){ 
    queue_element new = (queue_element) mm_malloc(sizeof(node));
    if(new==NULL){
        return EXIT_FAIL;
    }

    new->p = p;
    new->next = ready_queue;
    ready_count++;
    ready_queue = new;

    return EXIT_SUCCESS;
}

queue_element remove_from_ready_queue(uint64_t pid){
    if(ready_queue->p->pid == pid){
        queue_element aux = ready_queue;
        ready_queue = ready_queue->next;
        mm_free(aux->p);
        mm_free(aux);
        return ready_queue;
    }
    ready_queue->next = remove_from_ready_queue(pid);
    ready_count--;
    return ready_queue;
}


// después veo que hago en el caso border  ###############################
uint64_t my_create_process(uint64_t function, uint64_t ppid, uint64_t priority, uint64_t argc, uint8_t ** argv){
    if(process_counter != MAX_PROCESS){
        process new_process = (process) mm_malloc(sizeof(p));
        new_process->pid = ++process_counter;
        new_process->ppid = ppid;
        new_process->priority = priority;
        new_process->state = READY;
        uint64_t * initial_rsp = (uint64_t *) mm_malloc(PROCESS_STACK_SIZE);
        initial_rsp += PROCESS_STACK_SIZE / sizeof(uint64_t);
        new_process->stack_pointer = (uint64_t *)_setup_stack_structure_asm(initial_rsp, function, argc, argv);
                                                                            // rdi        rsi      rdx  rcx

        add_to_ready_queue(new_process);

        process_array[ppid]->child_list = mm_malloc(sizeof(node));
        process_array[ppid]->child_list->p = new_process;
        process_array[ppid]->child_list->next = NULL;

        return process_counter;
    }
}



void my_nice(uint64_t pid, uint64_t newPrio){
    if(process_array[pid]->priority < newPrio){ // upgrade
        for(uint32_t i = process_array[pid]->priority; i < newPrio - process_array[pid]->priority; i++){
            add_to_ready_queue(process_array[pid]);
        }
    }
    else if(process_array[pid]->priority > newPrio){ // downgrade
        for(uint32_t i = process_array[pid]->priority; i > newPrio - process_array[pid]->priority; i--){
            ready_queue = remove_from_ready_queue(process_array[pid]->pid);
        }
    }
    process_array[pid]->priority = newPrio;
}




uint32_t my_kill(uint64_t pid){
    process p = process_array[pid];

    if(p == NULL) return EXIT_FAIL;
    
    p->state = KILLED;
    for(int32_t i =0; i < p->priority; i++)
        ready_queue = remove_from_ready_queue(p->pid);
    

    // init adopts children
    while(p->child_list != NULL){
        p->child_list->p->ppid = INIT_PID;
        p->child_list = p->child_list->next;
    }
    process_array[INIT_PID]->child_list = p->child_list;

    // free children
    while(p->child_list != NULL){
        mm_free(p->child_list->p);
        queue_element aux = p->child_list;
        p->child_list = p->child_list->next;
        mm_free(aux);
    }
    mm_free(p->child_list);

    return EXIT_SUCCESS;
}



uint32_t my_block(uint64_t pid){

    if(process_array[pid] == NULL) return EXIT_FAIL;    

    process_array[pid]->state = BLOCKED;
    for(int32_t i =0; i < process_array[pid]->priority; i++)
        ready_queue = remove_from_ready_queue(process_array[pid]->pid);

    return EXIT_SUCCESS;
}



uint32_t my_unblock(uint64_t pid){
    if(process_array[pid] == NULL) return EXIT_FAIL;
    
    process_array[pid]->state = READY;
    for(int32_t i =0; i < process_array[pid]->priority; i++)
        add_to_ready_queue(process_array[pid]);
}



void my_yield(){
    _irq00Handler();
}


void my_wait(int64_t pid){

    process p = process_array[pid];

    if(p->child_list == NULL) return;

    queue_element temp = p->child_list;
    while(temp != NULL){
        if(temp->p->state != KILLED) my_yield();

        queue_element aux = temp;
        temp = temp->next;
        mm_free(aux->p);
        mm_free(aux);
    }
    p->child_list = temp;
}


void init_function(){
    current_process = NULL;
    ready_queue = NULL;
    ready_count = 0;

    process idle_process = (process) mm_malloc(sizeof(p));
    idle_process->pid = 0;
    idle_process->ppid = 0;
    idle_process->priority = 1;
    idle_process->state = READY;
    uint64_t * initial_rsp = (uint64_t *) mm_malloc(PROCESS_STACK_SIZE);
    initial_rsp += PROCESS_STACK_SIZE / sizeof(uint64_t);
    char * argv = {NULL};
    idle_process->stack_pointer = (uint64_t *)_setup_stack_structure_asm(initial_rsp, (uint64_t)idle, 0, argv);

    process_array[0] = idle_process;
}

void idle(){
    my_create_process((uint64_t)init_process, 0, 1, 0, 0);
    while(1) haltcpu();
}


void init_process(){
    my_create_process((uint64_t)USERLAND_DIREC, my_getpid(), 0, 0, NULL);
    _irq00Handler();
    my_wait(INIT_PID);
}
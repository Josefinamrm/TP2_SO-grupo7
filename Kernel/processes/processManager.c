#include "processManager.h"

// sería mas facil en el momento de crear el proceso decirle si escribe a la terminal o si escribe a un pipe (agodio)
enum fd {STDIN=0, STDOUT, STDERR};
uint64_t pids_test = 0;

/*--------------------------------------------------------- Process Control Structure ---------------------------------------------------------*/

struct p{

    uint64_t pid;
    uint64_t ppid;
    uint8_t priority;
    uint8_t state;
    uint64_t stack_pointer;

    // file managment
    // uint32_t * file_descriptors;

};

/*--------------------------------------------------------- Ready Process Queue ---------------------------------------------------------*/

typedef struct node{
    process p;
    struct node * next;
}t_node;

struct queue_info{
    t_node * front;
    t_node * rear;
    uint64_t size;
};

/*--------------------------------------------------------- Process Array ---------------------------------------------------------*/

process_queue ready_queue;

process process_array[MAX_PROCESS];

uint8_t process_counter = 0;

/*--------------------------------------------------------- Queue Functions Implementations ---------------------------------------------------------*/

// Initializes queue CHANGE TO MM_MALLOC
process_queue initialize_queue(){
    process_queue queue = (process_queue) malloc(sizeof(struct queue_info));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}


// Adds a process to the end of the queue
void add_process(process_queue queue, process p){
    t_node * new_node = (t_node *) mm_malloc(sizeof(t_node));
    new_node->p = p;
    new_node->next = NULL;

    if(queue->front == NULL){
        queue->front = queue->rear = new_node;
    }
    else{
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    new_node->next = queue->front;
    
    queue->size++;
}

// Removes all or one instance of the process in the queue
static void remove_process(process_queue queue, uint64_t pid, uint64_t remove_all){
    t_node * current = queue->front;
    t_node * prev = queue->rear;
    uint64_t stop = 0;
    do{
        if(current->p->pid == pid){
            if(ready_queue->size > 0){
                if(current == queue->front){
                    queue->front = current->next;
                    queue->rear->next = queue->front;
                }
                else if(current == queue->rear){
                    queue->rear = prev;
                    queue->rear->next = queue->front;
                }
                else{
                    prev->next = current->next;
                }
            }
            else{
                queue->front = queue->rear = NULL;
            }
            queue->size--;
            current->p->priority--;
            if(current->p->priority == 0 || !remove_all){
                stop = 1;
            }
            mm_free(current->p);
            mm_free(current);
        }
        else{
            prev = current;
            current = current->next;
        }
    }while(ready_queue->size > 0 && current != queue->front && !stop);
}


// Removes all instances of the process in the queue
void delete_process(process_queue queue, uint64_t pid){
    remove_process(queue, pid, 1);
}


// Checks if queue is empty, returns 1 if so
uint64_t is_empty(process_queue queue){
    return queue->size == 0;
}


// Backs up rsp register 
static void backup_current_process(uint64_t rsp){
    ready_queue->front->p->stack_pointer = rsp;
    ready_queue->front->p->state = READY;
}


// Sets next running process
static uint64_t setup_next_running_process(){
    ready_queue->rear = ready_queue->front;
    ready_queue->front = ready_queue->front->next;
    ready_queue->front->p->state = RUNNING;
    return ready_queue->front->p->stack_pointer;
}


// Returns next running process rsp from the ready process queue
uint64_t next_running_process(uint64_t current_rsp){
    backup_current_process(current_rsp);
    return setup_next_running_process();
}


// Returns idle process rsp, idle process will have pid 0 for now, must change later;
uint64_t idle_process_rsp(){
    return process_array[0]->stack_pointer;
}


uint64_t is_ready_queue_empty(){
    return is_empty(ready_queue);
}


/*--------------------------------------------------------- Process Syscall Implementations ---------------------------------------------------------*/
/* 

uint64_t my_getpid(){
    return current_process->pid;
}


queue_element add_to_ready_queue(process p){ 
    queue_element new = (queue_element) mm_malloc(sizeof(node));
    new->p = p;
    new->next = ready_queue;
    ready_count++;
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
        new_process->stack_pointer = (uint64_t *)setup_stack_structure_asm(initial_rsp, function, argc, argv);
                                                                            // rdi        rsi      rdx  rcx
        // Add to ready queue and force scheduler
        ready_queue = add_to_ready_queue(new_process);

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
    // FORZAR A SCHEDULE // TIMERTICK ############################################################
}



uint64_t my_wait(int64_t pid){
    process_array[my_getpid()]->state = BLOCKED;
    for(int32_t i =0; i < process_array[pid]->priority; i++)
        ready_queue = remove_from_ready_queue(process_array[pid]->pid);

    while(process_array[pid]->state != RUNNING);
    for(int32_t i =0; i < process_array[pid]->priority; i++)
        process_array[my_getpid()]->state = READY;

    return 0;
}*/

/* 
void init_process(){

    // inicializacion listas

    process idle_process = (process) mm_malloc(sizeof(struct p));
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
    hang();   
} */
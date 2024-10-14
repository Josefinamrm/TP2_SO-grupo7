
#include "processManager.h"
#define INIT_PID 1
uint64_t idle_running;


/*--------------------------------------------------------- Process Control Structure ---------------------------------------------------------*/

struct p{

    uint64_t pid;
    uint64_t ppid;
    uint64_t priority;
    uint64_t state;
    uint64_t stack_pointer;
    struct node * child_list;

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

// Initializes queue and intitializes idle_running to 0
process_queue initialize_queue(){
    process_queue queue = (process_queue) mm_malloc(sizeof(struct queue_info));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    idle_running = 0;
    return queue;
}


// Adds one or all process instances to the end of the queue
void add_process_instance(process_queue queue, process p, uint64_t add_all){
    uint64_t stop = 0;
    uint64_t prio = 0;
    while(prio < p->priority && !stop){
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
        prio++;
        if(!add_all) stop = 1;
    }
}


// Removes all instances of the process in the queue
void add_all_process_instances(process_queue queue, process p){
    add_process_instance(queue, p, 1);
}


// Removes all or one instance of the process in the queue
void remove_process_instance(process_queue queue, uint64_t pid, uint64_t remove_all){
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
void remove_all_process_instances(process_queue queue, uint64_t pid){
    remove_process_instance(queue, pid, 1);
}


// Checks if queue is empty, returns 1 if so
uint64_t is_empty(process_queue queue){
    return queue->size == 0;
}


/*--------------------------------------------------------- Ready Queue Functions ---------------------------------------------------------*/

static uint64_t add_to_ready_queue(process p){ 
    
    add_all_process_instances(ready_queue, p);

    return EXIT_SUCCESS;
}

// VALOR DE RETORNO ??? ################## Removes an instance of the ready_queue
static uint64_t remove_from_ready_queue(uint64_t pid){
    remove_process_instance(ready_queue, pid, 0);
    return EXIT_SUCCESS;
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
    if(idle_running){
        idle_running = 0;
    }else{
        backup_current_process(current_rsp);
    }
    return setup_next_running_process();
}


// Returns idle process rsp, idle process will have pid 0 for now, must change later;
uint64_t idle_process_rsp(){
    idle_running = 1;
    return process_array[0]->stack_pointer;
}


uint64_t is_ready_queue_empty(){
    return is_empty(ready_queue);
}


/*--------------------------------------------------------- Process Syscall Implementations ---------------------------------------------------------*/

uint64_t my_getpid(){
    return ready_queue->front->p->pid;
}


// después veo que hago en el caso border  ###############################
uint64_t my_create_process(uint64_t function, uint64_t ppid, uint64_t priority, uint64_t argc, uint8_t ** argv){
    if(process_counter != MAX_PROCESS){
        _cli();
        process new_process = (process) mm_malloc(sizeof(struct p));
        new_process->pid = ++process_counter;
        new_process->ppid = ppid;
        new_process->priority = priority;
        new_process->state = READY;
        uint64_t * initial_rsp = (uint64_t *) mm_malloc(PROCESS_STACK_SIZE);
        initial_rsp += PROCESS_STACK_SIZE / sizeof(uint64_t);
        new_process->stack_pointer = _setup_stack_structure_asm(initial_rsp, function, argc, argv);
                                                                            // rdi        rsi      rdx  rcx
        add_to_ready_queue(new_process);
        process_array[new_process->pid] = new_process;

        // check ->implementar lista, puede ir igual en mi otro file de tad de lista
        process_array[ppid]->child_list = mm_malloc(sizeof(t_node));
        process_array[ppid]->child_list->p = new_process;
        process_array[ppid]->child_list->next = NULL;

        _sti();
        return process_counter;
    }
    // for now......
    return 0;
}



void my_nice(uint64_t pid, uint64_t new_prio){
    if(process_array[pid]->priority < new_prio){ // upgrade
        for(uint64_t i = process_array[pid]->priority; i > process_array[pid]->priority - new_prio; i--){
            add_to_ready_queue(process_array[pid]);
        }
    }
    else if(process_array[pid]->priority > new_prio){ // downgrade
        for(uint64_t i = process_array[pid]->priority; i > new_prio - process_array[pid]->priority; i--){
            remove_from_ready_queue(process_array[pid]->pid);
        }
    }
    process_array[pid]->priority = new_prio;
}



//check
uint64_t my_kill(uint64_t pid){
    process p = process_array[pid];

    if(p == NULL) return EXIT_FAIL;
    
    p->state = KILLED;
    remove_all_process_instances(ready_queue, pid);
    

    // init adopts children
    while(p->child_list != NULL){
        p->child_list->p->ppid = INIT_PID;
        p->child_list = p->child_list->next;
    }
    process_array[INIT_PID]->child_list = p->child_list;

    // free children
    while(p->child_list != NULL){
        mm_free(p->child_list->p);
        t_node * aux = p->child_list;
        p->child_list = p->child_list->next;
        mm_free(aux);
    }
    mm_free(p->child_list);

    return EXIT_SUCCESS;
}



uint64_t my_block(uint64_t pid){

    if(process_array[pid] == NULL) return EXIT_FAIL;    

    process_array[pid]->state = BLOCKED;
    remove_all_process_instances(ready_queue, pid);

    return EXIT_SUCCESS;
}



uint64_t my_unblock(uint64_t pid){
    if(process_array[pid] == NULL) return EXIT_FAIL;
    
    process_array[pid]->state = READY;
    add_to_ready_queue(process_array[pid]);
    return EXIT_SUCCESS;
}


// Crear una int_20 como tal en assembler que haga int 20h
void my_yield(){
    force_timer_tick();
}


// check
void my_wait(int64_t pid){

    process p = process_array[pid];

    if(p->child_list == NULL) return;
    t_node * temp = p->child_list;
    while(temp != NULL){
        if(temp->p->state != KILLED) my_yield();

        t_node * aux = temp;
        temp = temp->next;
        mm_free(aux->p);
        mm_free(aux);
    }
    p->child_list = temp;
}

// ?
void init_function(){

    ready_queue = initialize_queue();


    // creates idle process
    process idle_process = (process) mm_malloc(sizeof(struct p));
    idle_process->pid = 0;
    idle_process->ppid = 0;
    idle_process->priority = 1;
    idle_process->state = READY;
    uint64_t * initial_rsp = (uint64_t *) mm_malloc(PROCESS_STACK_SIZE);
    initial_rsp += PROCESS_STACK_SIZE / sizeof(uint64_t);
    char * argv = {NULL};
    idle_process->stack_pointer = _setup_stack_structure_asm(initial_rsp, (uint64_t)idle, 0, argv);

    process_array[0] = idle_process;

}

void idle(){
    char * argv = {NULL};
    my_create_process((uint64_t)init_process, 0, 1, 0, argv);
    _idle();
}


void init_process(){
    char * argv = {NULL};
    my_create_process((uint64_t)USERLAND_DIREC, my_getpid(), 1, 0, argv);
    my_yield();
    my_wait(INIT_PID);
}

// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "processManager.h"
#define INIT_PID 1
uint8_t idle_running;
enum State {READY, RUNNING, BLOCKED, KILLED, ZOMBIE};


/*--------------------------------------------------------- Process Control Structure ---------------------------------------------------------*/

struct p{

    char * name;
    int16_t pid;
    int16_t ppid;
    uint8_t priority;
    enum State state;
    uint64_t stack_pointer;
    uint64_t base_pointer;
    struct queue_info * child_list;

    uint8_t foreground; // 1 if process is in foreground, 0 if not
 
};

/*--------------------------------------------------------- Ready Process Queue ---------------------------------------------------------*/

typedef struct node{
    process p;
    struct node * next;
}t_node;

struct queue_info{
    t_node * front;
    t_node * rear;
    uint32_t size;
};

/*--------------------------------------------------------- Process Array ---------------------------------------------------------*/

process_queue ready_queue;

process process_array[MAX_PROCESS];

uint8_t process_counter = 0;

process foreground_process;

/*--------------------------------------------------------- List Functions Implementations ---------------------------------------------------------*/

// Initializes children queue
children_queue initialize_children_queue(){
    children_queue list = (children_queue) mm_malloc(sizeof(struct queue_info));
    list->front = NULL;
    list->rear = NULL;
    list->size = 0;
    return list;
}

// Adds a process to the end of the children queue
void add_child(children_queue queue, process child){
    t_node * newborn = (t_node *) mm_malloc(sizeof(t_node));
    newborn->p = child;
    newborn->next = NULL;

    if(queue->size == 0){
        queue->front = queue->rear = newborn;
    }else{
        queue->rear->next = newborn;
        queue->rear = newborn;
    }
    queue->size++;
}

// Deletes a process from the children queue, but doesn´t free the process
void delete_child(children_queue queue, int16_t pid, uint8_t free_process){
    if(queue->size == 0){
        queue->front = queue->rear = NULL;
    }else{
        t_node * current = queue->front;
        t_node * prev = NULL;
        while(current != NULL && current->p->pid != pid){
            prev = current; 
            current = current->next;
        }
        if(current == NULL){
            return;
        }
        if(current == queue->front){
            queue->front = current->next;
        }
        else if(current == queue->rear){
            queue->rear = prev;
            prev->next = NULL;
        }
        else{
            prev->next = current->next;
        }
        if(free_process){
            process_array[current->p->pid] = NULL;
            mm_free((void *)current->p->base_pointer);
            mm_free((void *)current->p);
            process_counter--;
        }
        mm_free(current);
        queue->size--;
    }
}

// Checks whether the list is empty
uint64_t childless(children_queue queue){
    return queue->size == 0;
}

// Frees children list
void free_children_queue(children_queue queue, uint8_t free_process){
    if(queue->size > 0){
        t_node * current = queue->front;
        t_node * next = NULL;
        while(current != NULL){
            next = current->next;
            delete_child(queue, current->p->pid, free_process);
            current = next;
        }
    }
    mm_free(queue);
}

// Concatenate lists
void adopt_children(children_queue adoptive_p, children_queue children){
    adoptive_p->rear->next = children->front;
    adoptive_p->rear = children->rear;
    adoptive_p->size += children->size;
}

// Adds a process to the end of the queue -> same as add_child 
void enqueue(waiting_processes_queue queue, int16_t pid){
    add_child(queue, process_array[pid]);
}

// Deletes the first process from the queue 
int16_t dequeue(waiting_processes_queue queue){
    if(queue->size == 0)
        return -1;

    t_node * aux = queue->front;
    if(queue->size > 1){
        queue->front = queue->front->next;
        queue->size--;
    }else{
        queue->front = queue->rear = NULL;
        queue->size = 0;
    }
    int16_t pid_to_ret = aux->p->pid;
    mm_free(aux);
    return pid_to_ret;
}

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
void add_process_instance(process_queue queue, process p, uint8_t add_all){
    uint8_t stop = 0;
    uint8_t prio = 0;
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
void remove_process_instance(process_queue queue, int16_t pid, uint8_t remove_all){
    t_node * current = queue->front;
    t_node * prev = queue->rear;
    uint8_t stop = 0;
    do{
        if(current->p->pid == pid){
            if(ready_queue->size > 1){
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
                queue->size--;
            }
            else{
                queue->front = queue->rear = NULL;
                queue->size = 0;
            }
            current->p->priority--;
                if(current->p->priority == 0 || !remove_all){
                    stop = 1;
                }
                mm_free(current);
        }
        else{
            prev = current;
            current = current->next;
        }
    }while(ready_queue->size > 0 && current != queue->front && !stop);
}


// Removes all instances of the process in the queue
void remove_all_process_instances(process_queue queue, int16_t pid){
    remove_process_instance(queue, pid, 1);
}


// Checks if queue is empty, returns 1 if so
uint8_t is_empty(process_queue queue){
    return queue->size == 0;
}



/*--------------------------------------------------------- Ready Queue Functions ---------------------------------------------------------*/

// Adds n process instances, where n = priority
static uint32_t add_to_ready_queue(process p){ 
    
    add_all_process_instances(ready_queue, p);

    return FINISH_SUCCESFULLY;
}

// Removes an instance of the ready_queue
static uint32_t remove_from_ready_queue(int16_t pid){
    if(process_array[pid] == NULL){
        return EXIT_FAILURE;
    }
    uint8_t prio = process_array[pid]->priority;
    remove_process_instance(ready_queue, pid, 0);
    process_array[pid]->priority = prio;
    return FINISH_SUCCESFULLY;
}



// Backs up rsp register 
static void backup_current_process(uint64_t rsp){
    ready_queue->front->p->stack_pointer = rsp;
}


// Sets next running process
static uint64_t setup_next_running_process(){

    // Check if the current process is blocked or killed, if so remove it from the list
    if(ready_queue->front->p->state == BLOCKED || ready_queue->front->p->state == KILLED){
        remove_from_ready_queue(ready_queue->front->p->pid);
        if(ready_queue->size == 0){
            return process_array[0]->stack_pointer;
        }else{
            return ready_queue->front->p->stack_pointer;
        }
    }else{
        // change prior process to ready state
        ready_queue->front->p->state = READY;
    }


    // Move to the next process in the list
    ready_queue->rear = ready_queue->front;
    ready_queue->front = ready_queue->front->next;

    // Check if the next is a blocked or killed process, if so remove it from the list
    if(ready_queue->front->p->state == BLOCKED || ready_queue->front->p->state == KILLED){
        remove_from_ready_queue(ready_queue->front->p->pid);
        if(ready_queue->size == 0){
            return process_array[0]->stack_pointer;
        }
    }

    // Returns current running process
    ready_queue->front->p->state = RUNNING;
    if(ready_queue->front->p->foreground == 1){
        foreground_process = ready_queue->front->p;
    }
    return ready_queue->front->p->stack_pointer;
}


// Returns next running process rsp from the ready process queue
uint64_t next_running_process(uint64_t current_rsp){
    if(idle_running){
        // backs up idle rsp
        process_array[0]->stack_pointer = current_rsp;
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


uint8_t is_ready_queue_empty(){
    return is_empty(ready_queue);
}

// Returns the next available pid
int16_t next_available_pid(){
    for(int i=0; i < MAX_PROCESS; i++){
        if(process_array[i] == NULL){
            return i;
        }
    }
    return FINISH_ON_ERROR;
}


/*--------------------------------------------------------- Process Syscall Implementations ---------------------------------------------------------*/

// Returns the calling process pid
int16_t my_getpid(){
    return ready_queue->front->p->pid;
}


// después veo que hago en el caso border  ###############################  ME QUEDE ACA
int16_t my_create_process(uint64_t function, int16_t ppid, uint8_t priority, uint64_t argc, char ** argv){
    _cli();
    int16_t new_pid = next_available_pid();
    if(new_pid > 0 && argc > 0){
        process new_process = (process) mm_malloc(sizeof(struct p));
        // is this correct? should i memcopy?
        new_process->name = argv[0];
        new_process->pid = new_pid;
        new_process->ppid = ppid;
        new_process->priority = priority;
        new_process->state = READY;
        uint64_t * initial_rsp = (uint64_t *) mm_malloc(PROCESS_STACK_SIZE);
        initial_rsp += PROCESS_STACK_SIZE / sizeof(uint64_t);
        new_process->base_pointer = (uint64_t)initial_rsp;
        new_process->stack_pointer = _setup_stack_structure_asm((uint64_t)initial_rsp, function, argc, (uint64_t)argv);
        new_process->child_list = initialize_children_queue();
                                                              // rdi        rsi      rdx  rcx
        new_process->foreground = (uint8_t)argv[1];
        add_to_ready_queue(new_process);
        process_array[new_process->pid] = new_process;
        add_child(process_array[new_process->ppid]->child_list, new_process);

        process_counter++;
        
    }
    _sti();
    return new_pid;
}


// Exits the current process, killing it
void my_exit(){
    my_kill(my_getpid());
    force_timer_tick();
}

void my_exit_foreground(){
    my_kill(foreground_process->pid);
    force_timer_tick();
}

// Changes process priority
void my_nice(int16_t pid, uint8_t new_prio){
    if(process_array[pid]->priority < new_prio){ // upgrade
        for(int i = process_array[pid]->priority; i > process_array[pid]->priority - new_prio; i--){
            add_to_ready_queue(process_array[pid]);
        }
    }
    else if(process_array[pid]->priority > new_prio){ // downgrade
        for(int i = process_array[pid]->priority; i > new_prio - process_array[pid]->priority; i--){
            remove_from_ready_queue(process_array[pid]->pid);
        }
    }
    process_array[pid]->priority = new_prio;
}


// Kills process -> remove it from process_array ?
// el scheduler se va a encargar de sacarlo de la lista
int16_t my_kill(int16_t pid){
    process p = process_array[pid];

    // if process is not on the list then error
    if(p == NULL) return FINISH_ON_ERROR;
    
    p->state = KILLED;

    // init adopts children, if it has
    t_node * aux = p->child_list->front;
    while(aux != NULL){
        aux->p->ppid = INIT_PID;
        aux = aux->next;
    }
    adopt_children(process_array[INIT_PID]->child_list, p->child_list);

    // set children freeeeee (but not free the process resources, just free them from the list)
    free_children_queue(p->child_list, 0);
    p->child_list = NULL;

    return EXIT_SUCCESS;
}


// Blocks process
int16_t my_block(int16_t pid){

    if(process_array[pid] == NULL) return FINISH_ON_ERROR;    

    process_array[pid]->state = BLOCKED;
    if(ready_queue->front->p->pid == pid) force_timer_tick();

    return EXIT_SUCCESS;
}


// Unblocks process
int16_t my_unblock(int16_t pid){
    if(process_array[pid] == NULL) return FINISH_ON_ERROR;
    
    process_array[pid]->state = READY;
    add_to_ready_queue(process_array[pid]);
    return EXIT_SUCCESS;
}


// Yields cpu usage
void my_yield(){
    force_timer_tick();
}


static void my_wait_process(int16_t pid){

    process p = process_array[my_getpid()];

    if(p->child_list == NULL) return;

    while(1){
        if(process_array[pid]->state == KILLED){
            delete_child(p->child_list, pid, 1);
            return;
        }
        else{
            my_yield();
        }
    }
    

    /* t_node * current = p->child_list->front;
    while(current != NULL && current->p->pid != pid){
        current = current->next;
    }


        while(current->p->state == KILLED){
            delete_child(p->child_list, current->p->pid, 1);

        }
        
            force_timer_tick(); */
    
}

// Waits for all children to die -> free del stack pointer
void my_wait(int16_t pid){

    if(pid != -1){
        my_wait_process(pid);
        return;
    }
    else{
        process p = process_array[my_getpid()];
        if(p->child_list == NULL) return;

        t_node * aux = p->child_list->front;
        while(aux != NULL){
            if(aux->p->state == KILLED){
                t_node * save = aux;
                delete_child(p->child_list, aux->p->pid, 1);
                aux = save->next;
            }
            else{
                my_yield();
            }
        }
    }
}

// Prints processes info -> name, pid and state
void my_ps(){
    char * spacing = "            ";
    printArray("    ");
    printArray("NAME");
    printArray(spacing);
    printArray("PID");
    printArray(spacing);
    printArray("STATE\n");

    uint32_t i = 0;
    uint32_t counter = 0;
    while(counter < process_counter){
        if(process_array[i] != NULL){
            printArray("    ");
            printArray(process_array[i]->name);
            printArray(spacing);
            printDec(process_array[i]->pid);
            printArray(spacing);
            switch (process_array[i]->state){
                case READY:
                    printArray("READY");        
                    break;
                case RUNNING:
                    printArray("RUNNING");
                    break;
                case BLOCKED:
                    printArray("BLOCKED");
                    break;
                case KILLED:
                    printArray("KILLED");
                    break;
                case ZOMBIE:
                    printArray("ZOMBIE");
                    break;
            }   
            putChar('\n');
            counter++;
        }
        i++;
    }
}

/*--------------------------------------------------------- Base Processes and Functions ---------------------------------------------------------*/

// Creates idle process
static void create_idle_process(){
    _cli();
    process idle_process = (process) mm_malloc(sizeof(struct p));
    idle_process->name = "Idle";
    idle_process->pid = 0;
    idle_process->ppid = 0;
    idle_process->priority = 1;
    idle_process->state = READY;
    uint64_t * initial_rsp = (uint64_t *) mm_malloc(PROCESS_STACK_SIZE);
    initial_rsp += PROCESS_STACK_SIZE / sizeof(uint64_t);
    idle_process->base_pointer = (uint64_t)initial_rsp;
    char * argv[] = {idle_process->name, NULL};
    idle_process->stack_pointer = _setup_stack_structure_asm((uint64_t)initial_rsp, (uint64_t)idle, 1, (uint64_t)argv);
    idle_process->child_list = initialize_children_queue();
    process_array[0] = idle_process;
    process_counter++;
    _sti();
}


void init_function(){

    ready_queue = initialize_queue();
    foreground_process = (process) mm_malloc(sizeof(struct p));
    create_idle_process();
}

void idle(){
    char * argv[] = {"Init", NULL};
    my_create_process((uint64_t)init_process, 0, 1, 1, argv);
    _idle();
}

uint64_t test_sync(uint64_t argc, char *argv[]);

void init_process(){
    char * argv[] = {"userland", NULL};
    my_create_process((uint64_t)USERLAND_DIREC, my_getpid(), 1, 1, argv);
    my_wait(-1);
    my_exit();
    /* char * argv1[] = { "proceso_1", "3" ,NULL};
 
    int pid = my_create_process((uint64_t) process_1, my_getpid(), 1, 1, argv1);

    timer_wait(3);
    my_block(pid);
    printArray("despues de bloquearlo\n");
    timer_wait(1);
    my_unblock(pid);
    my_wait(-1);
    my_exit(); */
}


// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "processManager.h"

// Solo para testear pipes:
#include <syscallDispatcher.h>

#define INIT_PID 1
uint8_t idle_running;
typedef enum  {READY = 0, RUNNING, BLOCKED, KILLED, ASLEEP} State;

/*--------------------------------------------------------- Process Control Structure ---------------------------------------------------------*/

struct fd_struct{
    Type type;
    int16_t id;
    uint8_t open;
    Permission permission;
};

struct p{

    char * name;
    int16_t pid;
    int16_t ppid;
    uint8_t priority;
    State state;
    uint64_t stack_pointer;
    uint64_t base_pointer;
    struct queue_info * child_list;
    char * child_processes_sem;
    uint16_t active_child_processes;
    fd file_descriptors[MAX_FD];
    uint8_t foreground; 

};

/*--------------------------------------------------------- Process Queue ---------------------------------------------------------*/

typedef struct node{
    process p;
    struct node * next;
}t_node;

struct queue_info{
    t_node * front;
    t_node * rear;
    uint32_t size;
};

/*--------------------------------------------------------- Sleeping Process Queue ---------------------------------------------------------*/

typedef struct node_2{
    int16_t pid;
    uint64_t sleep_until;
    struct node_2 * next;
}s_node;

struct sleep_queue_info{
    s_node * front;
    s_node * rear;
    uint32_t size;
};

/*--------------------------------------------------------- Process Sets ---------------------------------------------------------*/

process_queue ready_queue;

sleeping_process_queue sleep_queue;

process process_array[MAX_PROCESSES];

uint8_t process_counter = 0;

int16_t last_fg_process_pid;

/*--------------------------------------------------------- Process Queue Functions Implementations ---------------------------------------------------------*/

// Initializes queue
process_queue initialize_process_queue(){
    process_queue queue = (process_queue) mm_malloc(sizeof(struct queue_info));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

// Adds a process to the end of the queue
void add_process(process_queue queue, process child){
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

// Deletes a process from the process queue, it frees it if indicated by free_process
void delete_process(process_queue queue, int16_t pid, uint8_t free_process){
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
            close_all_fds(pid);
            my_sem_close(current->p->child_processes_sem);
            mm_free((void *)current->p->base_pointer);
            mm_free((void *)current->p);
            process_array[current->p->pid] = NULL;
            process_counter--;
        }
        mm_free(current);
        queue->size--;
    }
}



// Checks if queue is empty, returns 1 if so
uint8_t is_empty(process_queue queue){
    return queue->size == 0;
}


// Frees process queue
void free_process_queue(process_queue queue, uint8_t free_process){
    if(queue->size > 0){
        t_node * current = queue->front;
        t_node * next = NULL;
        while(current != NULL){
            next = current->next;
            delete_process(queue, current->p->pid, free_process);
            current = next;
        }
    }
    mm_free(queue);
}

// Concatenates queues
void concat_queues(process_queue adoptive_p, process_queue children){
    adoptive_p->rear->next = children->front;
    adoptive_p->rear = children->rear;
    adoptive_p->size += children->size;
}

// Adds a process to the end of the queue -> same as add_process 
void enqueue_process(process_queue queue, int16_t pid){
    add_process(queue, process_array[pid]);
}

// Deletes the first process from the queue 
int16_t dequeue_process(process_queue queue){
    if(queue->size == 0)
        return FINISH_ON_ERROR;

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

/*--------------------------------------------------------- Process Queue Functions Implementations (Circular) ---------------------------------------------------------*/


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


// Adds all instances of the process in the queue
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

/*--------------------------------------------------------- Ready Queue Function Implementations ---------------------------------------------------------*/

// Initializes queue and intitializes idle_running to 0
process_queue initialize_ready_queue(){
    idle_running = 0;
    return initialize_process_queue();
}


// Adds n process instances, where n = priority
void add_to_ready_queue(process p){ 

    add_all_process_instances(ready_queue, p);
}

// Removes an instance of the ready_queue
void remove_from_ready_queue(int16_t pid){
    if(process_array[pid] == NULL){
        return;
    }
    uint8_t prio = process_array[pid]->priority;
    remove_process_instance(ready_queue, pid, 0);
    process_array[pid]->priority = prio;
}


/*--------------------------------------------------------- Sleep Queue Function Implementations  ---------------------------------------------------------*/

// Initializes sleep queue
process_queue initialize_sleep_queue(){
    initialize_process_queue();
}

// Adds a procces to the sleeping queue
void add_to_sleep_queue(int16_t pid, uint64_t until_ticks){
    s_node * sleeping_beauty = (s_node *) mm_malloc(sizeof(s_node));
    sleeping_beauty->pid = pid;
    sleeping_beauty->sleep_until = until_ticks;
    sleeping_beauty->next = NULL;

    if(sleep_queue->size == 0){
        sleep_queue->front = sleep_queue->rear = sleeping_beauty;
    }else{
        sleep_queue->rear->next = sleeping_beauty;
        sleep_queue->rear = sleeping_beauty;
    }

    sleep_queue->size++;
    go_to_sleep(pid);
}

// Removes a process from the sleeping queue which has slept enough
void remove_from_sleep_queue(uint64_t ticks_elapsed){
    if(sleep_queue->size == 0){
        sleep_queue->front = sleep_queue->rear = NULL;
    }else{
        s_node * current = sleep_queue->front;
        s_node * prev = NULL;
        while(current != NULL){
            if(current->sleep_until < ticks_elapsed){
                if(current == sleep_queue->front){
                    sleep_queue->front = current->next;
                }
                else if(current == sleep_queue->rear){
                    sleep_queue->rear = prev;
                    prev->next = NULL;
                }
                else{
                    prev->next = current->next;
                }
                sleep_queue->size--;
                wake_up(current->pid);
                mm_free(current);
            }
            prev = current; 
            current = current->next;
        }
    
    }
}


// Makes process go to sleep
int16_t go_to_sleep(int16_t pid){

    if(process_array[pid] == NULL) return FINISH_ON_ERROR;    

    process_array[pid]->state = ASLEEP;
    if(ready_queue->front->p->pid == pid) force_timer_tick();

    return EXIT_SUCCESS;
}


// Wakes process up
int16_t wake_up(int16_t pid){
    return my_unblock(pid);
}


// Auxiliary function to remove a process from the sleep queue but indicating pid
void static remove_from_squeue_by_pid(int16_t pid){
    if(sleep_queue->size == 0){
        sleep_queue->front = sleep_queue->rear = NULL;
    }else{
        s_node * current = sleep_queue->front;
        s_node * prev = NULL;
        while(current != NULL && current->pid != pid){
            prev = current; 
            current = current->next;
        }
        if(current == NULL){
            return;
        }
        if(current == sleep_queue->front){
            sleep_queue->front = current->next;
        }
        else if(current == sleep_queue->rear){
            sleep_queue->rear = prev;
            prev->next = NULL;
        }
        else{
            prev->next = current->next;
        }
        mm_free(current);
        sleep_queue->size--;
    }
}

/*--------------------------------------------------------- Scheduler Function Implementations ---------------------------------------------------------*/


// Backs up rsp register 
static void backup_current_process(uint64_t rsp){
    ready_queue->front->p->stack_pointer = rsp;
}


// Sets next running process
static uint64_t setup_next_running_process(){

    // Check if the current process is blocked, killed or asleep, if so remove it from the list
    if(ready_queue->front->p->state == BLOCKED || ready_queue->front->p->state == KILLED || ready_queue->front->p->state == ASLEEP){
        remove_from_ready_queue(ready_queue->front->p->pid);
        if(ready_queue->size == 0){
            return idle_process_rsp();
        }
    }else{
        // change prior process to ready state
        ready_queue->front->p->state = READY;
    }


    // Move to the next process in the list
    ready_queue->rear = ready_queue->front;
    ready_queue->front = ready_queue->front->next;

    // Check if the next is a blocked, asleep or killed process, if so remove it from the list
    if(ready_queue->front->p->state == BLOCKED || ready_queue->front->p->state == KILLED || ready_queue->front->p->state == KILLED){
        remove_from_ready_queue(ready_queue->front->p->pid);
        if(ready_queue->size == 0){
            return idle_process_rsp();
        }
    }

    // Returns current running process and updates the last foreground process pid
    ready_queue->front->p->state = RUNNING;
    if(ready_queue->front->p->foreground == 1){
        last_fg_process_pid = ready_queue->front->p->pid;
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


// Returns idle process rsp
uint64_t idle_process_rsp(){
    idle_running = 1;
    return process_array[0]->stack_pointer;
}


uint8_t is_ready_queue_empty(){
    return is_empty(ready_queue);
}

// Returns the next available pid
static int16_t next_available_pid(){
    for(int i=0; i < MAX_PROCESSES; i++){
        if(process_array[i] == NULL){
            return i;
        }
    }
    return FINISH_ON_ERROR;
}

/*--------------------------------------------------------- File Descriptor Functions Implementations ---------------------------------------------------------*/

static int16_t next_available_fd_number(fd file_descriptors[]){
    for(int i=0; i < MAX_FD; i++){
        if(file_descriptors[i] == NULL){
            return i;
        }
    }
    return FINISH_ON_ERROR;
}



// Opens new file descriptor for a certain pid
int16_t open_fd_for_pid(Type type, Permission permission, int16_t id, int16_t process_pid){
    process p = process_array[process_pid];
    int16_t fd_number = next_available_fd_number(p->file_descriptors);

    if(fd_number >= 0){

        fd new_fd = (fd) mm_malloc(sizeof(struct fd_struct));
        new_fd->open = 1;
        new_fd->permission = permission;
        new_fd->id = id;
        new_fd->type = type;
        p->file_descriptors[fd_number] = new_fd;
        return fd_number;
    }

    return FINISH_ON_ERROR;
}



// Opens new file descriptor
int16_t open_fd(Type type, Permission permission, int16_t id){
    return open_fd_for_pid(type, permission, id, my_getpid());
}



// Closes file descriptor from a certain pid
void close_fd_from_pid(int16_t fd_number, int16_t process_pid){

    if(fd_number >= MAX_FD){
        return;
    }

    fd fd = process_array[process_pid]->file_descriptors[fd_number];
    
    if(fd == NULL){
        return;
    }

    if(fd->type == PIPE){
        close_pipe_fd_end(fd->id, fd->permission);
    }

    mm_free(fd);
    process_array[process_pid]->file_descriptors[fd_number] = NULL;
}



// Closes file descriptor
void close_fd(int16_t fd_number){
    close_fd_from_pid(fd_number, my_getpid());
}




// Closes all file descriptors from a process
int16_t close_all_fds(int16_t pid){
    process p = process_array[pid];

    if(p == NULL){
        return FINISH_ON_ERROR;
    }

    // por ahora, porque puede ser que tenga un fd abierto y en el meido hayan nulls ->probar a darle mas memoria del stack
    for(int i = 0; p->file_descriptors[i] != NULL; i++){
        close_fd_from_pid(i, pid);
    }

    return FINISH_ON_ERROR;
}



// Writes to file descriptor
int64_t write_to_fd(int16_t fd_number, char * buffer, int to_write){
    int16_t fd_type = get_type((int16_t)fd_number);

    if(fd_type == -1 || fd_type == READ){
        return FINISH_ON_ERROR;
    }

    switch (fd_type)
    {
    case STDERR:
        printArrayOfDimWithColor(RED, BLACK, buffer, to_write);
        break;
    
    case PIPE:
        {int16_t pipe_id = get_id(fd_number);
        if(pipe_id != -1){
            return write_pipe(pipe_id, buffer, to_write);
        }}
        break;
    
    default:
        printArrayOfDimWithColor(WHITE, BLACK, buffer, to_write);
        break;
    }
    return to_write;
}



// Reads from file descriptor
int64_t read_from_fd(int16_t fd_number, char * buffer, int to_read){
    int i = 0;
    char c;
    int16_t fd_type = get_type((int16_t)fd_number);

    if(fd_type == -1 || fd_type == WRITE){
        return FINISH_ON_ERROR;
    }

    switch (fd_type){
    case STDIN:
        while(i < to_read){
            c = get_char_from_buffer();
            if(c==EOF) return 0;
            buffer[i++] = c;
     
        }
        break;

    // por ahora, después capaz cambiarlo al fd
    case PIPE:
        {int16_t pipe_id = get_id(fd_number);
        if(pipe_id != -1){
            i = read_pipe(pipe_id, buffer, to_read);
        }}
        break;
    
    default:
        break;
    }

    return i;
}



int16_t get_type(int16_t fd_number){
    fd fd = process_array[my_getpid()]->file_descriptors[fd_number];
    if( fd == NULL ){
        return FINISH_ON_ERROR;
    }
    return fd->type;
}


int16_t get_id(int16_t fd_number){
    fd fd = process_array[my_getpid()]->file_descriptors[fd_number];
    if( fd == NULL ){
        return FINISH_ON_ERROR;
    }
    return fd->id;
}


/*--------------------------------------------------------- Process Syscall Implementations ---------------------------------------------------------*/

// Returns the calling process pid
int16_t my_getpid(){
    return ready_queue->size == 0 ? 0 : ready_queue->front->p->pid;
}


static uint64_t calculate_argc(char ** argv){
    uint64_t argc = 0;
    while(argv[argc] != NULL) argc++;
    return argc;
}

static void initialize_fd(int16_t pid, int16_t ppid, int16_t read_fd, int16_t write_fd){
    fd parent_read_fd = process_array[ppid]->file_descriptors[read_fd];
    fd parent_write_fd = process_array[ppid]->file_descriptors[write_fd];

    open_fd_for_pid(parent_read_fd->type, parent_read_fd->permission, parent_read_fd->id, pid);         // stdin
    open_fd_for_pid(parent_write_fd->type, parent_write_fd->permission, parent_write_fd->id, pid);      // stdout
    open_fd_for_pid(STDERR, WRITE, -1, pid);
}

// después veo que hago en el caso border  ###############################  ME QUEDE ACA

int16_t my_create_process(uint64_t function, char ** argv, uint8_t foreground, int read_fd, int write_fd){

    int16_t new_pid = next_available_pid();
    uint64_t argc = calculate_argc(argv);

    if(new_pid > 0 && argc > 0){
        process new_process = (process) mm_malloc(sizeof(struct p));
        new_process->name = argv[0];
        new_process->pid = new_pid;
        new_process->ppid = my_getpid();
        new_process->priority = DEFAULT_PRIO;
        new_process->state = READY;
        uint64_t * initial_rsp = (uint64_t *) mm_malloc(PROCESS_STACK_SIZE);
        initial_rsp += PROCESS_STACK_SIZE / sizeof(uint64_t);
        new_process->base_pointer = (uint64_t)initial_rsp;
        new_process->stack_pointer = _setup_stack_structure_asm((uint64_t)initial_rsp, function, argc, (uint64_t)argv);
        new_process->child_list = initialize_process_queue();
                                                                           // rdi        rsi      rdx       rcx
        new_process->foreground = foreground;
        process_array[new_pid] = new_process;
        initialize_fd(new_pid, new_process->ppid, read_fd, write_fd);
        add_process(process_array[new_process->ppid]->child_list, new_process);

        new_process->child_processes_sem = (char *) mm_malloc(sizeof(7));
        create_sem_name(new_process->child_processes_sem, "pid", new_pid);
        my_sem_open(new_process->child_processes_sem, 0);
        new_process->active_child_processes = 0;
        process_array[new_process->ppid]->active_child_processes++;

        add_to_ready_queue(new_process);
        process_counter++;
        
    }
    return new_pid;
}


// Exits the current process, killing it
void my_exit(){
    my_kill(my_getpid());
    force_timer_tick();
}

void my_exit_foreground(){
    // CtrlC-ing forbidden for idle (not in fg but anyways), init and shell
    if(last_fg_process_pid <= 2){
        return;
    }
    my_kill(last_fg_process_pid);
}

// Changes process priority
int64_t my_nice(int16_t pid, uint8_t new_prio){

    if(new_prio == 0 || new_prio > 4){
        return FINISH_ON_ERROR;
    }

    if(process_array[pid]->state == READY || process_array[pid]->state == RUNNING){
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
    }
    
    process_array[pid]->priority = new_prio;

    return FINISH_SUCCESFULLY;
}


// Kills process
int16_t my_kill(int16_t pid){
    process p = process_array[pid];

    if(p == NULL) return FINISH_ON_ERROR;

    // If the process was asleep, the process won't be on the ready_queue, so if it was a fg process set fg pid to -1
    // If there was another fg process the scheduler will take care to update the variable in the next run
    if(p->state == ASLEEP && p->foreground){
        last_fg_process_pid = -1;
    }
    
    p->state = KILLED;

    // init adopts children, if it has
    t_node * aux = p->child_list->front;
    while(aux != NULL){
        aux->p->ppid = INIT_PID;
        aux = aux->next;
    }
    concat_queues(process_array[INIT_PID]->child_list, p->child_list);

    // set children freeeeee (but not free the process resources, just free them from the list)
    // and wake it up..
    free_process_queue(p->child_list, 0);
    remove_from_squeue_by_pid(pid);
    p->child_list = NULL;
    process_array[p->ppid]->active_child_processes--;
    my_sem_post(process_array[p->ppid]->child_processes_sem);

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

    while(process_array[pid]->state != KILLED){
        my_sem_wait(p->child_processes_sem);
    }

    delete_process(p->child_list, pid, 1);
    
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


        while(p->active_child_processes){
            my_sem_wait(p->child_processes_sem);
        }

        t_node * aux = p->child_list->front;
        while(aux != NULL){
            if(aux->p->state == KILLED){
                t_node * save = aux->next;
                delete_process(p->child_list, aux->p->pid, 1);
                aux = save;
            }
        }
    }
}

// Prints processes info -> name, pid and state
// Fills the array with each process structure
int64_t get_process_info(process_view processes[MAX_PROCESSES]){
    int new_dim = 0;
    while(process_array[new_dim] != NULL){
        processes[new_dim].name = process_array[new_dim]->name;
        processes[new_dim].pid = process_array[new_dim]->pid;
        processes[new_dim].priority = process_array[new_dim]->priority;
        processes[new_dim].stack_pointer = process_array[new_dim]->stack_pointer;
        switch (process_array[new_dim]->state)
        {
        case RUNNING:
            processes[new_dim].state = "RUNNING";
            break;
        case READY:
            processes[new_dim].state = "READY";
            break;
        case BLOCKED:
            processes[new_dim].state = "BLOCKED";
            break;
        case KILLED:
            processes[new_dim].state = "KILLED";
            break;
        case ASLEEP:
            processes[new_dim].state = "ASLEEP";
            break;
        default:
            break;
        }

        if(process_array[new_dim]->foreground){
            processes[new_dim].foreground = "fg";
        }else{
            processes[new_dim].foreground = "bg";
        }
        new_dim++;
    }
    return new_dim;
}

/*--------------------------------------------------------- Base Processes and Functions ---------------------------------------------------------*/

// Initializes standard file descriptors
void initialize_std_fd(int16_t pid){
    open_fd_for_pid(STDIN, READ, -1, pid);
    open_fd_for_pid(STDOUT, WRITE, -1, pid);
    open_fd_for_pid(STDERR, WRITE, -1, pid);
}

// Creates idle process
static void create_idle_process(){
    process idle_process = (process) mm_malloc(sizeof(struct p));

    idle_process->name = "Idle";
    idle_process->pid = 0;
    idle_process->ppid = 0;
    idle_process->priority = DEFAULT_PRIO;
    idle_process->state = READY;
    
    uint64_t * initial_rsp = (uint64_t *) mm_malloc(PROCESS_STACK_SIZE);
    initial_rsp += PROCESS_STACK_SIZE / sizeof(uint64_t);
    idle_process->base_pointer = (uint64_t)initial_rsp;
    char * argv[] = {idle_process->name, NULL};
    idle_process->stack_pointer = _setup_stack_structure_asm((uint64_t)initial_rsp, (uint64_t)idle, 1, (uint64_t)argv);
    idle_process->child_list = initialize_process_queue();
    process_array[0] = idle_process;

    initialize_std_fd(0);

    idle_process->child_processes_sem = (char *) mm_malloc(sizeof(7));
    create_sem_name(idle_process->child_processes_sem, "pid", 0);
    my_sem_open(idle_process->child_processes_sem, 0);

    process_counter++;
}


void init_function(){
    ready_queue = initialize_ready_queue();
    sleep_queue = initialize_sleep_queue();
    last_fg_process_pid = -1;
    create_idle_process();
}

void idle(){
    char * argv[] = {"Init", NULL};
    my_create_process((uint64_t)init_process, argv, 1, STDIN, STDOUT);
    _idle();
}

void init_process(){
    char * argv[] = {"Shell", NULL};
    my_create_process((uint64_t)USERLAND_DIREC, argv, 1, STDIN, STDOUT);
    my_wait(-1);
    my_exit();
}

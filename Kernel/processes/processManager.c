// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "processManager.h"

// Solo para testear pipes:
#include <syscallDispatcher.h>

#define INIT_PID 1
uint8_t idle_running;
typedef enum  {READY = 0, RUNNING, BLOCKED, KILLED, ZOMBIE} State;

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

// Checks whether the list is empty
uint64_t is_children_queue_empty(children_queue queue){
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


// Checks if queue is empty, returns 1 if so
uint8_t is_empty(process_queue queue){
    return queue->size == 0;
}



/*--------------------------------------------------------- Ready Queue Functions ---------------------------------------------------------*/

// Adds n process instances, where n = priority
static int32_t add_to_ready_queue(process p){ 
    
    add_all_process_instances(ready_queue, p);

    return FINISH_SUCCESFULLY;
}

// Removes an instance of the ready_queue
static int32_t remove_from_ready_queue(int16_t pid){
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
    for(int i=0; i < MAX_PROCESS; i++){
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



// Opens new file descriptor
int16_t open_fd(Type type, Permission permission, int16_t id, int16_t process_pid){

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



// Closes file descriptor
void close_fd(int16_t fd_number){
    int16_t pid = my_getpid();
    fd fd = process_array[pid]->file_descriptors[fd_number];
    
    if(fd_number > MAX_FD || fd == NULL){
        return;
    }

    if(fd->type == PIPE){
        close_fd_end(fd->id, fd->permission);
    }

    mm_free(fd);
    process_array[pid]->file_descriptors[fd_number] = NULL;
}




// Closes all file descriptors from a process
int16_t close_all_fds(int16_t pid){
    process p = process_array[pid];

    if(p == NULL){
        return FINISH_ON_ERROR;
    }

    for(int i = 0; i < MAX_FD; i++){
        close_fd(p->file_descriptors[i]);
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
        int16_t pipe_id = get_id(fd_number);
        if(pipe_id != -1){
            return write_pipe(pipe_id, buffer, to_write);
        }
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
            buffer[i++] = c;
        }
        break;

    // por ahora, después capaz cambiarlo al fd
    case PIPE:
        int16_t pipe_id = get_id(fd_number);
        if(pipe_id != -1){
            i = read_pipe(pipe_id, buffer, to_read);
        }
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

    open_fd(parent_read_fd->type, parent_read_fd->permission, parent_read_fd->id, pid);         // stdin
    open_fd(parent_write_fd->type, parent_write_fd->permission, parent_write_fd->id, pid);      // stdout
    open_fd(STDERR, WRITE, -1, pid);
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
        new_process->child_list = initialize_children_queue();
                                                                           // rdi        rsi      rdx       rcx
        new_process->foreground = foreground;
        process_array[new_pid] = new_process;
        initialize_fd(new_pid, new_process->ppid, read_fd, write_fd);
        add_child(process_array[new_process->ppid]->child_list, new_process);

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
    my_kill(foreground_process->pid);
    force_timer_tick();
}

// Changes process priority
int64_t my_nice(int16_t pid, uint8_t new_prio){

    if(new_prio == 0){
        return FINISH_ON_ERROR;
    }

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

    return FINISH_SUCCESFULLY;
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

    delete_child(p->child_list, pid, 1);

    /* process p = process_array[my_getpid()];

    if(p->child_list == NULL) return;

    while(1){
        if(process_array[pid]->state == KILLED){
            delete_child(p->child_list, pid, 1);
            return;
        }
        else{
            my_yield();
        }
    } */
    
}

// Waits for all children to die -> free del stack pointer
void my_wait(int16_t pid){

    /* if(pid != -1){
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
    } */

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
                delete_child(p->child_list, aux->p->pid, 1);
                aux = save;
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

// Initializes standard file descriptors
void initialize_std_fd(int16_t pid){
    open_fd(STDIN, READ, -1, pid);
    open_fd(STDOUT, WRITE, -1, pid);
    open_fd(STDERR, WRITE, -1, pid);
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
    idle_process->child_list = initialize_children_queue();
    process_array[0] = idle_process;

    initialize_std_fd(0);

    idle_process->child_processes_sem = (char *) mm_malloc(sizeof(7));
    create_sem_name(idle_process->child_processes_sem, "pid", 0);
    my_sem_open(idle_process->child_processes_sem, 0);

    process_counter++;
}


void init_function(){
    ready_queue = initialize_queue();
    foreground_process = (process) mm_malloc(sizeof(struct p));
    create_idle_process();
}

void idle(){
    char * argv[] = {"Init", NULL};
    my_create_process((uint64_t)init_process, argv, 1, STDIN, STDOUT);
    _idle();
}


void read_from_pipe(){
    char buf[10];
    int bytes_read = ksys_read(STDIN, buf, 5);
    printArrayOfDim(buf, bytes_read);
    my_exit();
}

void write_to_pipe(){
    char hola[] = "hola";
    ksys_write(STDOUT, hola, 5); 
    my_exit();
}

void init_process(){
    /* char* argv1[] = {"read_from_pipe", NULL};
    char* argv2[] = {"write_to_pipe", NULL};

    int fds[2];
    if(open_pipe(fds) == -1){
        printArray("error creando pipes");
    }
    my_create_process((uint64_t)read_from_pipe, argv1, 1, fds[0], STDOUT);
    my_create_process((uint64_t) write_to_pipe, argv2, 1, STDIN, fds[1]); */
    char * argv[] = {"userland", NULL};
    my_create_process((uint64_t)USERLAND_DIREC, argv, 1, STDIN, STDOUT);
    my_wait(-1);
    my_exit();
}

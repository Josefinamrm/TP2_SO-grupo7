#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H


#include <stdint.h>
#include <stdlib.h>
#include <memoryManager.h>
#include <interrupts.h>
#include <lib.h>
#include "videoDriver.h"
#include "time.h"

#define MAX_PROCESS 200
#define PROCESS_STACK_SIZE 4096
#define FINISH_SUCCESFULLY 0
#define FINISH_ON_ERROR -1


// IDEA: CAMBIAR LA IMPL DE LISTA Y PROCESO A OTRO FILE NO POR AHORA

// sería mas facil en el momento de crear el proceso decirle si escribe a la terminal o si escribe a un pipe (agodio)
enum fd {STDIN=0, STDOUT, STDERR};

typedef struct queue_info * children_list;

typedef struct queue_info * process_queue;

typedef struct p * process;

/*--------------------------------------------------------- List Functions  ---------------------------------------------------------*/

// Initializes children list
children_list initialize_children_list();

// Adds a process to the end of the children list
void add_child(children_list list, process child);

// Deletes a process from the children list
void delete_child(children_list list, uint64_t pid);

// Checks whether the list is empty
uint64_t childless(children_list list);

// Frees children list
void set_for_adoption(children_list list);

// Concatenate lists
void adopt_children(children_list adoptive_p, children_list children);

/*--------------------------------------------------------- Queue Functions ---------------------------------------------------------*/

// Initializes queue
process_queue initialize_queue();


// Adds a process to the end of the queue
void add_process_instance(process_queue queue, process p, uint64_t add_all);


// Removes all or one instance of the process in the queue
void remove_process_instance(process_queue queue, uint64_t pid, uint64_t remove_all);


// Removes all instances of the process in the queue
void remove_all_process_instances(process_queue queue, uint64_t pid);


// Checks if queue is empty, returns 1 if so
uint64_t is_empty(process_queue queue);


// Returns next running process rsp from the ready process queue
uint64_t next_running_process(uint64_t current_rsp);


// Returns idle process rsp
uint64_t idle_process_rsp();


// Returns wether ready queue is empty (1) or not (0)
uint64_t is_ready_queue_empty();

/*--------------------------------------------------------- Syscalls ---------------------------------------------------------*/

// Returns the pid of the current process
uint64_t my_getpid();

// Creates a new process
int64_t my_create_process(uint64_t function, uint64_t ppid, uint64_t priority, uint64_t argc, uint8_t ** argv);

// Exits the current process, killing it
void my_exit();

// Changes process priority
void my_nice(uint64_t pid, uint64_t newPrio);

// Kills process
int64_t my_kill(uint64_t pid);

// Blocks process
int64_t my_block(uint64_t pid);

// Unblocks process
int64_t my_unblock(uint64_t pid);

// Yields cpu usage
void my_yield();

// Waits for all children to finish
void my_wait();

// Prints processes info -> name, pid and state
void my_ps();

/*--------------------------------------------------------- Base Processes and Functions ---------------------------------------------------------*/

void init_function();

void init_process();

void idle();


#endif
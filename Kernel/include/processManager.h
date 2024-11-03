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

typedef struct queue_info * children_queue;

typedef struct queue_info * process_queue;

typedef struct queue_info * waiting_processes_queue;

typedef struct p * process;

/*--------------------------------------------------------- Process List Functions  ---------------------------------------------------------*/

// Initializes children queue
children_queue initialize_children_queue();

// Adds a process to the end of the children queue
void add_child(children_queue queue, process child);

// Deletes a process from the children queue, but doesn´t free the process
void delete_child(children_queue queue, int16_t pid, uint8_t free_process);

// Checks whether the queue is empty
uint64_t childless(children_queue queue);

// Frees children queue
void free_children_queue(children_queue queue, uint8_t free_process);

// Concatenate queues
void adopt_children(children_queue adoptive_p, children_queue children);

// Adds a process to the end of the queue -> same as add_child 
void enqueue(waiting_processes_queue queue, int16_t pid);

// Deletes the first process from the queue from the list, returns its pid
int16_t dequeue(waiting_processes_queue queue);

/*--------------------------------------------------------- Process Queue Functions ---------------------------------------------------------*/

// Initializes queue
process_queue initialize_queue();


// Adds a process to the end of the queue
void add_process_instance(process_queue queue, process p, uint8_t add_all);


// Removes all or one instance of the process in the queue
void remove_process_instance(process_queue queue, int16_t pid, uint8_t remove_all);


// Removes all instances of the process in the queue
void remove_all_process_instances(process_queue queue, int16_t pid);


// Checks if queue is empty, returns 1 if so
uint8_t is_empty(process_queue queue);


// Returns next running process rsp from the ready process queue
uint64_t next_running_process(uint64_t current_rsp);


// Returns idle process rsp
uint64_t idle_process_rsp();


// Returns wether ready queue is empty (1) or not (0)
uint8_t is_ready_queue_empty();

// Returns the next available pid
int16_t next_available_pid();

/*--------------------------------------------------------- Syscalls ---------------------------------------------------------*/

// Returns the pid of the current process
int16_t my_getpid();

// Creates a new process
int16_t my_create_process(uint64_t function, int16_t ppid, uint8_t priority, uint64_t argc, char ** argv);

// Exits the current process, killing it
void my_exit();

// Changes process priority
void my_nice(int16_t pid, uint8_t newPrio);

// Kills process
int16_t my_kill(int16_t pid);

// Blocks process
int16_t my_block(int16_t pid);

// Unblocks process
int16_t my_unblock(int16_t pid);

// Yields cpu usage
void my_yield();

// Waits for all children to finish
void my_wait(int16_t pid);

// Prints processes info -> name, pid and state
void my_ps();

/*--------------------------------------------------------- Base Processes and Functions ---------------------------------------------------------*/

void init_function();

void init_process();

void idle();


#endif
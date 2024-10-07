#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H


#include <stdint.h>
#include <stdlib.h>
#include "memoryManager.h"

#define MAX_PROCESS 200
#define PROCESS_STACK_SIZE 4096

// sería mas facil en el momento de crear el proceso decirle si escribe a la terminal o si escribe a un pipe (agodio)
enum fd {STDIN=0, STDOUT, STDERR};

enum State {READY, RUNNING, BLOCKED, KILLED, ZOMBIE};



/*--------------------------------------------------------- Process Control Structure ---------------------------------------------------------*/
typedef struct {

    uint8_t * name;
    uint64_t pid;
    uint64_t ppid;
    uint8_t priority;
    uint8_t state;
    uint64_t * stack_pointer;

    // file managment
    // uint32_t * file_descriptors;

} p ;
typedef p * process;


/*--------------------------------------------------------- Process Control Variables ---------------------------------------------------------*/

typedef struct node{
    process p;
    struct node * next;
}node;
typedef node * queue_element;


process current_process = NULL;
queue_element ready_queue = NULL;


/*--------------------------------------------------------- Process Control Functions ---------------------------------------------------------*/


/* // Inserts at the beginning of the list
void add_to_ready_queue(process p);

// Removes all instances of the process from the ready queue
void remove_from_ready_queue(uint64_t pid); */

uint64_t _setup_stack_structure_asm(uint64_t top_stack, uint64_t function, uint64_t argc, uint8_t ** argv);

/*--------------------------------------------------------- Syscalls ---------------------------------------------------------*/
uint64_t my_getpid();
uint64_t my_create_process(uint8_t * name, uint64_t function, uint64_t ppid, uint64_t priority, uint64_t argc, uint8_t ** argv);
uint64_t my_nice(uint64_t pid, uint64_t newPrio);
uint64_t my_kill(uint64_t pid);
uint64_t my_block(uint64_t pid);
uint64_t my_unblock(uint64_t pid);
uint64_t my_yield();
uint64_t my_wait(int64_t pid);



#endif
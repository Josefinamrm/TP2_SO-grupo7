#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H


#include <stdint.h>
#include <stdlib.h>
#include <memoryManager.h>
#include <interrupts.h>

#define MAX_PROCESS 200
#define PROCESS_STACK_SIZE 4096

#define EXIT_FAIL -1
#define EXIT_SUCCESS 0

// sería mas facil en el momento de crear el proceso decirle si escribe a la terminal o si escribe a un pipe (agodio)
enum fd {STDIN=0, STDOUT, STDERR};

enum State {READY, RUNNING, BLOCKED, KILLED, ZOMBIE};



/*--------------------------------------------------------- Process Control Structure ---------------------------------------------------------*/
typedef struct node{
    struct pr * p;
    struct node * next;
}node;
typedef node * queue_element;

typedef struct pr {
    uint64_t pid;
    uint64_t ppid;
    uint8_t priority;
    uint8_t state;
    uint64_t * stack_pointer;

    queue_element child_list;
} p ;
typedef p * process;


/*--------------------------------------------------------- Process Control Variables ---------------------------------------------------------*/


extern process current_process;
extern queue_element ready_queue;
extern process idle_process;
extern uint32_t ready_count;

uint64_t my_getpid();
uint64_t my_create_process(uint64_t function, uint64_t ppid, uint64_t priority, uint64_t argc, uint8_t ** argv);
void my_nice(uint64_t pid, uint64_t newPrio);
uint32_t my_kill(uint64_t pid);
uint32_t my_block(uint64_t pid);
uint32_t my_unblock(uint64_t pid);
void my_yield();
void my_wait(int64_t pid);

void init_function();

void init_process();
void idle();

#endif
#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H


#include <stdint.h>

#define MAX_PROCESS 100

enum State {READY, RUNNING, BLOCKED, KILLED, ZOMBIE};

typedef p * process;


uint64_t my_getpid();
uint64_t my_create_process(char *name);
uint64_t my_nice(uint64_t pid, uint64_t newPrio);
uint64_t my_kill(uint64_t pid);
uint64_t my_block(uint64_t pid);
uint64_t my_unblock(uint64_t pid);
uint64_t my_yield();
uint64_t my_wait(int64_t pid);


#endif
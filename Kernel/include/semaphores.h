#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <lib.h>
#include <processManager.h>

#define MAX_SEM 400

typedef struct{
    char * name;
    int * value;
} sem_t; 


typedef struct{
    sem_t * sem;
    uint8_t count;
    waiting_processes_queue wp_queue;
    uint8_t lock;
} sem_block;

// return 0 on creation, 1 on open, -1 on error
uint8_t sem_open(char * name, int value);

void sem_close(char * name);

void sem_post(char * name);

void sem_wait(char * name);



#endif
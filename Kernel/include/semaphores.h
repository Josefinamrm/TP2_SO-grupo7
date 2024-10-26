#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <lib.h>
#include <processManager.h>

typedef struct{
    char * name;
    int value;
    waiting_processes_queue wp_queue;
    uint8_t lock;
} sem_t; 

void sem_open(char * name, int value);

void sem_close(char * name);

void sem_post(sem_t * semaphore);

void sem_wait(sem_t * semaphore);


#endif
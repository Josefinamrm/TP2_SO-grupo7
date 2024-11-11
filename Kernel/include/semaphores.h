#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <lib.h>
#include <processManager.h>
#include <standard_types.h>
#include <stringsLib.h>
#define MAX_SEM 400

typedef struct{
    char * name;
    int value;
} sem_t;

typedef struct{
    sem_t * sem;
    process_queue wp_queue;
    uint16_t times_opened;
    uint8_t lock;
} sem_block;


// Opens semaphore if it exists, if not it creates it and opens it
int16_t my_sem_open(char * name, int value);

// Closes semaphore
void my_sem_close(char * name);

// Increments the value of the sempahore
void my_sem_post(char * name);

// Decrements the value of the semaphore, blocks if it goes "below" 0
void my_sem_wait(char * name);

// Creates semaphore name as "name" + number to serve as a unique id. Copies it to sem_name
void create_sem_name(char * sem_name, const char * name, int16_t number);

#endif
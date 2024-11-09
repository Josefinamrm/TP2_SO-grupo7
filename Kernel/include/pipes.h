#ifndef PIPES_H
#define PIPES_H

#include <stdint.h>
#include "semaphores.h"
#include "processManager.h"

#define BUFFER_SIZE 4096
#define MAX_PIPES 200

#include <memoryManager.h>
typedef struct pipe_struct * pipes;


// Receives an array of integers representing file desciptors, fd[0] is for reading and fd[1] is for writing
// On success, returns the pipe id, on error returns -1
int16_t open_pipe(uint8_t file_descriptors[2]);

// Optional ? if all fd referring to pipe are closed then close pipe -> counter ?
int16_t close_pipe(int16_t pipe_id);

// Blocking write -> if full block, returns n of characters written ?
int16_t write_pipe(int16_t pipe_id, char * buf, int to_write);

// Blocking read -> if empty block, returns n of characters read ?
int16_t read_pipe(int16_t pipe_id, char * buf, int to_read);

#endif
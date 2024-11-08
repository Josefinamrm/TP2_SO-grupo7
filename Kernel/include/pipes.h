#ifndef PIPES_H
#define PIPES_H

#include <stdint.h>
#include <processManager.h>
#include <semaphores.h>

#define PIPE_SIZE 4096
#define MAX_PIPES 200

typedef struct pipe_struct * pipe_ptr;


// Receives an array of integers representing file desciptors, fd[0] is for reading and fd[1] is for writing
// On success, returns the pipe id, on error returns -1
int16_t open_pipe(int file_descriptors[2]);

// Optional ? if all fd referring to pipe are closed then close pipe -> counter ?
int16_t close_pipe(int16_t pipe_id);

// Blocking write -> if full block, returns n of characters written ?
int16_t write_pipe(int16_t pipe_id, char * buf, int to_write);

// Blocking read -> if empty block, returns n of characters read ?
int16_t read_pipe(int16_t pipe_id, char * buf, int to_read);

// Increases file descriptor and returns its new value, on error returns -1
void increase_fds(int16_t pipe_id, Permission permission);

// Decreases file descriptor and returns its new value, on error returns -1
void decrease_fds(int16_t pipe_id, Permission permission);

#endif
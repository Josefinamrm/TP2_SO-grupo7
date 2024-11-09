#ifndef PIPES_H
#define PIPES_H

#include <stdint.h>
#include <processManager.h>
#include <standard_types.h>
#include <semaphores.h>

#define PIPE_SIZE 4096
#define MAX_PIPES 200

typedef struct pipe_struct * pipe_ptr;


// Receives an array of integers representing file desciptors, fd[0] is for reading and fd[1] is for writing
// On success, returns 0, on error returns -1
int16_t open_pipe(int file_descriptors[2]);

// Closes pipe and associates file descriptors
int16_t close_pipe(int16_t pipe_id);

// Blocking write, returns number of bytes written. On error returns -1
int16_t write_pipe(int16_t pipe_id, char * buf, int to_write);

// Blocking read , returns number of bytes read. On error returns -1 and upon reading eof returns 0
int16_t read_pipe(int16_t pipe_id, char * buf, int to_read);

// Closes pipe end, indicated in permission
void close_fd_end(int16_t pipe_id, Permission permission);

#endif
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "pipes.h"

struct pipe_struct{
    char * buffer;
    uint16_t read_index;
    uint16_t write_index;
    char * full_slots_sem;
    char * empty_slots_sem;
    int16_t read_fd;
    int16_t write_fd;
    // por ahora
    uint32_t dim;
};


pipe_ptr pipe_array[MAX_PIPES];



static int16_t next_available_pipe_number(){
    for(int i=0; i < MAX_PIPES; i++){
        if(pipe_array[i] == NULL){
            return i;
        }
    }
    return FINISH_ON_ERROR;
}



static char read_char(pipe_ptr pipe){
    char to_ret = pipe->buffer[pipe->read_index++];
    if(pipe->read_index >= PIPE_SIZE){
        pipe->read_index = 0;
    }
    return to_ret;
}

static void write_char(pipe_ptr pipe, char character){
    pipe->buffer[pipe->write_index++] = character;
    if(pipe->write_index >= PIPE_SIZE){
        pipe->write_index = 0;
    }
}



// Opens pipe for a certain pid
int16_t open_pipe_for_pid(int file_descriptors[2], int16_t pid){
    int16_t pipe_number = next_available_pipe_number();
    
    if(pipe_number >= 0){
        pipe_ptr new_pipe = (pipe_ptr) mm_malloc(sizeof(struct pipe_struct));
        new_pipe->buffer = (char *) mm_malloc(PIPE_SIZE);
        new_pipe->read_index = new_pipe->write_index = 0;

        new_pipe->empty_slots_sem = (char *) mm_malloc(9);
        new_pipe->full_slots_sem = (char *) mm_malloc(9);
        create_sem_name(new_pipe->empty_slots_sem, "empty", pipe_number);
        my_sem_open(new_pipe->empty_slots_sem, PIPE_SIZE);
        create_sem_name(new_pipe->full_slots_sem, "full", pipe_number);
        my_sem_open(new_pipe->full_slots_sem, 0);

        new_pipe->dim = 0;

        new_pipe->read_fd = file_descriptors[0] = open_fd_for_pid(PIPE, READ, pipe_number, pid);
        new_pipe->write_fd = file_descriptors[1] = open_fd_for_pid(PIPE, WRITE, pipe_number, pid);

        pipe_array[pipe_number] = new_pipe;
        return FINISH_SUCCESFULLY;
    }
    return FINISH_ON_ERROR;
}



// Receives an array of integers representing file desciptors, fd[0] is for reading and fd[1] is for writing
// On success, returns 0, on error returns -1
int16_t open_pipe(int file_descriptors[2]){
    return open_pipe_for_pid(file_descriptors, my_getpid());
}



// Closes all file descriptors from a certain pid
int16_t close_pipe_from_pid(int16_t pipe_id, int16_t pid){
    pipe_ptr pipe = pipe_array[pipe_id];

    if(pipe == NULL){
        return FINISH_ON_ERROR;
    }

    close_fd_from_pid(pipe->read_fd, pid);
    close_fd_from_pid(pipe->write_fd, pid);
    mm_free((void *)pipe->buffer);
    my_sem_close(pipe->empty_slots_sem);
    my_sem_close(pipe->full_slots_sem);
    mm_free((void *)pipe->empty_slots_sem);
    mm_free((void *)pipe->full_slots_sem);
    mm_free((void *)pipe);
    pipe_array[pipe_id] = NULL;
    return FINISH_SUCCESFULLY;
}



// Closes pipe and associated file descriptors
int16_t close_pipe(int16_t pipe_id){
    return close_pipe_from_pid(pipe_id, my_getpid());
}


// Blocking write, returns number of bytes written. On error returns -1
int16_t write_pipe(int16_t pipe_id, char * buf, int to_write){
    pipe_ptr pipe = pipe_array[pipe_id];

    if(pipe == NULL || pipe->read_fd == -1){
        return FINISH_ON_ERROR;
    }

    int i = 0;

    while(i < to_write && pipe->dim <= PIPE_SIZE){
        my_sem_wait(pipe->empty_slots_sem);
        write_char(pipe, buf[i++]);
        pipe->dim++;
        my_sem_post(pipe->full_slots_sem);
    }

    return i;
}



// Blocking read , returns number of bytes read. On error returns -1 and upon reading eof returns 0
int16_t read_pipe(int16_t pipe_id, char * buf, int to_read){
    pipe_ptr pipe = pipe_array[pipe_id];

    if(pipe == NULL){
        return FINISH_ON_ERROR;
    }
    
    if(pipe->dim == 0 && pipe->write_fd == -1){
        return FINISH_SUCCESFULLY;
    }

    int i = 0;

    do{
        my_sem_wait(pipe->full_slots_sem);
        buf[i++] = read_char(pipe);
        pipe->dim--;
        my_sem_post(pipe->empty_slots_sem);
    }while(i < to_read && pipe->dim > 0);

    return i;
}



// Closes pipe end, indicated in permission
void close_pipe_fd_end(int16_t pipe_id, Permission permission){
    pipe_ptr pipe = pipe_array[pipe_id];

    if(pipe == NULL){
        return;
    }

    switch (permission)
    {
    case READ:
        if(pipe->read_fd == -1)
            return;
        pipe->read_fd = -1;
        break;
    
    case WRITE:
        if(pipe->write_fd == -1)
            return;
        pipe->write_fd = -1;
        break;
    }
}
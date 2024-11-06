#include "pipes.h"

struct pipe_struct{
    char * buffer;
    uint16_t read_index;
    uint16_t write_index;
    // yo me encargo de última de la parte de block
    // semaphore -> value == filled_slots  -> read: if filled_Slots == 0 block
    // semaphore -> value == empty_slots -> write : if empty_slots == 0 block
    char * filled_slots_sem;
    char * empty_slots_sem;
};

pipe pipe_array[MAX_PIPES];


static int16_t next_available_pipe_number(){
    for(int i=0; i < MAX_PIPES; i++){
        if(pipe_array[i] == NULL){
            return i;
        }
    }
    return FINISH_ON_ERROR;
}


// Receives an array of integers representing file desciptors, fd[0] is for reading and fd[1] is for writing
// On success, returns the pipe id, on error returns -1
int16_t open_pipe(uint8_t file_descriptors[2]){
    int16_t pipe_number = next_available_pipe_number();

    if(pipe_number > 0){
        pipe new_pipe = (pipe) mm_malloc(sizeof(struct pipe_struct));
        new_pipe->buffer = (char *) mm_malloc(BUFFER_SIZE);
        new_pipe->read_index = new_pipe->write_index = 0;
        // semaphores ?
        pipe_array[pipe_number] = new_pipe;

        return pipe_number;
    }
    return FINISH_ON_ERROR;
}



// Optional : if all fd referring to pipe are closed then close pipe -> counter ?
int16_t close_pipe(int16_t pipe_id);



// Blocking write -> if full block, returns n of characters written ?
int16_t write_pipe(int16_t pipe_id, char * buf, int to_write);



// Blocking read -> if empty block, returns n of characters read ?
int16_t read_pipe(int16_t pipe_id, char * buf, int to_read);
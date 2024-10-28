// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <semaphores.h>

sem_block * sem_array[MAX_SEM] = {0};

uint16_t sem_counter = 0;

// Returns next available id for sempahores
static int16_t next_available_position(){
    uint8_t found = 0;
    int16_t to_ret = -1;
    for(int i = 0; i < MAX_SEM && !found; i++){
        if(sem_array[i] == NULL){
            found = 1;
            to_ret = i;
        }
    }
    return to_ret;
}



// Compares 2 strings
static int strcmp(const char *s1, const char *s2){
    while (*s1 == *s2++) {
        if (*s1++ == 0)
			return 0;
    }
	return (*s1 - *--s2);
}



// Checks whether semaphore exists, if it exists it returns the id else returns -1
static int16_t get_sem_id(char * name){
    uint16_t id = -1;
    for(int i = 0, j = sem_counter; i < MAX_SEM && j > 0; i++){
        if(sem_array[i] != NULL){
            if(strcmp(name, sem_array[i]->sem->name) == 0){
                return i;
            }else{
                j--;
            }
        }
    }
    return id;
}



// Opens semaphore if it exists, if not it creates it and opens it
int16_t my_sem_open(char * name, int value){
    
    if(name == NULL || value < 0 || sem_counter == MAX_SEM){
        return FINISH_ON_ERROR;
    }

    int16_t sem_block_id = get_sem_id(name);
    sem_block * new_sem_block;

    if(sem_block_id == -1){
        uint8_t sem_id = next_available_position();
        new_sem_block = (sem_block *) mm_malloc(sizeof(sem_block));
        new_sem_block->sem = (sem_t *) mm_malloc(sizeof(sem_t));

        new_sem_block->sem->name = name;
        new_sem_block->sem->value = value;
        new_sem_block->lock = 1;
        new_sem_block->wp_queue = initialize_children_queue();          // change maybe
        new_sem_block->times_opened = 1;
        sem_array[sem_id] = new_sem_block;
        sem_counter++;
    }else{
        sem_array[sem_block_id]->times_opened++;
    }
    
    return FINISH_SUCCESFULLY;
}



// Closes sempahore and "deletes" it
void my_sem_close(char * name){

    int16_t sem_block_id = get_sem_id(name);

    if(sem_block_id == -1){
        return ;
    }

    if(sem_array[sem_block_id]->times_opened > 1){
        sem_array[sem_block_id]->times_opened--;
    }else{
        mm_free(sem_array[sem_block_id]->sem);
        free_children_queue(sem_array[sem_block_id]->wp_queue, 0);
        mm_free(sem_array[sem_block_id]);
        sem_array[sem_block_id] = NULL;
    }
    return ;
}



// Increments the value of the sempahore
void my_sem_post(char * name){
    int16_t sem_block_id = get_sem_id(name);

    if(sem_block_id != -1){
        acquire(&(sem_array[sem_block_id]->lock));
        if(sem_array[sem_block_id]->sem->value == 0){
            int16_t pid = dequeue(sem_array[sem_block_id]->wp_queue);
            my_unblock(pid);
        }
        sem_array[sem_block_id]->sem->value++;
        release(&(sem_array[sem_block_id]->lock));
    }
}



// Decrements the value of the semaphore, blocks if it goes "below" 0
void my_sem_wait(char * name){

    int16_t sem_block_id = get_sem_id(name);

    if(sem_block_id != -1){
        acquire(&(sem_array[sem_block_id]->lock));
        if(sem_array[sem_block_id]->sem->value == 0){
            int16_t pid = my_getpid();
            enqueue(sem_array[sem_block_id]->wp_queue, pid);
            release(&(sem_array[sem_block_id]->lock));
            my_block(pid);          // ??
        }         
        sem_array[sem_block_id]->sem->value--;
        release(&(sem_array[sem_block_id]->lock));
    }
}


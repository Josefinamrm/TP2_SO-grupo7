#include <semaphores.h>


sem_block sem_array[MAX_SEM] ={0};
int sem_count = 0;

static uint16_t strcmp(const char * s1, const char * s2){
    while (*s1 == *s2++) {
        if (*s1++ == 0)
			return 0;
    }
	return (*s1 - *--s2);
}

static uint32_t search_sem(char * name){     // change 
    for(int i = 0; i < sem_count; i++){
        if(strcmp(sem_array[i].sem->name, name) == 0){
            return i;
        }
    }
    return -1;
}

uint8_t sem_open(char * name, int value){

    if(name == NULL || value < 0 || sem_count == MAX_SEM){
        return -1;
    }

    if(search_sem(name) == -1){
        sem_t * semaphore = (sem_t *) mm_malloc(sizeof(sem_t));
        semaphore->name = name;
        semaphore->value = value;

        sem_array[sem_count].sem = semaphore;
        sem_array[sem_count].count = 1;
        sem_array[sem_count].lock = 1;
        sem_array[sem_count].wp_queue = initialize_children_queue();          // change maybe

        sem_count++;
        return 0;
        
    }

    sem_array[search_sem(name)].count++;
    return 1;
}

void sem_close(char * name){
    uint8_t pos = search_sem(name);

    if(pos == -1){
        return;
    }

    if(sem_array[pos].count > 1){
        sem_array[pos].count--;
    }
    else{
        mm_free(sem_array[pos].sem);
        sem_array[pos].sem = NULL; // VER QUE HAGO CON ESTOS NULL
        sem_array[pos].count = 0;
        sem_array[pos].wp_queue = NULL;
    }
}


void sem_post(char * name){
    uint8_t pos = search_sem(name);
    if(pos == -1){
        return;
    }

    acquire(sem_array[pos].lock);

    if(sem_array[pos].sem->value == 0 && !is_empty(sem_array[pos].wp_queue)){
        int16_t pid = dequeue(sem_array[pos].wp_queue);
        my_unblock(pid);
    }
    else{
        sem_array[pos].sem->value++;
    }

    acquire(sem_array[pos].lock);
    release(sem_array[pos].lock);
}

void sem_wait(char * name){

    uint8_t pos = search_sem(name);
    if(pos == -1){
        return;
    }

    acquire(sem_array[pos].lock);

    if(sem_array[pos].sem->value == 0){
        int16_t pid = my_getpid();
        my_block(pid);
        enqueue(sem_array[pos].wp_queue, pid);
    }
    else{
        sem_array[pos].sem->value--;
    }

    release(sem_array[pos].lock);
}


// void sem_post(sem_t * semaphore){
//     acquire(semaphore->lock);
//     semaphore->value++;
//     int16_t pid = dequeue(semaphore->wp_queue);
//     my_unblock(pid);
//     acquire(semaphore->lock);
// }


// void sem_wait(sem_t * semaphore){
//     acquire(semaphore->lock);
//     if(semaphore->value == 0){
//         int16_t pid = my_getpid();
//         my_block(pid);          // ??
//         enqueue(semaphore->wp_queue, pid);
//     }
//     else{
//         semaphore->value--;
//     }
//     release(semaphore->lock);
// }

/* tendria que hacer de alguna forma que, cuando yo me bloqueo en un semaforo me pongo en la lista de espera de ese
    semaforo
    por lo general siempre va a haber un proceso que cree el semaforo y que otro lo abra. entonces podria poner en cada
    proceso un array de semaforos o una lista de semaforos y todos los procesos bloqueados esperando al semaforo en esa lista ?
    
 */
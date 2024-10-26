#include <semaphores.h>

void sem_open(char * name, int value){
    sem_t * semaphore = (sem_t *) mm_malloc(sizeof(sem_t));
    semaphore->name = name;
    semaphore->value = value;
    semaphore->lock = 1;
    semaphore->wp_queue = initialize_children_queue();          // change maybe
}

void sem_close(char * name){
    // estructura?
}


void sem_post(char * name){}

void sem_wait(char * name){}
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
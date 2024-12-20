// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <phylos.h>
#include "random.h"

static int counter;

static char * fork[MAX];

static char * phylo_mutex = "phylo_mutex";
static char * printing = "printing";
static char * changing_mutex = "changing";

static uint64_t initial_pid;
static phylo_t phylos[MAX];

void add_philosopher(){
    usys_sem_wait(phylo_mutex);
    usys_sem_wait(changing_mutex);

    if (counter >= MAX) {
        usys_sem_post(changing_mutex);
        usys_sem_post(phylo_mutex);
        return;
    }

    char *num = usys_malloc(10);
    int_to_string(counter, num, 10);
    create_sem(num);

    phylos[counter].state = THINKING;

    create_philosopher(num);


    usys_sem_post(changing_mutex);
    usys_sem_post(phylo_mutex);

}

void remove_philosopher() {
    usys_sem_wait(phylo_mutex);
    usys_sem_wait(changing_mutex);

    if (counter <= INITIAL) {
        usys_sem_post(phylo_mutex);
        return;
    }

    int i = counter - 1; 

    // usys_sem_wait(fork[i]);
    usys_sem_close(fork[i]);
    usys_free(fork[i]);

    usys_kill(phylos[i].pid);

    counter--;

    usys_sem_post(changing_mutex);
    usys_sem_post(phylo_mutex);
}


void get_stdin(){
    while(1){
        char c = get_char();
        if(c == 'a'){
            add_philosopher();
        }
        else if(c == 'r'){
            remove_philosopher();
        }
        else if(c == 'q' || c == -1){
            exit_phylos();
        }
    }
}

void phylo_loop(){
    while(1){
        table();
        usys_sleep(2);
    }
}

void main_phylos(){
    print("Presionar tecla \"Q\" para abandonar\n");
    counter = 0;
    init_dinner();
    char * argv[] = {"phylo_loop", NULL};
    initial_pid = usys_create_process((uint64_t)phylo_loop, argv, 0, STDIN, STDOUT); 
    get_stdin();
}

void create_sem(char num[]){
    int i = satoi(num);    
    fork[i] = num;
    usys_sem_open(fork[i], 1);
}

void create_philosopher(char num[]){
    if(counter >= MAX) return;
    char ** argv;
    argv = usys_malloc(2 * sizeof(char*));
    argv[0] = "philosopher";
    argv[1] = num;
    argv[2] = NULL;

    int i = satoi(num); 
    phylos[i].pid = usys_create_process((uint64_t)philosopher_ps, argv, 0, STDIN, STDOUT);
    usys_free(argv);
    counter++;
}

void init_dinner(){
    usys_sem_open(phylo_mutex, 1);
    usys_sem_open(printing, 1);
    usys_sem_open(changing_mutex, 1);
    for(int i = 0; i < INITIAL; i++){
        phylos[i].state = THINKING;
    }
    char *num;
    for(int i = 0; i < INITIAL; i++){
        num = usys_malloc(10);
        int_to_string(i, num, 10);
        create_sem(num);
        create_philosopher(num);
    }
}

void table(){
    usys_sem_wait(phylo_mutex);
    usys_write(STDOUT,"* ", 2);
    for(int i = 0; i < counter; i++){
        if(phylos[i].state == EATING)
            usys_write(STDOUT," E ", 3);
        else
            usys_write(STDOUT," . ", 3);
    }
    usys_write(STDOUT," *\n", 3);
    usys_sem_post(phylo_mutex);
}


void exit_phylos(){
    usys_sem_wait(phylo_mutex);
    for(int i = 0; i < counter; i++){
        usys_sem_close(fork[i]);
        usys_free(fork[i]);
        usys_kill(phylos[i].pid);
        usys_wait_processes(phylos[i].pid);
    }
    usys_kill(initial_pid);
    usys_wait_processes(initial_pid);
    usys_sem_close(printing);
    usys_sem_close(phylo_mutex);
    usys_exit();
}

// Philosopher i -----------------------------------------------------------------------------------------------------------------

void think(int i){
    phylos[i].state = THINKING;
    usys_sleep(random_in_range(1, 3));

}

void eat(int i){
    phylos[i].state = EATING;
    usys_sleep(random_in_range(1, 3));
}


void philosopher(int i){
    while(1){
        think(i);
        take_forks(i);
        eat(i);
        put_forks(i);
        usys_sleep(1);
    }
}



void take_forks(int i){

    usys_sem_wait(phylo_mutex);
    phylos[i].state = HUNGRY;
    usys_sem_post(phylo_mutex);

    if((i % 2)){
        usys_sem_wait(fork[LEFT_FORK(i)]);
        usys_sem_wait(fork[RIGHT_FORK(i, counter)]);
    } else {
        usys_sem_wait(fork[RIGHT_FORK(i, counter)]);
        usys_sem_wait(fork[LEFT_FORK(i)]);
    }   
}

void put_forks(int i){
    usys_sem_wait(phylo_mutex);
    phylos[i].state = THINKING;
    usys_sem_post(fork[LEFT_FORK(i)]);
    usys_sem_post(fork[RIGHT_FORK(i, counter)]);

    usys_sem_post(phylo_mutex);
}

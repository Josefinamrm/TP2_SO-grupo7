#include <phylos.h>



void main_phylos(){
    init_dinner();
    while(1){
        table();
        usys_wait(1000);
    }
}

void create_sem(char num[]){
    int i = satoi(num);    
    fork[i] = num;
    usys_sem_open(fork[i], 1);
}

void create_philosopher(char num[]){
    char * argv[] = {"philosopher", num, NULL};        
    int pid = usys_create_process((uint64_t)philosopher_ps, argv, 0, STDIN, STDOUT);
}

void init_dinner(){
    usys_sem_open("mutex", 1);
    for(int i = 0; i < N; i++){
        state[i] = THINKING;
    }
    char num[10];
    for(int i = 0; i < N; i++){
        int_to_string(i, num, 10);
        create_sem(num);
        create_philosopher(num);
    }
}

void table(){
    usys_sem_wait("mutex");
    print("* ");
    for(int i = 0; i < N; i++){
        if(state[i] == EATING)
            print("E ");
        else
            print(".");
    }
    print(" *\n");
    usys_sem_post("mutex");
}


// Philosopher i ------------------------------------------------------------------

void phylosopher(int i){
    while(1){
        state[i] = THINKING;
        take_forks(i);
        state[i] = EATING;
        put_forks(i);
    }
}

void take_forks(int i){
    usys_sem_wait("mutex");
    state[i] = HUNGRY;
    test(i);
    usys_sem_post("mutex");
    usys_sem_wait(fork[i]);
}

void put_forks(int i){
    usys_sem_wait("mutex");
    state[i] = THINKING;
    test(LEFT);
    test(RIGHT);
    usys_sem_post("mutex");
}

void test(int i){
    if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
        state[i] = EATING;
        usys_sem_post(fork[i]);
    }
}
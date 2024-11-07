#include <phylos.h>

#define N 5
#define LEFT(x) (x + N - 1) % N
#define RIGHT(x) (x + 1) % N

typedef char * sem_name;
const enum {THINKING=0, HUNGRY, EATING} State;
int count = 0;


typedef struct{
    sem_name sem;
    int state
} phylo_t;

phylo_t phylos[N] = {0};

void table(){
    print("* ");
    for(int i = 0; i < count; i++){
        if(phylos[i].state == EATING)
            print(" E ");
        else
            print(" . ");
    }
    print(" *\n");
}


void init_dinner(){
    char * argv[] = {"phylosopher", NULL};
    for(int i = 0; i < N; i++){
        char name[2];
        int_to_string(i, name, 2);
        phylos[i].sem = name;
        usys_sem_open(name, 0);
        usys_create_process((uint64_t)phylosopher, i, 1, 0, 1);

    }
    usys_sem_open("mutex", 1);
    
}

void main_phylos(){
    init_dinner();
    while(1){
        for(int i = 0; i < N; i++){
            phylosopher(i);
        }
        table();
        usys_wait(2000);
    }

}



void phylosopher(int i){
    while(1){
        phylos[i].state = THINKING;
        take_forks(i);
        phylos[i].state = EATING;
        put_forks(i);
    }
}

void take_forks(int i){
    usys_sem_wait("mutex");
    phylos[i].state = HUNGRY;
    test(i);
    usys_sem_post(phylos[i].sem);
}

void put_forks(int i){
    usys_sem_wait("mutex");
    phylos[i].state = THINKING;
    test(LEFT(i));
    test(RIGHT(i));
    usys_sem_post("mutex");
}  

void test(int i){
    if(phylos[i].state == HUNGRY && phylos[LEFT(i)].state != EATING && phylos[RIGHT(i)].state != EATING){
        phylos[i].state = EATING;
        usys_sem_post(phylos[i].sem);
    }
}
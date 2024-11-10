// #include <phylos.h>


// void add_philosopher(int use){
//     int i = counter;

//     if(i && use){
//         usys_sem_wait(phylo_mutex);
//         adding = i-1;
//         usys_sem_wait(addex);
//     }
//     phylos[i].state = THINKING;

    
//     // usys_sem_wait(phylo_mutex);
    
//     // usys_sem_wait(fork[counter]);

//     char *num = usys_malloc(10);
//     int_to_string(counter, num, 10);
//     create_sem(num);
//     phylos[counter].state = THINKING;
    
//     create_philosopher(num);
    
//     if(i && use){
//         adding = -1;
//         usys_sem_post(phylo_mutex);
//         usys_sem_post(returnex);
//     }

//     // usys_sem_post(fork[(counter-1)]);
    
//     // usys_sem_post(phylo_mutex);

// }

// void remove_philosopher() {
//     int i = counter;
//     usys_sem_wait(phylo_mutex);
//     adding = i-2;

//     // if (counter <= INITIAL) {
//     //     usys_sem_post(phylo_mutex);
//     //     return;
//     // }
//     usys_sem_wait(addex);
//     usys_kill(phylos[i-1].pid);
//     usys_sem_close(fork[i-1]);
//     counter--;

//     adding = -1;

//     // int i = counter - 1; 
    
//     // usys_sem_wait(fork[i]);
//     // usys_sem_close(fork[i]);
//     // usys_free(fork[i]);

//     // usys_kill(phylos[i].pid);

//     // counter--;

//     usys_sem_post(phylo_mutex);
//     usys_sem_wait(returnex);
// }


// void get_stdin(){
//     while(1){
//         char c = get_char(STDIN);
//         if(c == 'a'){
//             add_philosopher(1);
//         }
//         else if(c == 'r'){
//             remove_philosopher();
//         }
//         else if(c == 'q'){
//             exit_phylos();
//         }
//     }
// }

// void phylo_loop(){
//     while(1){
//         usys_sem_wait(printex2);
//         table();
//         usys_wait(3000);
//         usys_sem_post(printex1);
//     }
// }

// void main_phylos(){
//     init_dinner();
//     char * argv[] = {"phylo_loop", NULL};
//     initial_pid = usys_create_process((uint64_t)phylo_loop, argv, 0, STDIN, STDOUT); 
//     get_stdin();
// }

// void create_sem(char num[]){
//     int i = satoi(num);    
//     fork[i] = num;
//     usys_sem_open(fork[i], 1);
// }

// void create_philosopher(char num[]){
//     if(counter >= MAX) return;
//     char ** argv;
//     argv = usys_malloc(3 * sizeof(char*));
//     argv[0] = "philosopher";
//     argv[1] = num;
//     argv[2] = NULL;

//     int i = satoi(num); 
//     phylos[i].pid = usys_create_process((uint64_t)philosopher_ps, argv, 0, STDIN, STDOUT);
//     usys_free(argv);
//     counter++;
// }

// void init_dinner(){
//     usys_sem_open(phylo_mutex, 1);
//     usys_sem_open(returnex, 0);
//     usys_sem_open(printex1, 1);
//     usys_sem_open(printex2, 0);
//     usys_sem_open(addex, 0);


//     for(int i = 0; i < INITIAL; i++){
//         phylos[i].state = THINKING;
//     }
//     char *num;
//     for(int i = 0; i < INITIAL; i++){
//         num = usys_malloc(10);
//         int_to_string(i, num, 10);
//         create_sem(num);
//         add_philosopher(0);
//     }
// }

// void table(){
//     usys_sem_wait(phylo_mutex);
//     print("* ");
//     for(int i = 0; i < counter; i++){
//         if(phylos[i].state == EATING)
//             print("E ");
//         else
//             print(". ");
//     }
//     print(" *\n");
//     usys_sem_post(phylo_mutex);
// }


// void exit_phylos(){
//     usys_sem_close(phylo_mutex);
//     for(int i = 0; i < counter; i++){
//         usys_sem_close(fork[i]);
//         usys_free(fork[i]);
//         usys_kill(phylos[i].pid);
//     }
//     usys_kill(initial_pid);
//     usys_exit();
// }

// // Philosopher i -----------------------------------------------------------------------------------------------------------------

// void think(int i){
//     usys_sem_wait(printex1);
//     phylos[i].state = THINKING;
//     usys_wait(random_in_range(1, 3) * 1000);
//     usys_sem_post(printex2);

// }

// void eat(int i){
//     phylos[i].state = EATING;
//     usys_wait(random_in_range(1, 3) * 1000);
// }

// void check_adding(int i){
//     if(adding == i){
//         usys_sem_post(addex);
//         usys_sem_wait(phylo_mutex);
//         usys_sem_post(returnex);    
//         usys_sem_post(returnex);
//     }
// }


// void philosopher(int i){
//     while(1){
//         think(i);
//         check_adding(i);
//         check_adding(i);
//         take_forks(i);
//         eat(i);
//         put_forks(i);
//         usys_wait(1000);
//     }
// }



// void take_forks(int i){
    
//     if((i % 2)){
//         usys_sem_wait(fork[LEFT(i)]);
//         usys_sem_wait(fork[RIGHT(i, counter)]);
//     } else {
//         usys_sem_wait(fork[RIGHT(i, counter)]);
//         usys_sem_wait(fork[LEFT(i)]);
//     }   
//     usys_sem_wait(phylo_mutex);
//     phylos[i].state = HUNGRY;
//     usys_sem_post(phylo_mutex);
// }

// void put_forks(int i){
//     usys_sem_wait(phylo_mutex);
//     phylos[i].state = THINKING;
//     usys_sem_post(fork[LEFT(i)]);
//     usys_sem_post(fork[RIGHT(i, counter)]);
   
//     usys_sem_post(phylo_mutex);
// }

#include <phylos.h>


void add_philosopher(){
    usys_sem_wait(phylo_mutex);
    
    usys_sem_wait(fork[counter]);

    char *num = usys_malloc(10);
    int_to_string(counter, num, 10);
    create_sem(num);
    
    phylos[counter].state = THINKING;
    
    create_philosopher(num);

    usys_sem_post(fork[(counter-1)]);
    
    usys_sem_post(phylo_mutex);

}

void remove_philosopher() {
    usys_sem_wait(phylo_mutex);

    if (counter <= INITIAL) {
        usys_sem_post(phylo_mutex);
        return;
    }

    int i = counter - 1; 
    
    usys_sem_wait(fork[i]);
    usys_sem_close(fork[i]);
    usys_free(fork[i]);

    usys_kill(phylos[i].pid);

    counter--;

    usys_sem_post(phylo_mutex);
}


void get_stdin(){
    while(1){
        char c = get_char(STDIN);
        if(c == 'a'){
            add_philosopher();
        }
        else if(c == 'r'){
            remove_philosopher();
        }
        else if(c == 'q'){
            exit_phylos();
        }
    }
}

void phylo_loop(){
    while(1){
        table();
        usys_wait(3000);
    }
}

void main_phylos(){
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
    argv = usys_malloc(3 * sizeof(char*));
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
    print("* ");
    for(int i = 0; i < counter; i++){
        if(phylos[i].state == EATING)
            print("E ");
        else
            print(". ");
    }
    print(" *\n");
    usys_sem_post(phylo_mutex);
}


void exit_phylos(){
    usys_sem_close(phylo_mutex);
    for(int i = 0; i < counter; i++){
        usys_sem_close(fork[i]);
        usys_free(fork[i]);
        usys_kill(phylos[i].pid);
    }
    usys_kill(initial_pid);
    usys_exit();
}

// Philosopher i -----------------------------------------------------------------------------------------------------------------

void think(int i){
    phylos[i].state = THINKING;
    usys_wait(random_in_range(1, 3) * 1000);

}

void eat(int i){
    phylos[i].state = EATING;
    usys_wait(random_in_range(1, 3) * 1000);
}


void philosopher(int i){
    while(1){
        think(i);
        take_forks(i);
        eat(i);
        put_forks(i);
        usys_wait(1000);
    }
}



void take_forks(int i){
    
    if((i % 2)){
        usys_sem_wait(fork[LEFT(i)]);
        usys_sem_wait(fork[RIGHT(i, counter)]);
    } else {
        usys_sem_wait(fork[RIGHT(i, counter)]);
        usys_sem_wait(fork[LEFT(i)]);
    }   
    usys_sem_wait(phylo_mutex);
    phylos[i].state = HUNGRY;
    usys_sem_post(phylo_mutex);
}

void put_forks(int i){
    usys_sem_wait(phylo_mutex);
    phylos[i].state = THINKING;
    usys_sem_post(fork[LEFT(i)]);
    usys_sem_post(fork[RIGHT(i, counter)]);
   
    usys_sem_post(phylo_mutex);
}

void test_philosopher(int i) {
    if (phylos[i].state == HUNGRY &&
        phylos[LEFT(i)].state != EATING &&
        phylos[RIGHT(i, counter)].state != EATING) {
        phylos[i].state = EATING;
        usys_sem_post(fork[i]); // Permitir que el filósofo proceda
    }
}
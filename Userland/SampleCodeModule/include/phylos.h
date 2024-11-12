#ifndef PHYLOS_H
#define PHYLOS_H

#include <user_syscalls.h>
#include <user_lib.h>
#include <processes.h>  



#define MAX 10
#define INITIAL 5

#define THINKING 0
#define HUNGRY 1
#define EATING 2

#define LEFT_FORK(x) x
#define RIGHT_FORK(x, y) (((x) + 1) % (y))

typedef struct {
    int state;
    uint64_t pid;
} phylo_t;


void table();
void init_dinner();
void main_phylos();
void philosopher(int i);
void create_sem(char num[]);
void create_philosopher(char num[]);
void take_forks(int i);
void put_forks(int i);
void exit_phylos();

#endif
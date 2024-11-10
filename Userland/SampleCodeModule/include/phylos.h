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

#define LEFT(x) x
#define RIGHT(x, y) (x + 1) % y

static char * fork[MAX];

static char * phylo_mutex = "phylo_mutex";
static char * printing = "printing";
static char * adding_mutex = "adding";

static uint64_t initial_pid;

typedef struct {
    int state;
    uint64_t pid;
} phylo_t;

static phylo_t phylos[MAX];

void table();
void init_dinner();
void main_phylos();
void philosopher(int i);
void test_philosopher(int i);
void take_forks(int i);
void put_forks(int i);
void exit_phylos();

#endif
#ifndef PHYLOS_H
#define PHYLOS_H

#include <user_syscalls.h>
#include <user_lib.h>
#include <processes.h>  

#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N

static int state[N];
static char fork[N];

void table();
void init_dinner();
void main_phylos();
void phylosopher(int i);
void take_forks(int i);
void put_forks(int i);
void test(int i);

#endif
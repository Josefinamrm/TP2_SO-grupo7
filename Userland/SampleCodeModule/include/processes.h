#ifndef PROCESSES_H
#define PROCESSES_H

#include <user_syscalls.h>
#include <test_processes.h>
#include <test_prio.h>

void ps_ps();
void testprocess_ps(int argc, char *argv[]);
void testprio_ps();


#endif
#ifndef PROCESSES_H
#define PROCESSES_H

#include <user_syscalls.h>
#include <test_processes.h>
#include <test_prio.h>
#include <test_sync.h>
#include <user_lib.h>

void ps_ps();
void testprocess_ps(int argc, char *argv[]);
void testprio_ps();
void memoryinfo_ps();
void testsynchro_ps(uint64_t argc, char *argv[]);
void my_process_inc_ps(uint64_t argc, char *argv[]);
void loop_ps();
void cat_ps();

#endif
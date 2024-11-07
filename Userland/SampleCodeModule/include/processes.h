#ifndef PROCESSES_H
#define PROCESSES_H

#include <user_syscalls.h>
#include <test_processes.h>
#include <test_prio.h>
#include <test_sync.h>
#include <user_lib.h>
#include <phylos.h>

void ps_ps();
void testprocess_ps(int argc, char *argv[]);
void testprio_ps();
void memoryinfo_ps();
void testsynchro_ps(int argc, char *argv[]);
void my_process_inc_ps(int argc, char *argv[]);
void loop_ps();
void cat_ps();
void wc_ps();
void filter_ps();
void phylos_ps();
void kill_ps(int argc, char *argv[]);
void nice_ps(int argc, char *argv[]);
void block_ps(int argc, char *argv[]);

#endif
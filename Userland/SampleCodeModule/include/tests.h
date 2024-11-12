#ifndef TESTS_H
#define TESTS_H

#include <stdint.h>
#include <user_syscalls.h>
#include <test_util.h>
#include <processes.h>
#include <user_lib.h>

uint64_t test_sync(int argc, char *argv[]);
uint64_t my_process_inc(int argc, char *argv[]);
uint64_t test_processes(int argc, char *argv[]);
uint64_t test_prio();
uint64_t test_mm(uint64_t argc, char *argv[]);

#endif
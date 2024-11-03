#ifndef TEST_SYNC_H
#define TEST_SYNC_H

#include <stdint.h>
#include <user_syscalls.h>
#include <test_util.h>
#include <user_lib.h>
#include <processes.h>

uint64_t test_sync(uint64_t argc, char *argv[]);
uint64_t my_process_inc(uint64_t argc, char *argv[]);

#endif
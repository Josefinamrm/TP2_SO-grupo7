// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <stdio.h>
#include "user_syscalls.h"
#include "test_util.h"
#include <processes.h>

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  int64_t aux = *p;
  usys_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(int argc,char *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc!= 4){
    return -1;
  }

  if ((n = satoi(argv[1])) <= 0){
    return -1;
  }
  if ((inc = satoi(argv[2])) == 0){
    return -1;
  }
  if ((use_sem = satoi(argv[3])) < 0){
    return -1;
  }


  if (use_sem){
    if (usys_sem_open(SEM_ID, 1) == -1) {
      print("test_sync: ERROR opening semaphore\n");
      return -1;
    }
  }


  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      usys_sem_wait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      usys_sem_post(SEM_ID);
  }

  if (use_sem)
    usys_sem_close(SEM_ID);

   return 0;
}

uint64_t test_sync(int argc, char *argv[]) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];
  if (argc != 3){
    return -1;
  }

  char *argvDec[] = {"process_inc1", argv[1], "-1", argv[2], NULL};
  char *argvInc[] = {"process_inc2", argv[1], "1", argv[2], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i + TOTAL_PAIR_PROCESSES] = usys_create_process((uint64_t)my_process_inc_ps, argvInc,0, STDIN, STDOUT);
    pids[i] = usys_create_process((uint64_t)my_process_inc_ps,argvDec, 0, STDIN, STDOUT);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    usys_wait_processes(pids[i]);
    usys_wait_processes(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  print("Final value:");
  print_dec(global);
  print("\n");

  return 0;
}
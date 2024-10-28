#include <stdint.h>
#include <stdio.h>
#include "test_util.h"
#include "processManager.h"
#include <semaphores.h>

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 1

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  my_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]) {
        printArray("dentro de process inc\n");
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 4)
    return -1;

  if ((n = satoi(argv[1])) <= 0)
    return -1;
  if ((inc = satoi(argv[2])) == 0)
    return -1;
  if ((use_sem = satoi(argv[3])) < 0)
    return -1;

  if (use_sem)
    if (my_sem_open(SEM_ID, 1) == -1) {
      printArray("test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      my_sem_wait(SEM_ID);
    my_yield();
    //slowInc(&global, inc);
    if (use_sem)
      my_sem_post(SEM_ID);
  }

  if (use_sem)
    my_sem_close(SEM_ID);

  my_exit();
  return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
    printArray("dentro de test_sync\n");
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 3)
    return -1;

  char *argvDec[] = {"my_process_inc_dec", argv[1], "-1", argv[2], NULL};
  char *argvInc[] = { "my_process_inc_inc",argv[1], "1", argv[2], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = my_create_process((uint64_t)my_process_inc, my_getpid(), 1, 4, argvDec);
    pids[i + TOTAL_PAIR_PROCESSES] = my_create_process((uint64_t)my_process_inc, my_getpid(), 1, 4, argvInc);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    my_wait(pids[i]);
    my_wait(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  printArray("Final value: ");
  printDec(global);
  putChar('\n');

  my_exit();
  return 0;
}
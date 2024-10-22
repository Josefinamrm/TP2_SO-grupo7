#include <user_syscalls.h>
#include <user_lib.h>
#include <test_util.h>
#include <test_processes.h>


enum State_t { RUNNING_t,
             BLOCKED_t,
             KILLED_t };

typedef struct P_rq {
  int32_t pid;
  enum State_t state;
} p_rq;


uint64_t test_processes(uint64_t argc, char *argv[]) {
print("Dentro de test processes\n");

  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  char *argvAux[] = {0};

  if (argc != 2)
    return -1;

  if ((max_processes = satoi(argv[1])) <= 0)
    return -1;

  p_rq p_rqs[max_processes];

  while (1) {
    print("antes de crear procesos\n");
    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      p_rqs[rq].pid = create_process((uint64_t)endless_loop, get_pid(), 1, 1, (uint8_t **)argvAux);

      if (p_rqs[rq].pid == -1) {
        print("test_processes: ERROR creating process\n");
        return -1;
      } else {
        p_rqs[rq].state = RUNNING_t;
        alive++;
      }
    }
        print("luego de crear procesos\n");


    // Randomly kills, blocks or unblocks processes until every one has been killed
     while (alive > 0) {
      print("Antes de segundo for los procesos\n");
      for (rq = 0; rq < max_processes; rq++) {
        action = GetUniform(100) % 2;

        switch (action) {
          case 0:
            if (p_rqs[rq].state == RUNNING_t || p_rqs[rq].state == BLOCKED_t) {
              if (kill(p_rqs[rq].pid) == -1) {
                print("test_processes: ERROR killing process\n");
                return -1;
              }
              p_rqs[rq].state = KILLED_t;
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING_t) {
              if (block(p_rqs[rq].pid) == -1) {
                print("test_processes: ERROR blocking process\n");
                return -1;
              }
              p_rqs[rq].state = BLOCKED_t;
            }
            break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED_t && GetUniform(100) % 2) {
          if (unblock(p_rqs[rq].pid) == -1) {
            print("test_processes: ERROR unblocking process\n");
            return -1;
          }
          p_rqs[rq].state = RUNNING_t;
        }
        print("despues del segundo for\n");
    } 
    usys_wait(2);
  }
}
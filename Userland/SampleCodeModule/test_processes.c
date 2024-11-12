// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <user_syscalls.h>
#include <user_lib.h>
#include <test_util.h>

enum State_t { RUNNING_t,
             BLOCKED_t,
             KILLED_t };

typedef struct P_rq {
  int32_t pid;
  enum State_t state;
} p_rq;

int64_t test_processes(int argc, char *argv[]) {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  char *argvAux[] = {"test process", NULL};

  if (argc != 2){
    return -1;
  }

  if ((max_processes = satoi(argv[1])) <= 0)
    return -1;

  p_rq p_rqs[max_processes];

  while (1) {
    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      p_rqs[rq].pid = usys_create_process((uint64_t)endless_loop, argvAux, 0, 0, 1);

      if (p_rqs[rq].pid == -1) {
        print("test_processes: ERROR creating process\n");
        return -1;
      } else {
        p_rqs[rq].state = RUNNING_t;
        alive++;
      }
    }


    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0) {
      for (rq = 0; rq < max_processes; rq++) {
        action = GetUniform(100) % 2;

        switch (action) {
          case 0:
            if (p_rqs[rq].state == RUNNING_t || p_rqs[rq].state == BLOCKED_t) {
              if (usys_kill(p_rqs[rq].pid) == -1) {
                print("test_processes: ERROR killing process\n");
                return -1;
              }
              p_rqs[rq].state = KILLED_t;
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING_t) {
              if (usys_block(p_rqs[rq].pid) == -1) {
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
          if (usys_unblock(p_rqs[rq].pid) == -1) {
            print("test_processes: ERROR unblocking process\n");
            return -1;
          }
          p_rqs[rq].state = RUNNING_t;
        }
    }
  }

}
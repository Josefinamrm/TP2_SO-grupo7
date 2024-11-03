#ifndef _SHELL_H_
#define _SHELL_H_ 

#include <user_lib.h>
#include <ucolors.h>
#include <eliminator.h>
#include <user_syscalls.h>
#include "../include/test_prio.h"
#include <processes.h>

void init_shell();

typedef struct Command {
    char* name_id;
    void (*func) (char * param);
    char* desc;
    char* usage;
} Command;

#endif
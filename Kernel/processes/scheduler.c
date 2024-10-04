#include "processManager.h"

uint64_t * scheduler(uint64_t * rsp){
    current_process->stack_pointer = rsp;
    my_unblock(current_process->pid);
    current_process = ready_queue->next->p;
    current_process->state = RUNNING;
    remove_from_ready_queue(current_process->pid);
    return current_process->stack_pointer;
}
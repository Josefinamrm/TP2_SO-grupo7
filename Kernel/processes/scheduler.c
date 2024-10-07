#include "processManager.h"

uint64_t * scheduler(uint64_t * rsp){

    // Stores running process information
    current_process->stack_pointer = rsp;
    current_process->state = READY;

    // Chooses next running process from the circular list
    ready_queue = ready_queue->next;
    current_process = ready_queue->p;
    current_process->state = RUNNING;

    return current_process->stack_pointer;
}
#include "processManager.h"


uint64_t scheduler(uint64_t rsp){
    if(is_ready_queue_empty()){
        return idle_process_rsp();
    }
    return next_running_process(rsp);
}
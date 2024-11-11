// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "processManager.h"


uint64_t scheduler(uint64_t rsp){
    if(is_ready_queue_empty()){
        return idle_process_rsp();
    }
    return next_running_process(rsp);
}
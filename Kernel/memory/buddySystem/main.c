// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include "buddy.h"

int main(){
    printf("Hello, World!\n");
    mm_init(0, 0);
    print_mem_states();

    int * aux = mm_malloc(20);
    
    return 0;
}
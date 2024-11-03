// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <processes.h>
#include <test_sync.h>

void ps_ps(){
    usys_ps();
    usys_exit();
}

void testprocess_ps(int argc, char *argv[]){
    test_processes(argc, argv);
    usys_exit();
}

void testprio_ps(){
    test_prio();
    usys_exit();
}

void memoryinfo_ps() {

    print("-------------------------------------------------------\n");
    print("Total space: ");
    print_dec(usys_total_space());
    print("\n");

    print("Occupied space: ");
    print_dec(usys_occupied_space());
    print("\n");

    print("Unused space: ");
    print_dec(usys_unused_space());
    print("\n");
    print("-------------------------------------------------------\n");
    usys_exit();
}

void testsynchro_ps(uint64_t argc, char *argv[]){
    test_sync(argc,argv);
    usys_exit();
}

void my_process_inc_ps(uint64_t argc, char *argv[]){
    my_process_inc(argc,argv);
    usys_exit();
}
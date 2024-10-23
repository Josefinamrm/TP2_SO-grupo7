#include <processes.h>

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
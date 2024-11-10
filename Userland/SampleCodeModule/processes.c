// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <processes.h>
#include <test_sync.h>
#include <shell.h>

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

    usys_write(STDOUT, "-------------------------------------------------------\n", 56);
    usys_write(STDOUT, "Total space: ", 13);
    char total_space[100];
    int_to_string(usys_total_space(), total_space, 100);
    usys_write(STDOUT, total_space, stringlen(total_space));
    usys_write(STDOUT, "\n", 1);

    usys_write(STDOUT, "Occupied space: ", 16);
    char occupied_space[100];
    int_to_string(usys_occupied_space(), occupied_space, 100);
    usys_write(STDOUT, occupied_space, stringlen(occupied_space));
    usys_write(STDOUT, "\n", 1);

    usys_write(STDOUT, "Unused space: ", 14);
    char unused_space[100];
    int_to_string(usys_unused_space(), unused_space, 100);
    usys_write(STDOUT, unused_space, stringlen(unused_space));
    usys_write(STDOUT, "\n", 1);
    usys_write(STDOUT, "-------------------------------------------------------\n", 56);
     
    usys_exit();
}

void testsynchro_ps(int argc, char *argv[]){
    test_sync(argc, argv);
    usys_exit();
}

void my_process_inc_ps(int argc, char *argv[]){
    my_process_inc(argc, argv);
    usys_exit();
}

void loop_ps(){

    while(1){
        usys_write(STDOUT, "Hola! Soy el proceso: ", 22);
        char pid_str[10];
        int_to_string(usys_get_pid(), pid_str, 10);
        usys_write(STDOUT, pid_str, stringlen(pid_str));
        usys_write(STDOUT, "a\n", 2);
        usys_exit();
    }
}

void cat_ps(){
    char buffer[1024];
    int bufferIndex = 0;
    char c;
    while((c = usys_read(STDIN, buffer, 1024)) > 0){ // print the stdin?
        usys_write(STDOUT, buffer, c);
    }

    usys_write(STDOUT, buffer, stringlen(buffer));
    print("\n");
    usys_exit();
}


void wc_ps(){
   char buffer[1024];
    int bufferIndex = 0;
    char c;
    while((c = usys_read(STDIN, buffer, 1024)) > 0){ // print the stdin?
        usys_write(STDOUT, buffer, c);
    }

    int count = 1;
    int j=0;
    for(int i = 0; i < stringlen(buffer); i++){
        if(buffer[i] == '\n' && buffer[i+1] != 0){
            count++;
        }
    }
    char count_str[10];
    int_to_string(count, count_str, 10);
    usys_write(STDOUT, count_str, 10);
    usys_write(STDOUT, "\n", 1);
    usys_exit(); 
}

void filter_ps(){
    char buffer[1024];
    int bufferIndex = 0;
    char c;
    while((c = usys_read(STDIN, buffer, 1024)) > 0){ // print the stdin?
        usys_write(STDOUT, buffer, c);
    }

    char to_print[stringlen(buffer)];
    int j=0;
    for(int i = 0; i < stringlen(buffer); i++){
        if(buffer[i] != 'a' && buffer[i] != 'e' && buffer[i] != 'i' && buffer[i] != 'o' && buffer[i] != 'u'){
            to_print[j++] = buffer[i];
        }
    }
    j--;
    usys_write(STDOUT, to_print, j);
    usys_write(STDOUT, "\n", 1);
    usys_exit(); 
}

void phylos_ps(){
    main_phylos();
    usys_wait_processes(-1);
    usys_exit();
}

void philosopher_ps(int argc, char *argv[]){
    int i = satoi(argv[1]);
    philosopher(i);
    usys_exit();
}

void kill_ps(int argc, char *argv[]){
    int pid = satoi(argv[1]);
    if(usys_kill(pid) == 0){
        print("Process ");
        print_dec(pid);
        print(" killed\n");
    } else {
        print_error("Error killing process ");
        print_dec(pid);
        print("\n");
    }
    usys_exit();
}

void nice_ps(int argc, char *argv[]){
    int pid = satoi(argv[1]);
    int prio = satoi(argv[2]);
    usys_nice(pid, prio);
    print("Process ");
    print_dec(pid);
    print(" priority changed to ");
    print_dec(prio);
    print("\n");
    usys_exit();
}

void block_ps(int argc, char *argv[]){
    int pid = satoi(argv[1]);
    if(usys_block(pid) == 0){
        print("Process ");
        print_dec(pid);
        print(" blocked\n");
    } else {
        print_error("Error blocking process ");
        print_dec(pid);
        print("\n");
    }
    usys_exit();
}
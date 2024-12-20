// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <processes.h>
#include <test_sync.h>
#include <shell.h>

void ps_ps(){
    process_view processes[MAX_PROCESSES];
    int dim = usys_get_process_info(processes);
    char * spacing = "        ";
    print("Name");
    print(spacing);
    print("Pid");
    print(spacing);
    print("Priority");
    print(spacing);
    print("State");
    print(spacing);
    print("RSP");
    print(spacing);
    print("fg");
    put_char('\n');
    for(int i = 0; i < dim; i++){
        print(processes[i].name);
        print(spacing);
        print_dec(processes[i].pid);
        print(spacing);
        print_dec(processes[i].priority);
        print(spacing);
        print(processes[i].state);
        print(spacing);
        print_dec(processes[i].stack_pointer);
        print(spacing);
        print(processes[i].foreground);
        put_char('\n');
    }
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
    put_char('\n');

    print("Occupied space: ");
    print_dec(usys_occupied_space());
    put_char('\n');

    print("Unused space: ");
    print_dec(usys_unused_space());
    put_char('\n');
    print("-------------------------------------------------------\n");
     
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
    int pid = usys_get_pid();

    while(1){
        print("Hola! Soy el proceso: ");
        print_dec(pid);
        put_char('\n');
        usys_sleep(1);
    }
        usys_exit();
}

void cat_ps(){
    char buffer[1024];
    int bufferIndex = 0;
    char c;
    while(usys_read(STDIN, &c, 1) > 0){ // print the stdin?
        put_char(c);
        buffer[bufferIndex++] = c;
        if(c == '\n'){
            buffer[bufferIndex] = '\0';
            print(buffer);
            bufferIndex = 0;
        }
    }
    usys_exit();
}


void wc_ps(){
    int line_count = 0;
    char c;
    while(usys_read(STDIN, &c, 1) > 0){ // print the stdin?
        put_char(c);
        if(c == '\n'){
            line_count++;
        }
    }
    print_dec(line_count);
    put_char('\n');
    usys_exit(); 
}

void filter_ps(){
    char buffer[1024];
    int bufferIndex = 0;
    char c;
    while(usys_read(STDIN, &c, 1) > 0){ // print the stdin?
        put_char(c);
        buffer[bufferIndex++] = c;
    }
    buffer[bufferIndex] = '\0';

    char to_print[bufferIndex];
    int j=0;
    put_char('\n');
    for(int i = 0; i < bufferIndex; i++){
        if(!is_vowel(buffer[i])){
            to_print[j++] = buffer[i];
        }
    }
    to_print[j] = '\0';
    print(to_print);
    put_char('\n');
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

void testmemory_ps(int argc, char *argv[]){
    print_dec(test_mm(argc, argv));  
    usys_exit();
}
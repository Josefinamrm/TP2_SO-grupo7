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

    print("-------------------------------------------------------\n");
    print("Total space: ");
    usys_wait(5);
    print_dec(usys_total_space());
    usys_wait(5);
    print("\n");

    print("Occupied space: ");
    usys_wait(5);
    print_dec(usys_occupied_space());
    usys_wait(5);
    print("\n");

    print("Unused space: ");
    print_dec(usys_unused_space());
    print("\n");
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
    while(1){
        print("Hola! Soy el proceso nro ");
        print_dec(usys_get_pid());
        print("\n");
        usys_wait(4000);
    }
    usys_exit();
}

void cat_ps(){
    char buffer[1];
    int bufferIndex = 0;
    char c;
    char reading = 1;
    while((c = usys_read(STDIN, buffer, 1)) > 0){ // print the stdin?
        usys_write(STDOUT, buffer, c);
    }
    // buffer[bufferIndex] = 0;
    usys_write(STDOUT, buffer, bufferIndex-1);
    print("\n");
    usys_exit();
}


void wc_ps(){
    char buffer[1024];
    int bufferIndex = 0;
    int count = 0;
    char c;
    char reading = 1;
    while(reading){
        while((c = usys_read(STDIN, buffer, 1024)) > 0){ // print the stdin?
            bufferIndex++;
        } 
        reading = 0;
    }
    // buffer[bufferIndex] = 0;
    
    for(int i=0; i<3; i++){
        put_char(buffer[i]);
    }

    for(int i = 0; i < bufferIndex; i++){
        usys_write(STDOUT, (buffer[i]), 1); 
        put_char(buffer[i]); 
    }

    char count_str[11];
    int_to_string(count, count_str, 11);
    usys_write(STDOUT, count_str, 11);
    print("\n");
    usys_exit();   
}

void filter_ps(){
    char buffer[100];
    int bufferIndex = 0;
    char c;
    char reading = 1;
    while (reading) {  // if ESC 
        c = get_char();  // non-blocking read
        if (c != 0) {
            if (c == '\b' && bufferIndex > 0) {
                bufferIndex--;
                put_char(c);
            } else if (c == '\n') {
                print("\n");
                buffer[bufferIndex] = '\0';  // Null-terminate the command
                reading = 0;
            } 
            else if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u'){
                put_char(c);
            }
            else if (c != '\b') {
                put_char(c);
                buffer[bufferIndex++] = c;
           }
        }
    } 
    usys_write(STDOUT, buffer, bufferIndex);
    print("\n");
    usys_exit();
}

void phylos_ps(){
    main_phylos();
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
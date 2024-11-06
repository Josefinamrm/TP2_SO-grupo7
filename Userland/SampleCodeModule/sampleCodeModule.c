// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */

#include <shell.h>

#define INPUT_SIZE 100 
#define COMMAND_COUNT 19
#define CANT_REGS 18
#define TRUE 1
#define FALSE 0
#define MAX_ARGS 10

void help();
void divzero();
void invopcode();
void time();
void play_eliminator();
void zoomin();
void zoomout();
void inforeg();
void clear_shell();
void beep();
void testprocess();
void testprio();
void ps();
void mem();
void testsynchro(); 
void loop();
void cat();
void wc();
void filter();

static char buffer[INPUT_SIZE] = {0};
static int bufferIndex = 0;
static int currentFontSize;
static int gameActive = 0;
static char foreground = TRUE;
static int argC=0;
static char *arguments[MAX_ARGS] = {0};

static Command commands[] = {
    {"help", help, "Muestra la lista de comandos.","No recibe argumentos."},
    {"divzero", divzero, "Simula la excepcion de division por 0.","No recibe argumentos."},
    {"invopcode", invopcode, "Simula la excepcion de opcode invalida.","No recibe argumentos."},
    {"time", time, "Muestra la hora actual.","No recibe argumentos."},
    {"inforeg", inforeg, "Imprime los registros capturados por CTRL.","No recibe argumentos."},
    {"eliminator", play_eliminator, "Inicia el juego de eliminator.","No recibe argumentos."},
    {"zoomin", zoomin, "Aumenta el tamanio de la letra.", "No recibe argumentos."},
    {"zoomout", zoomout, "Disminuye el tamanio de la letra.", "No recibe argumentos."},
    {"clear", clear_shell, "Limpia la shell.", "No recibe argumentos."},
    {"beep", beep, "Emite un beep.","No recibe argumentos."},
    {"testprocess", testprocess, "Crea el proceso test_process.","Recibe 1 argumento: Cantidad de procesos a crear."},
    {"testprio", testprio, "Crea procesos con distintas prioridades.","No recibe argumentos."},
    {"ps", ps, "Muestra los procesos y sus estados.","No recibe argumentos."},
    {"mem", mem, "Muestra informacion de la memoria.","No recibe argumentos."},
    {"testsynchro", testsynchro, "Crea el proceso para testear sincronizacion CON semaforos.","Recibe dos argumentos: cantidad de incrementos o decrementos y 1 o 0 (CON o SIN semáforos)."},
    {"loop", loop, "Imprime saludo y ID cada 2 segundos.","No recibe argumentos."},
    {"cat", cat, "Imprime el stdin tal como lo recibe.","No recibe argumentos."},
    {"wc", wc, "Cuenta la cantidad de palabras en el stdin.","No recibe argumentos."},
    {"filter", filter, "Filtra el stdin y muestra solo las letras.","No recibe argumentos."}
};

void parse_command(char *str) {

    if (strcmp(str, "")==0) {
        return;
    }

    argC = parse_command_arg(str, arguments);

    char * cmd = arguments[0];


    if (argC > 1) {
        if(strcmp(arguments[1], "b") == 0){
            foreground = FALSE;
            str = cmd;
        }
    }

    for (int i = 0; i < COMMAND_COUNT; i++) {
        if (strcmp(cmd, commands[i].name_id) == 0) {
            (*commands[i].func)(cmd);
            return;
        }
    } 
        
    print_error("Error: comando no diponible. Ingrese \"help\" para ver los comandos disponibles.\n");
}

void print_prompt_icon() {
    print_color(GREEN, "$>");
}


int main()
{
    print("Bienvenido a ");
    print_color(BLUE,"DORY_OS\n");
    print_color(GRAY,"Ingrese ");
    print_color(WHITE,"\"help\" ");
    print_color(GRAY,"para ver los comandos disponibles.\n");

    print_color(GRAY, "Presione ");
    print_color(WHITE, "\"b\" ");
    print_color(GRAY, "luego del comando para ejecutar en background.\n");

    char c;
    int running = 1; 
    currentFontSize = usys_get_font_size();
    print_prompt_icon();
    while (running){
        c = get_char();
        put_char(c);
    }
    // print("Ingrese uno de los siguientes comandos:\n");
    // for(int i = 1; i < COMMAND_COUNT-1 ; i++){
    //         print_color(LIGHT_BLUE, commands[i].name_id);
    //         print(" | ");
    // } 
    // print_color(LIGHT_BLUE, commands[COMMAND_COUNT-1].name_id);


    /* print_color(GRAY,"Ingrese ");
    print_color(WHITE,"\"help\" ");
    print_color(GRAY,"para ver los comandos disponibles.\n");

    print_color(GRAY, "Presione ");
    print_color(WHITE, "\"b\" ");
    print_color(GRAY, "luego del comando para ejecutar en background.\n");

    char c;
    int running = 1; 
    currentFontSize = usys_get_font_size();
    print_prompt_icon();
    while (running) {  // if ESC 
        c = get_char();  // non-blocking read
        if (c != 0) {
            if (c == '\b' && bufferIndex > 0) {
                bufferIndex--;
                put_char(c);
            } else if (c == '\n') {
                put_char(c);
                // printPromptIcon();
                buffer[bufferIndex] = '\0';  // Null-terminate the command
                parse_command(buffer);  // Process the command
                bufferIndex = 0;  // Reset the buffer for the next command
                print_prompt_icon();
            } else if (c != '\b') {
                put_char(c);
                buffer[bufferIndex++] = c;
            }
            foreground = TRUE;
            argC = 0;
        }
    } 
    usys_exit();
    return 0; */
}


/* --------------------------- Common functions ------------------------------------------------------------------------*/

static int no_arguments_func(char * func){
    if((foreground && argC > 1) || (!foreground && argC > 2)){
        print_error("Error: el comando ");
        print(func);
        print_error(" no recibe argumentos.\n");
        return -1;
    }
    return 0;
}

static void missing_arguments(char * func){
    print_error("Error: faltan argumentos para el comando ");
    print(func);
    print_error(".\n");
}

static void too_many_arguments(char * func){
    print_error("Error: demasiados argumentos para el comando ");
    print(func);
    print_error(".\n");
}

static int cant_arguments_func(char * func, int cant, int needed){
    if(!foreground) needed++;
    if (cant > needed){
        too_many_arguments(func);
        return -1;
    } else if (cant < needed){
        missing_arguments(func);
        return -1;
    }
    return 0;
}

/* --------------------------- Commands functions ------------------------------------------------------------------------*/

void help() {
    print_color(YELLOW, "Hacemos zoom-out para ver los comandos disponibles.\n");
    zoomout();
    print("Comandos disponibles:\n");
    for(int i = 1; i < COMMAND_COUNT ; i++){
            print("   ");
            print_color(LIGHT_BLUE, commands[i].name_id);
            print(": ");
            print(commands[i].desc);
            print(" - ");
            print_color(GRAY, commands[i].usage);
            put_char('\n');
    } 
    print_color(YELLOW,"Restauramos el tamanio de la letra.\n");
    zoomin();
}

void divzero() { 
    if(no_arguments_func("divzero") ==-1) return;
    int a = 1; //rax??
    int b = 0; 
    if ((a/b) == 1) {
        print_error("This is wrong...");
    }
}
void invopcode() {
    if(no_arguments_func("invopcode") ==-1) return;
    _invalid_opcode_exception();
}

void time() {
    if(no_arguments_func("time") ==-1) return;
    print_color(GREEN, "ART (Argentine Time): UTC/GMT -3 horas\n"); 
    _get_time(); 
}

void zoomin() {
    if(no_arguments_func("zoomin") ==-1) return;
    print_color(YELLOW, "Esto va a borrar toda la pantalla. Esta seguro de que quiere proceder?\n");
    print_color(ORANGE, "Indicar Si (Si) o N (No)\n"); 
    char c;
    while (1) {
        while ((c = get_char()) == 0); 
        if (c == 'N' || c == 'n') {
            return;
        } else if (c == 'S' || c == 's') {
            if (currentFontSize >= 3) {
                print_color(RED, "No se puede agrandar mas.\n");
                return;
            }
            usys_change_font_size(++currentFontSize);
            clear_shell();
            return;
        } else {
            print_color(RED, "Indique S o N\n");
        }
    }
}

void zoomout() {
    if(no_arguments_func("zoomout") ==-1) return;
    print_color(YELLOW, "Esto va a borrar toda la pantalla. Esta seguro de que quiere proceder?\n");
    print_color(ORANGE, "Indicar S (Si) o N (No)\n"); 
    char c;
    while (1) {
        while ((c = get_char()) == 0); 
        if (c == 'N' || c == 'n') {
            return;
        } else if (c == 'S' || c == 's') {
            if (currentFontSize <= 1) {
                print_color(RED, "No se puede achicar mas.\n");
                return;
            }
            usys_change_font_size(--currentFontSize);
            clear_shell();
            return;
        } else {
            print_color(RED, "Indique S o N\n");
        }
    }
}

void inforeg() {
    if(no_arguments_func("inforeg") ==-1) return;
    uint64_t regs[CANT_REGS];
    char *regsNames[CANT_REGS] = {"rax:", "rbx:", "rcx:", "rdx:", "rsi:", "rdi:", "rbp:", "rsp:", "r8:", "r9:",
					   "r10:", "r11:", "r12:", "r13:", "r14:", "r15:", "rip:", "rflags:"};
    char aux[128];
    int flag = usys_get_registers(regs);
    if (flag) {
        print_color(GREEN, "Informacion de los registros: \n");
        for(int i = 0; i<CANT_REGS; i++){
            print("    ");
			print_color(GREEN, regsNames[i]);
			uint_to_base(regs[i], aux, 16);
			print(aux);
			print("\n");
         }
    } else {
        print_color(RED, "No se pudo encontrar ningun momento de captura de registro. Presione CTRL para capturar los registros.\n");
    }
}
void clear_shell() {
    if(no_arguments_func("clear") ==-1) return;
    usys_clear_screen();
}

void beep() {
    if(no_arguments_func("beep") ==-1) return;
    print_color(GREEN, "BEEP!!\n");
    usys_beep(1000, 10);
}

void play_eliminator() {
    if(no_arguments_func("eliminator") ==-1) return;
    gameActive = 1;
    clear_shell();
    eliminator();
    clear_shell(); 
    gameActive = 0;
}

void testprocess() {
    if(cant_arguments_func("testprocess", argC, 2) == -1) return;
    char * arg_command = (foreground)? arguments[1] : arguments[2];
    char * argv[] = {"test processes", foreground, arg_command ,NULL};
    int pid = usys_create_process((uint64_t)testprocess_ps,  usys_get_pid() , 1 , 3, argv);
    if(foreground) 
        usys_wait_processes(pid);
}

void testprio() {
    if(no_arguments_func("testprio") ==-1) return;
    char * argv[] = {"test prio", foreground, NULL};
    int pid = usys_create_process((uint64_t)testprio_ps, usys_get_pid(), 1, 2, argv);
    if(foreground) 
        usys_wait_processes(pid);
}

void ps() {
    if(no_arguments_func("ps") ==-1) return;
    char * argv[] = {"ps", foreground, NULL};
    int pid = usys_create_process((uint64_t)ps_ps, usys_get_pid(), 1, 2, argv); 
    if(foreground) 
        usys_wait_processes(pid);
}

void mem() {
    if(no_arguments_func("mem") ==-1) return;
    char * argv[] = {"memoryinfo", foreground, NULL};
    int pid = usys_create_process((uint64_t)memoryinfo_ps, usys_get_pid(), 1, 2, argv); 
    if(foreground) 
        usys_wait_processes(pid);
}

void testsynchro() {
    if(cant_arguments_func("testsynchro", argC, 3) == -1) return;
    char * arg_1 = foreground? arguments[1] : arguments[2];
    char * arg_2 = foreground? arguments[2] : arguments[3];
    //char * argv[] = {"test synchro", foreground, "5", "1", NULL};
    char * argv[] = {"test synchro", foreground, arg_1, arg_2, NULL};
    int pid = usys_create_process((uint64_t)testsynchro_ps, usys_get_pid(), 1, 4, argv); 
    if(foreground) 
        usys_wait_processes(pid);
}

// void testnosynchro() {
//     if(cant_arguments_func("testnosynchro", argC, 2) == -1) return;
//     char * arg_1 = foreground? arguments[1] : arguments[2];
//     char * arg_2 = foreground? arguments[2] : arguments[3];
//     char * argv[] = {"test synchro", foreground, arg_1, arg_2, NULL};
//     int pid = usys_create_process((uint64_t)testsynchro_ps, usys_get_pid(), 1, 4, argv); 
//     if(foreground) 
//         usys_wait_processes(pid);
// }

void loop(){
    no_arguments_func("loop");
    char * argv[] = {"loop", foreground, NULL};
    int pid = usys_create_process((uint64_t) loop_ps, usys_get_pid(), 1, 2, argv);
    if(foreground)
        usys_wait_processes(pid);
}

void cat(){
    char * argv[] = {"cat", foreground, NULL};
    int pid = usys_create_process((uint64_t) cat_ps, usys_get_pid(), 1, 2, argv);
    if(foreground)
        usys_wait_processes(pid);
}

void wc(){
    char * argv[] = {"wc", foreground, NULL};
    int pid = usys_create_process((uint64_t) wc_ps, usys_get_pid(), 1, 2, argv);
    if(foreground)
        usys_wait_processes(pid);
}

void filter(){
    char * argv[] = {"filter", foreground, NULL};
    int pid = usys_create_process((uint64_t) filter_ps, usys_get_pid(), 1, 2, argv);
    if(foreground)
        usys_wait_processes(pid);
}
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */

#include <shell.h>

#define INPUT_SIZE 100 
#define COMMAND_COUNT 21
#define CANT_REGS 18
#define TRUE 1
#define FALSE 0
#define MAX_ARGS 10

void help();
void time();
void zoomin();
void zoomout();
void inforeg();
void clear_shell();
void beep();
void testprocess();
void testprio();
void ps(int fd[]);
void mem(int fd[]);
void testsynchro(int fd[]); 
void loop(int fd[]);
void cat(int fd[]);
void wc(int fd[]);
void filter(int fd[]);
void phylo(int fd[]);
void kill();
void nice();
void block();
void testmemory();

static char buffer[INPUT_SIZE] = {0};
static int bufferIndex = 0;
static int currentFontSize;
static char foreground = TRUE;
static char pipe = FALSE;
static int argC=0;
static char *arguments[MAX_ARGS] = {0};

static Command commands[] = {
    {"help", help, "Muestra la lista de comandos.","No recibe argumentos."},
    {"time", time, "Muestra la hora actual"},
    {"inforeg", inforeg, "Imprime los registros capturados por CTRL.","No recibe argumentos."},
    {"zoomin", zoomin, "Aumenta el tamanio de la letra.", "No recibe argumentos."},
    {"zoomout", zoomout, "Disminuye el tamanio de la letra.", "No recibe argumentos."},
    {"clear", clear_shell, "Limpia la shell.", "No recibe argumentos."},
    {"testprocess", testprocess, "Crea el proceso para testear la creacion de procesos.","Recibe 1 argumento: Cantidad de procesos a crear."},
    {"testprio", testprio, "Crea el proceso para testear la prioridad de los procesos.","No recibe argumentos."},
    {"beep", beep, "Emite un beep"},
    {"ps", (void (*)(char *))ps, "Muestra los procesos y sus estados.","No recibe argumentos."},
    {"mem", (void (*)(char *))mem, "Muestra informacion de la memoria.","No recibe argumentos."},
    {"testsynchro", (void (*)(char *))testsynchro, "Crea el proceso para testear sincronizacion de procesos.","Recibe dos argumentos: cantidad de incrementos o decrementos y 1 o 0 (CON o SIN semaforos)."},
    {"loop", (void (*)(char *))loop, "Imprime saludo y ID cada 2 segundos.","No recibe argumentos."},
    {"cat", (void (*)(char *))cat, "Imprime el stdin tal como lo recibe.","No recibe argumentos."},
    {"wc", (void (*)(char *))wc, "Cuenta la cantidad de palabras en el stdin.","No recibe argumentos."},
    {"filter", (void (*)(char *))filter, "Filtra el stdin y muestra solo las letras.","No recibe argumentos."},
    {"phylo", (void (*)(char *))phylo, "Muestra el problema de los filosofos.","No recibe argumentos."},
    {"kill", (void (*)(char *))kill, "Mata un proceso.","Recibe 1 argumento: PID del proceso a matar."},
    {"nice", (void (*)(char *))nice, "Cambia la prioridad de un proceso.","Recibe 2 argumentos: PID del proceso y nueva prioridad."},
    {"block", (void (*)(char *))block, "Bloquea un proceso.","Recibe 1 argumento: PID del proceso a bloquear."}, 
    {"testmemory", (void (*)(char *))testmemory, "Crea el proceso para testear el manejo de memoria.", "Recibe 1 argumento: memoria máxima."}
};


void execute_command(char * cmd, int fd[2]) {

    for (int i = 0; i < COMMAND_COUNT; i++) {
        if (strcmp(cmd, commands[i].name_id) == 0) {
            (commands[i].func)((void *)fd);
            return;
        }
    }

    print_error("Error: comando no diponible. Ingrese \"help\" para ver los comandos disponibles.\n");
}


void pipe_impl(char * args[], int argC) {

    int fds[2];
    if(usys_open_pipe(fds) == -1){
        print_error("Error al crear el pipe.\n");
        return;
    }
    int fd_left[2];
    fd_left[0] = STDIN;
    fd_left[1] = fds[1];

    int fd_right[2];
    fd_right[0] = fds[0];
    fd_right[1] = STDOUT;

    execute_command(args[0], fd_left);
    execute_command(args[2], fd_right);

}


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
        if (strcmp(arguments[1], "|") == 0){
            pipe = TRUE;
            pipe_impl(arguments, argC);
            return;
        }
    }
 
    if(pipe == FALSE){
        int fd[2] = {STDIN, STDOUT};
        execute_command(cmd, fd);
    }
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
    currentFontSize = usys_get_font_size();
    print_prompt_icon();
    while (1) {  // if ESC 
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
            pipe = FALSE;
            argC = 0;
        }
    } 
    usys_exit();
    return 0;
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
    print("Comandos disponibles:\n");
    for(int i = 0; i < COMMAND_COUNT ; i++){
            print("   ");
            print_color(LIGHT_BLUE, commands[i].name_id);
            print(": ");
            print(commands[i].desc);
            print(" - ");
            print_color(GRAY, commands[i].usage);
            put_char('\n');
    }
}

void time() {
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
    print_color(GREEN, "BEEP!!\n");
    usys_beep(1000, 1);
}

void testprocess() {
    if(cant_arguments_func("testprocess", argC, 2) == -1) return;
    char * arg_command = (foreground)? arguments[1] : arguments[2];
    char * argv[] = {"test processes", arg_command ,NULL};
    int pid = usys_create_process((uint64_t)testprocess_ps, argv, foreground, STDIN, STDOUT);
    if(foreground) 
        usys_wait_processes(pid);
}

void testprio() {
    if(no_arguments_func("testprio") ==-1) return;
    char * argv[] = {"test prio", NULL};
    int pid = usys_create_process((uint64_t)testprio_ps, argv, foreground, STDIN, STDOUT);
    if(foreground) 
        usys_wait_processes(pid);
}

void ps(int fd[]) {
    if(pipe == FALSE) if(no_arguments_func("ps") ==-1) return;
    char * argv[] = {"ps", NULL};
    int pid = usys_create_process((uint64_t)ps_ps, argv, foreground, fd[0], fd[1]); 
    if(foreground) 
        usys_wait_processes(pid);
}

void mem(int fd[]) {
    if(pipe == FALSE) if(no_arguments_func("mem") ==-1) return;
    char * argv[] = {"memoryinfo", NULL};
    int pid = usys_create_process((uint64_t)memoryinfo_ps, argv, foreground, fd[0], fd[1]); 
    if(foreground) 
        usys_wait_processes(pid);
}

void testsynchro(int fd[]) {
    if(pipe == FALSE) if(cant_arguments_func("testsynchro", argC, 3) == -1) return;
    char * arg_1 = foreground? arguments[1] : arguments[2];
    char * arg_2 = foreground? arguments[2] : arguments[3];
    char * argv[] = {"test synchro", arg_1, arg_2, NULL};
    int pid = usys_create_process((uint64_t)testsynchro_ps, argv, foreground, STDIN, STDOUT); 
    if(foreground) 
        usys_wait_processes(pid);
}

void loop(int fd[]){
    if(pipe == FALSE) if(no_arguments_func("loop") ==-1) return;
    char * argv[] = {"loop", NULL};
    int pid = usys_create_process((uint64_t) loop_ps, argv, foreground, fd[0], fd[1]);
    if(foreground)
        usys_wait_processes(pid);
}

void cat(int fd[]){
    if(pipe == FALSE) if(no_arguments_func("cat") ==-1) return;
    char * argv[] = {"cat", NULL};
    int pid = usys_create_process((uint64_t) cat_ps, argv, foreground, fd[0], fd[1]);
    if(foreground)
        usys_wait_processes(pid);
}

void wc(int fd[]){
    if(pipe == FALSE) if(no_arguments_func("wc")==-1) return;
    char * argv[] = {"wc", NULL};
    int pid = usys_create_process((uint64_t) wc_ps, argv, foreground, fd[0], fd[1]);
    if(foreground)
        usys_wait_processes(pid);
}

void filter(int fd[]){
    if(pipe == FALSE) if(no_arguments_func("filter")==-1) return;
    char * argv[] = {"filter", NULL};
    int pid = usys_create_process((uint64_t) filter_ps, argv, foreground, fd[0], fd[1]);
    if(foreground)
        usys_wait_processes(pid);
}

void phylo(int fd[]){
    if(pipe == FALSE)  if(no_arguments_func("phylos")==-1) return;
    char * argv[] = {"phylos", NULL};
    int pid = usys_create_process((uint64_t) phylos_ps, argv, foreground, fd[0], fd[1]);
    if(foreground)
        usys_wait_processes(pid);
}

void kill(){
    if(cant_arguments_func("kill", argC, 2) == -1) return;
    if(strcmp(arguments[1], "1") == 0 || strcmp(arguments[1], "2") == 0){
        print_error("Error: no se puede matar al proceso.");
        print(arguments[1]);
        print("\n");
        return;
    }
    char * argv[] = {"kill", arguments[1], NULL};
    int pid = usys_create_process((uint64_t) kill_ps, argv, foreground, STDIN, STDOUT);
    if(foreground)
        usys_wait_processes(pid);
}

void nice(){
    if(cant_arguments_func("nice", argC, 3) == -1) return;
    char * argv[] = {"nice", arguments[1], arguments[2], NULL};
    int pid = usys_create_process((uint64_t) nice_ps, argv, foreground, STDIN, STDOUT);
    if(foreground)
        usys_wait_processes(pid);
}

void block(){
    if(cant_arguments_func("block", argC, 2) == -1) return;
    if(strcmp(arguments[1], "1") == 0 || strcmp(arguments[1], "2") == 0){
        print_error("Error: no se puede bloquear al proceso.");
        print(arguments[1]);
        print("\n");
        return;
    }
    char * argv[] = {"block", arguments[1], NULL};  
    int pid = usys_create_process((uint64_t) block_ps, argv, foreground, STDIN, STDOUT);
    if(foreground)
        usys_wait_processes(pid);
}

void testmemory(){
    if(cant_arguments_func("testmemory", argC, 2) == -1) return;
    char * arg_command = (foreground)? arguments[1] : arguments[2];
    char * argv[] = {"test processes", arg_command,NULL};
    int pid = usys_create_process((uint64_t)testmemory_ps, argv, foreground, STDIN, STDOUT);
    if(foreground) 
        usys_wait_processes(pid);
}
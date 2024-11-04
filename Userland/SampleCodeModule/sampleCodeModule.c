// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */

#include <shell.h>

#define INPUT_SIZE 100 
#define COMMAND_COUNT 16
#define CANT_REGS 18
#define TRUE 1
#define FALSE 0


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
void testnosynchro();

static char buffer[INPUT_SIZE] = {0};
static int bufferIndex = 0;
static int currentFontSize;
static int gameActive = 0;
static char foreground = TRUE;

static Command commands[] = {
    {"help", help, "Muestra la lista de comandos"},
    {"divzero", divzero, "Simula la excepcion de division por 0"},
    {"invopcode", invopcode, "Simula la excepcion de opcode invalida"},
    {"time", time, "Muestra la hora actual"},
    {"inforeg", inforeg, "Imprime los registros capturados por CTRL"},
    {"eliminator", play_eliminator, "Inicia el juego de eliminator"},
    {"zoomin", zoomin, "Aumenta el tamanio de la letra"},
    {"zoomout", zoomout, "Disminuye el tamanio de la letra"},
    {"clear", clear_shell, "Limpia la shell"},
    {"beep", beep, "Emite un beep"},
    {"testprocess", testprocess, "Crea el proceso test_process"},
    {"testprio", testprio, "Crea procesos con distintas prioridades"},
    {"ps", ps, "Muestra los procesos y sus estados"},
    {"mem", mem, "Muestra informacion de la memoria"},
    {"testsynchro", testsynchro, "Crea el proceso para testear sincronización CON semaforos"},
    {"testnosynchro", testnosynchro, "Crea el proceso para testear sincronización CON semaforos"}
};

void parse_command(char *str) {
    char argument[] = {0};

    if (strcmp(str, "")==0) {
        return;
    }

    int argC = parse_command_arg(str);
    char cmd[10] = {0};

    char valid_cmd = TRUE;

    if (argC > 1) {
        char * flag = check_back(str, cmd);
        if(strcmp(flag, "b") == 0){
            foreground = FALSE;
            str = cmd;
        }
        else{
            valid_cmd = FALSE;
        }
    }

    if(valid_cmd){
        for (int i = 0; i < COMMAND_COUNT; i++) {
            if (strcmp(str, commands[i].name_id) == 0) {
                (*commands[i].func)(argument);
                return;
            }
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
    print_color(GREEN,"OUR_OS\n");
    print("Ingrese uno de los siguientes comandos:\n");
    for(int i = 1; i < COMMAND_COUNT-1 ; i++){
            print_color(LIGHT_BLUE, commands[i].name_id);
            print(" | ");
    } 
    print_color(LIGHT_BLUE, commands[COMMAND_COUNT-1].name_id);
    print("\nIngrese \"help\" para la descripcion los comandos.\n");
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
        }
    } 
    usys_exit();
    return 0;
}

void help() {
    print("Comandos disponibles:\n");
    for(int i = 1; i < COMMAND_COUNT ; i++){
            print("   ");
            print_color(LIGHT_BLUE, commands[i].name_id);
            print(": ");
            print(commands[i].desc);
            put_char('\n');
    } 
}

void divzero() { 
    int a = 1; //rax??
    int b = 0; 
    if ((a/b) == 1) {
        print_error("This is wrong...");
    }
}
void invopcode() {
    _invalid_opcode_exception();
}

void time() {
    print_color(GREEN, "ART (Argentine Time): UTC/GMT -3 horas\n"); 
    _get_time(); 
}

void zoomin() {
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
    usys_clear_screen();
}

void beep() {
    print_color(GREEN, "BEEP!!\n");
    usys_beep(1000, 10);
}

void play_eliminator() {
    gameActive = 1;
    clear_shell();
    eliminator();
    clear_shell(); 
    gameActive = 0;
}

void testprocess() {
    char * argv[] = {"test processes", "3" ,NULL};
    int pid = usys_create_process((uint64_t)testprocess_ps, usys_get_pid() , 1 , 2, argv);
    if(foreground) 
        usys_wait_processes(pid);
}

void testprio() {
    char * argv[] = {"test prio", NULL};
    int pid = usys_create_process((uint64_t)testprio_ps, usys_get_pid(), 1, 1, argv);
    if(foreground) 
        usys_wait_processes(pid);
}

void ps() {
    char * argv[] = {"ps", NULL};
    int pid = usys_create_process((uint64_t)ps_ps, usys_get_pid(), 1, 1, argv); 
    if(foreground) 
        usys_wait_processes(pid);
}

void mem() {
    char * argv[] = {"memoryinfo", NULL};
    int pid = usys_create_process((uint64_t)memoryinfo_ps, usys_get_pid(), 1, 1, argv); 
    if(foreground) 
        usys_wait_processes(pid);
}

void testsynchro() {
    char * argv[] = {"test synchro", "7", "1", NULL};
    int pid = usys_create_process((uint64_t)testsynchro_ps, usys_get_pid(), 1, 3, argv); 
    if(foreground) 
        usys_wait_processes(pid);
}

void testnosynchro() {
    char * argv[] = {"test synchro", "5", "0", NULL};
    int pid = usys_create_process((uint64_t)testsynchro_ps, usys_get_pid(), 1, 3, argv); 
    if(foreground) 
        usys_wait_processes(pid);
}
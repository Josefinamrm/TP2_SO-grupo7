#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>

// Definición de constantes para los identificadores de excepciones
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

static void zero_division();
static void invalid_opcode();
static void showRegs();


extern uint64_t regs[];
char* regsNames[18] = {"rax:", "rbx:", "rcx:", "rdx:", "rsi:", "rdi:", "rbp:", "rsp:", "r8:", "r9:",
                       "r10:", "r11:", "r12:", "r13:", "r14:", "r15:", "rip:", "rflags:"};

// Función para manejar las excepciones según su identificador
void exceptionDispatcher(int exception) {
	clearScreen();

	if (exception == ZERO_EXCEPTION_ID){
		zero_division();
	}

	if(exception == INVALID_OPCODE_ID){
		invalid_opcode();
	}
}

static void zero_division() {
	printArrayWithColor(RED, BLACK, "Error: Division por cero\n");
	showRegs();
	printArrayWithColor(YELLOW, BLACK, "Presione cualquier letra para continuar\n");
	while(getChar() == 0){
		_hlt();
	};
	clearScreen();
}

static void invalid_opcode() {
	printArrayWithColor(RED, BLACK, "Error: Opcode invalido\n");
	showRegs();		
	printArrayWithColor(YELLOW, BLACK, "Presione cualquier letra para continuar\n");
	while(getChar() == 0){
		_hlt();
	};
	clearScreen();
}

static void showRegs(){
	for (int i = 0; i < 18; i++){
        printArrayWithColor(RED, BLACK, regsNames[i]);
        printHex(regs[i]);
        newLine();
    }
}
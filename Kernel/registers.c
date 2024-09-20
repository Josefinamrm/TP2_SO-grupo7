#include <registers.h>
#include <videoDriver.h>

extern uint64_t regs[];
extern uint8_t regsSaved;

int getRegisters(uint64_t registers[]){
    if(!regsSaved){
        // printArray("return 0 de getRegisters\n");
        return 0;
    }

    for(int i = 0; i < 18; i++){
        registers[i] = regs[i];
    }
    // printArray("return 1 de getRegisters\n");
    return 1;
}
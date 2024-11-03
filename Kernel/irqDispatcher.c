// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <time.h>
#include <stdint.h>
#include <keyboardDriver.h>

static uint64_t int_20(uint64_t rsp);
static uint64_t int_21();
uint64_t scheduler(uint64_t rsp);

					//    rdi             rsi
uint64_t irqDispatcher(uint64_t irq, uint64_t rsp) {
	switch (irq) {
		case 0:
			return int_20(rsp);
			break;
		case 1: 
			return int_21();
			break; 
	}
	return 0;
}

uint64_t int_20(uint64_t rsp) {
	timer_handler();
	return scheduler(rsp);
}

uint64_t int_21() { 
	keyboard_handler();
	return 0;
}
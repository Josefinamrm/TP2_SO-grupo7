 /*
 *   interrupts.h
 *
 *  Created on: Apr 18, 2010
 *      Author: anizzomc
 */

#ifndef INTERRUPS_H_
#define INTERRUPS_H_

#include <idtLoader.h>
#include <stdint.h>

#define USERLAND_DIREC 0x400000


void _irq00Handler(void);
void _irq01Handler(void);
void _irq02Handler(void);
void _irq03Handler(void);
void _irq04Handler(void);
void _irq05Handler(void);

void _exception0Handler(void);
void _exception6Handler(void);

void _syscallHandler(void);

void _cli(void);

void _sti(void);

void _hlt(void);

void _idle(void);

void _setUser();

void picMasterMask(uint8_t mask);

void picSlaveMask(uint8_t mask);


uint64_t _setup_stack_structure_asm(uint64_t top_stack, uint64_t function, uint64_t argc, uint64_t argv);


#endif /* INTERRUPS_H_ */

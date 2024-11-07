#ifndef _KEYBOARDDRIVER_H_ 
#define _KEYBOARDDRIVER_H_ 

#include <stdint.h>
#include <stdio.h>
#include <naiveConsole.h>
#include <interrupts.h>
#include <lib.h>
#include <processManager.h>
#include <videoDriver.h>
#include <semaphores.h>

#define ENTER_KEY 0x1C
#define LSHIFT_KEY 0x2a
#define LSHIFT_RELEASE_KEY 0xAA // no me toma 0xAA solo 
#define CAPS_LOCK 0x3a
#define ESC_KEY 27
#define LCTRL_KEY 0x1D
#define LCTRL_RELEASE_KEY 0x9D

#define EOF (-1)


#define BUFFER_SIZE 256 // verlo desp 

void initialize_keyboard_driver();

void keyboard_handler(); 

extern char getScanCode();

char get_char_from_buffer();

char get_last_char_from_buffer();

void flushBuffer();

#endif

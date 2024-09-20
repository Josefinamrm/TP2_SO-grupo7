#ifndef _KEYBOARDDRIVER_H_ 
#define _KEYBOARDDRIVER_H_ 

#include <stdint.h>
#include <stdio.h>
#include <naiveConsole.h>
#include <interrupts.h>
#include <lib.h>

#define ENTER_KEY 0x1C
#define LSHIFT_KEY 0x2a
#define LSHIFT_RELEASE_KEY 0xAA // no me toma 0xAA solo 
#define CAPS_LOCK 0x3a
#define ESC_KEY 27

#define BUFFER_SIZE 256 // verlo desp 

void keyboard_handler(); 

extern char getScanCode();

char getChar();

char getLastChar();

void flushBuffer();

#endif

#ifndef USER_LIB_H
#define USER_LIB_H

#include <stdint.h>
#include <user_syscalls.h>
#include <ucolors.h>
#define INPUT_SIZE 100

void print(char *arr);

int strlen(char *arr);

void strcpy(char *destination, const char *source);

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

void _invalid_opcode_exception(void);

int parseCommandArg(char * str);

int strcmp(const char *s1, const char *s2);

char getChar(); 

char getLastChar();

void putChar(char c); 

char getLastChar(); 

void printColor(int fontColor, char * string);

void printError(char * string);

void intToString(int num, char* buf, int dim);

void printScore(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, int num);

void udrawArray(uint32_t fontColor, uint32_t backgroundColor, int x, int y, char *arr);

void udrawFrame(uint32_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y, uint64_t thickness);


#endif
#ifndef USER_LIB_H
#define USER_LIB_H

#include <stdint.h>
#include <user_syscalls.h>
#include <ucolors.h>
#include <stddef.h>

#define INPUT_SIZE 100

void print(char *arr);

int strlen(char *arr);

void strcpy(char *destination, const char *source);

uint32_t uint_to_base(uint64_t value, char *buffer, uint32_t base);

void _invalid_opcode_exception(void);

int parse_command_arg(char * str, char *arguments[]);

int strcmp(const char *s1, const char *s2);

char get_char(); 

char get_last_char();

void put_char(char c); 

char get_last_char(); 

void print_color(int fontColor, char * string);

void print_error(char * string);

void int_to_string(int num, char* buf, int dim);

void print_score(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, int num);

void print_dec(int num);

void udraw_array(uint32_t fontColor, uint32_t backgroundColor, int x, int y, char *arr);

void udraw_frame(uint32_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y, uint64_t thickness);


#endif
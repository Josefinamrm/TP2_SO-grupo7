#ifndef USER_SYSCALLS_H
#define USER_SYSCALLS_H
#include <stdint.h>
#include <interrupts.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define LASTIN 3


int usys_read(int fd, char * buf, int size);

int usys_write(int fd, char * buf, int size);

int usys_write_color(uint32_t fontColor, char * buf, int size);

void usys_change_draw_size(int size);

void usys_print_array_of_draw_size(uint32_t fontColor, uint32_t backgroundColor, int x, int y, char * buf);

void usys_print_square(uint32_t fontColor, int x, int y, int size);

void usys_print_rect(uint32_t color, int x, int y, int size_x, int size_y);

int usys_get_registers(uint64_t * registers);

void usys_wait(int ms);

void usys_clear_screen();

void usys_change_font_size(int new_font_size);

int usys_get_font_size();

void usys_beep(int frec, int time_ms);

int usys_get_time(); 

void flush_buffer();


// processes syscalls

uint64_t usys_get_pid();

int64_t usys_create_process(uint64_t function, uint64_t ppid, uint64_t priority, uint64_t argc, uint8_t ** argv);

void usys_nice(uint64_t pid, uint64_t newPrio);

int64_t usys_kill(uint64_t pid);

int64_t usys_block(uint64_t pid);

int64_t usys_unblock(uint64_t pid);

void usys_yield();

void usys_wait_processes();

void usys_ps();

void usys_wait_pid(uint64_t pid);


#endif
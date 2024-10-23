#ifndef SYSCALLDISPATCHER_H
#define SYSCALLDISPATCHER_H

#include <stdint.h>
#include <videoDriver.h>
#include <registers.h>
#include <time.h>
#include <keyboardDriver.h> 
#include <clock.h>
#include <sound.h>
#include "processManager.h"
#include "memoryManager.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define LASTIN 3
#define TIME_STR 10

extern char getScanCode();
extern int getTime(int timeUnit);
extern void _hlt();

uint64_t ksys_read(uint64_t fd, uint64_t buffer, uint64_t count);
uint64_t ksys_write(uint64_t fd, uint64_t buffer, uint64_t count);
uint64_t ksys_change_draw_size(uint64_t size);
uint64_t ksys_print_array_of_draw_size(uint64_t fontColor, uint64_t backgroundColor, uint64_t x, uint64_t y, uint64_t buf);
uint64_t ksys_write_color(uint64_t fontColor, uint64_t buffer, uint64_t count);
uint64_t ksys_getRegisters(uint64_t rdi);
uint64_t ksys_wait(uint64_t seconds);
uint64_t ksys_clear_screen();
uint64_t ksys_change_font_size(uint64_t font);
uint64_t ksys_get_font_size();
uint64_t ksys_beep(uint64_t frec, uint64_t time_ms);
uint64_t ksys_getTime();
uint64_t ksys_draw_square(uint64_t color, uint64_t x, uint64_t y, uint64_t size);
uint64_t ksys_draw_rect(uint64_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y);
uint64_t ksys_draw_array(uint64_t fontColor, uint64_t backgroundColor, uint64_t x, uint64_t y, uint64_t arr);
uint64_t ksys_getpid();
int64_t ksys_create_process(uint64_t function, uint64_t ppid, uint64_t priority, uint64_t argc, uint64_t argv);
uint64_t ksys_nice(uint64_t pid, uint64_t newPrio);
int64_t ksys_kill(uint64_t pid);
int64_t ksys_block(uint64_t pid);
int64_t ksys_unblock(uint64_t pid);
uint64_t ksys_yield();
uint64_t ksys_ps();
uint64_t ksys_wait_processes(uint64_t pid);
uint64_t ksys_exit();
uint64_t ksys_malloc(unsigned int nbytes);
uint64_t ksys_realloc(void * ptr, uint64_t new_size); 
uint64_t ksys_free(void * ptr);
uint64_t ksys_unused_space();
uint64_t ksys_occupied_space();
uint64_t ksys_total_space();

#endif
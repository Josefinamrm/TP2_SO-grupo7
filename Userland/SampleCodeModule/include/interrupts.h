#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

uint64_t _read(uint64_t fd, uint64_t buf, uint64_t size);

uint64_t _write(uint64_t fd, uint64_t buf, uint64_t size);

uint64_t _change_draw_size(uint64_t size);

uint64_t _print_array_of_draw_size(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, uint64_t buf);

uint64_t _write_color(uint64_t fontCOlor, uint64_t buf, uint64_t size);

uint64_t _getRegisters(uint64_t * registers);

uint64_t _wait(uint64_t ms);

uint64_t _clear_screen();

uint64_t _change_font_size(uint64_t new_font_size);

uint64_t _get_font_size();

uint64_t _beep(uint64_t frec, uint64_t time);

uint64_t _print_square(uint64_t color, uint64_t x, uint64_t y, uint64_t size);

uint64_t _print_rect(uint64_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y);

uint64_t _get_time();

uint64_t _draw_array(uint64_t fontColor, uint64_t backgroundColor, uint64_t x, uint64_t y, uint64_t arr);

uint64_t _flush_buffer();

uint64_t _get_pid();

int64_t _create_process(uint64_t function, uint64_t argv, uint64_t foreground, uint64_t read_fd, uint64_t write_fd);

int64_t _nice(uint64_t pid, uint64_t newPrio);

int64_t _kill(uint64_t pid);

int64_t _block(uint64_t pid);

int64_t _unblock(uint64_t pid);

void _yield();

void _wait_processes(uint64_t pid);

void _ps();

void _exit();

// memory syscalls

void * _malloc(uint64_t nbytes);

void * _realloc(uint64_t ptr, uint64_t new_size); 

void _free(uint64_t ptr);

int _unused_space();

int _occupied_space();

int _total_space();

// semaphore syscalls

int16_t _sem_open(uint64_t name, uint64_t value);

void _sem_close(uint64_t name);

void _sem_post(uint64_t name);

void _sem_wait(uint64_t name);

// ipc 

int16_t _open_pipe(uint64_t file_descriptors[2]);

int16_t _close_pipe(int16_t pipe_id);

int16_t _write_pipe(int16_t pipe_id, char * buf, int to_write);

int16_t _read_pipe(int16_t pipe_id, char * buf, int to_read);

int16_t _open_fd(int8_t type, int8_t permission, int16_t pipe_id, int16_t process_pid);

void _close_fd(int16_t fd_number);

int16_t _close_all_fds(int16_t pid);

int64_t _write_to_fd(int16_t fd_number, char * buffer, int to_write);

int64_t _read_from_fd(int16_t fd_number, char * buffer, int to_read);
#endif



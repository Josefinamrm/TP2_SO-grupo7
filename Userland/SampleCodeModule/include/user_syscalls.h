#ifndef USER_SYSCALLS_H
#define USER_SYSCALLS_H
#include <interrupts.h>

typedef enum {STDIN = 0, STDOUT, STDERR, BACKGROUND, PIPE, LASTIN} Type;

#define READ 0
#define WRITE 1

#define MAX_PROCESSES 200
typedef struct{
    char * name;
    int16_t pid;
    uint8_t priority;
    char * state;
    uint64_t stack_pointer;
    char * foreground;
}process_view;


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

int64_t usys_create_process(uint64_t function, char ** argv, uint8_t foreground, int16_t read_fd, int16_t write_fd);

int64_t usys_nice(int16_t pid, uint8_t new_prio);

int64_t usys_kill(int16_t pid);

int64_t usys_block(int16_t pid);

int64_t usys_unblock(int16_t pid);

void usys_yield();

void usys_wait_processes(int16_t pid);

uint64_t usys_get_process_info(process_view processes[MAX_PROCESSES]);

void usys_exit();

// memory syscalls

void * usys_malloc(unsigned int nbytes);

void * usys_realloc(void * ptr, uint64_t new_size); 

void usys_free(void * ptr);

int usys_unused_space();

int usys_occupied_space();

int usys_total_space();

// semaphores syscalls

int64_t usys_sem_open(char * name, int value);

void usys_sem_close(char * name);

void usys_sem_post(char * name);

void usys_sem_wait(char * name);

// ipc

int64_t usys_open_pipe(int file_descriptors[2]);

int64_t usys_close_pipe(int16_t pipe_id);

int64_t usys_write_pipe(int16_t pipe_id, char * buf, int to_write);

int64_t usys_read_pipe(int16_t pipe_id, char * buf, int to_read);

int64_t usys_open_fd(int8_t type, int8_t permission, int16_t pipe_id);

void usys_close_fd(int16_t fd_number);

#endif
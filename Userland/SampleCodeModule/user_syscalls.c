// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <user_syscalls.h>
#include <interrupts.h>

#define INPUT_SIZE 100

int usys_read(int fd, char * buf, int size) {
    return _read((uint64_t) fd, (uint64_t) buf, (uint64_t) size);
}

int usys_write(int fd, char * buf, int size){
    return _write((uint64_t) fd, (uint64_t) buf, (uint64_t) size);
}

int usys_write_color(uint32_t fontColor, char * buf, int size) {
    return _write_color((uint64_t) fontColor, (uint64_t) buf, (uint64_t) size);
}

void usys_change_draw_size(int size){
    _change_draw_size((uint64_t) size);
}

void usys_print_array_of_draw_size(uint32_t fontColor, uint32_t backgroundColor, int x, int y, char * buf){
    _print_array_of_draw_size((uint64_t) fontColor, (uint64_t) backgroundColor, (uint64_t) x, (uint64_t) y, (uint64_t) buf);
}

void usys_print_square(uint32_t fontColor, int x, int y, int size){
    _print_square((uint32_t) fontColor, (uint64_t) x, (uint64_t) y, (uint64_t) size);
}

void usys_print_rect(uint32_t color, int x, int y, int size_x, int size_y) {
    _print_rect((uint32_t) color, (uint64_t) x, (uint64_t) y, (uint64_t) size_x, (uint64_t) size_y);
}

int usys_get_registers(uint64_t * registers){
    return _getRegisters(registers);
}

void usys_wait(int ms){
    _wait((uint64_t ) ms);
}

void usys_clear_screen() {
    _clear_screen();
}

void usys_change_font_size(int new_font_size) {
    _change_font_size(new_font_size);
}

int usys_get_font_size() {
    return _get_font_size();
}

void usys_beep(int frec, int time_ms) {
    _beep((uint32_t)frec, (uint64_t) time_ms);
}

int usys_get_time() {
    return _get_time();
}

void flush_buffer(){
    _flush_buffer();
}

uint64_t usys_get_pid(){
    return _get_pid();
}

int64_t usys_create_process(uint64_t function, char ** argv, uint8_t foreground, int16_t read_fd, int16_t write_fd){
    return _create_process(function, (uint64_t) argv, (uint64_t)foreground, (uint64_t)read_fd, (uint64_t)write_fd);
}

int64_t usys_nice(int16_t pid, uint8_t newPrio){
    return _nice((uint64_t)pid, (uint64_t)newPrio);
}

int64_t usys_kill(int16_t pid){
    return _kill((uint64_t)pid);
}

int64_t usys_block(int16_t pid){
    return _block((uint64_t)pid);
}

int64_t usys_unblock(int16_t pid){
   return _unblock((uint64_t)pid);
}

void usys_yield(){
    _yield();
}

void usys_wait_processes(int16_t pid){
    _wait_processes((uint64_t)pid);
}

uint64_t usys_get_process_info(process_view processes[MAX_PROCESSES]){
    return _get_processes_info((uint64_t)processes);
}

void usys_exit(){
    _exit();
}


void * usys_malloc(unsigned int nbytes){
    return _malloc((uint64_t)nbytes);
}

void * usys_realloc(void * ptr, uint64_t new_size){
    return _realloc((uint64_t)ptr, new_size);
} 

void usys_free(void * ptr){
    _free((uint64_t)ptr);
}

int usys_unused_space(){
    return _unused_space();
}

int usys_occupied_space(){
    return _occupied_space();
}

int usys_total_space(){
    return _total_space();
}

int64_t usys_sem_open(char * name, int value){
    return _sem_open((uint64_t)name, (uint64_t)value);
}

void usys_sem_close(char * name){
    _sem_close((uint64_t)name);
}

void usys_sem_post(char * name){
    _sem_post((uint64_t)name);
}

void usys_sem_wait(char * name){
    _sem_wait((uint64_t)name);
}

int64_t usys_open_pipe(int file_descriptors[2]){
    return _open_pipe((uint64_t)file_descriptors);
}

int64_t usys_close_pipe(int16_t pipe_id){
    return _close_pipe((uint64_t)pipe_id);
}

int64_t usys_write_pipe(int16_t pipe_id, char * buf, int to_write){
    return _write_pipe((uint64_t)pipe_id, (uint64_t)buf, (uint64_t)to_write);
}

int64_t usys_read_pipe(int16_t pipe_id, char * buf, int to_read){
    return _read_pipe((uint64_t)pipe_id, (uint64_t)buf, (uint64_t)to_read);
}

int64_t usys_open_fd(int8_t type, int8_t permission, int16_t pipe_id){
    return _open_fd((uint64_t)type, (uint64_t)permission, (uint64_t)pipe_id);
}

void usys_close_fd(int16_t fd_number){
    _close_fd((uint64_t)fd_number);
}
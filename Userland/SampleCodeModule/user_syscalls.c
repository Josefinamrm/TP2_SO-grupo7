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

int64_t usys_create_process(uint64_t function, uint64_t ppid, uint64_t priority, uint64_t argc, uint8_t ** argv){
    return _create_process(function, ppid, priority, argc, argv);
}

void usys_nice(uint64_t pid, uint64_t newPrio){
    _nice(pid, newPrio);
}

int64_t usys_kill(uint64_t pid){
    return _kill(pid);
}

int64_t usys_block(uint64_t pid){
    return _block(pid);
}

int64_t usys_unblock(uint64_t pid){
   return _unblock(pid);
}

void usys_yield(){
    _yield();
}

void usys_wait_processes(){
    _wait_processes();
}

void usys_ps(){
    return _ps();
}

void usys_wait_pid(uint64_t pid){
    _wait_pid(pid);
}
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <syscallDispatcher.h>
#include <semaphores.h>

int64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t rax)
{
    switch (rax)
    {
        case 0:
            return ksys_read(rdi, rsi, rdx);
        case 1:
            return ksys_write(rdi, rsi, rdx);
        case 2:
            return ksys_getTime();
        case 3:
            return ksys_change_draw_size(rdi);
        case 4:
            return ksys_print_array_of_draw_size(rdi, rsi, rdx, rcx, r8);
        case 5:
            return ksys_draw_array(rdi, rsi, rdx, rcx, r8);
        case 6:
            return ksys_write_color(rdi, rsi, rdx);
        case 7:
            return ksys_clear_screen();
        case 8:
            return ksys_getRegisters(rdi);
        case 9:
            return ksys_sleep(rdi);
        case 10:
            return ksys_change_font_size(rdi);
        case 11:
            return ksys_get_font_size();
        case 12:
            return ksys_beep(rdi, rsi);
        case 13:
            return ksys_draw_square(rdi, rsi, rdx, rcx);
        case 14:
            return ksys_draw_rect(rdi, rsi, rdx, rcx, r8);
        case 15:
            flushBuffer();
            return FINISH_SUCCESFULLY;
        case 16:
            return ksys_getpid();
        case 17:
            return ksys_create_process(rdi, rsi, rdx, rcx, r8);
        case 18:
            return ksys_nice(rdi, rsi);
        case 19:
            return ksys_kill(rdi);
        case 20:
            return ksys_block(rdi);
        case 21:
            return ksys_unblock(rdi);
        case 22:
            return ksys_yield();
        case 23:
            return ksys_wait_processes(rdi);
        case 24:
            return ksys_get_process_info(rdi);
        case 25:
            return ksys_exit();
        case 26:
            return ksys_malloc(rdi);
        case 27:
            return ksys_realloc(rdi, rsi);
        case 28:
            return ksys_free(rdi);
        case 29:    
            return ksys_unused_space();
        case 30:
            return ksys_occupied_space();
        case 31:
            return ksys_total_space();
        case 32:
            return ksys_sem_open(rdi, rsi);
        case 33:
            return ksys_sem_close(rdi);
        case 34:
            return ksys_sem_post(rdi);
        case 35:    
            return ksys_sem_wait(rdi);
        case 36:
            return ksys_open_pipe(rdi);
        case 37:
            return ksys_close_pipe(rdi);
        case 38:
            return ksys_write_pipe(rdi, rsi, rdx);
        case 39:
            return ksys_read_pipe(rdi, rsi, rdx);
        case 40:
            return ksys_open_fd(rdi, rsi, rdx);
        case 41:
            return ksys_close_fd(rdi);
    }



    return FINISH_SUCCESFULLY;
}

int64_t ksys_read(uint64_t fd, uint64_t buffer, uint64_t count)
{
    return read_from_fd((int16_t)fd, (char *)buffer, (int)count);
}

// check error 
int64_t ksys_write(uint64_t fd, uint64_t buffer, uint64_t count)
{
    return write_to_fd((int16_t)fd, (char *) buffer, (int)count);
}

uint64_t ksys_getTime()
{
    char reserve[TIME_STR];
    timeToStr(reserve);
    printArray(reserve);
    return FINISH_SUCCESFULLY;
}

uint64_t ksys_change_draw_size(uint64_t size)
{
    changeDrawSize(size);
    return FINISH_SUCCESFULLY;
}

uint64_t ksys_print_array_of_draw_size(uint64_t fontColor, uint64_t backgroundColor, uint64_t x, uint64_t y, uint64_t buf)
{
    drawArrayOfSize(fontColor, backgroundColor, x, y, (char *)buf);
    return FINISH_SUCCESFULLY;
}

uint64_t ksys_write_color(uint64_t fontColor, uint64_t buffer, uint64_t count)
{
    printArrayOfDimWithColor(fontColor, BLACK, (char *)buffer, count);
    return count;
}

uint64_t ksys_getRegisters(uint64_t rdi)
{
    uint64_t toRet = getRegisters((uint64_t *)rdi);
    return toRet;
}

uint64_t ksys_sleep(uint64_t s)
{
    sleep_s(s);
    return FINISH_SUCCESFULLY;
}

uint64_t ksys_clear_screen()
{
    clearScreen();
    return FINISH_SUCCESFULLY;
}

uint64_t ksys_change_font_size(uint64_t font)
{
    changeFontSize(font);
    return FINISH_SUCCESFULLY;
}

uint64_t ksys_get_font_size()
{
    return getFontSize();
}

uint64_t ksys_beep(uint64_t frec, uint64_t time_ms)
{
    beep((uint32_t)frec, (int)time_ms);
    return FINISH_SUCCESFULLY;
}

uint64_t ksys_draw_square(uint64_t color, uint64_t x, uint64_t y, uint64_t size)
{
    drawSquare(color, x, y, size);
    return FINISH_SUCCESFULLY;
}

uint64_t ksys_draw_rect(uint64_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y)
{
    drawRect(color, x, y, size_x, size_y);
    return FINISH_SUCCESFULLY;
}

uint64_t ksys_draw_array(uint64_t fontColor, uint64_t backgroundColor, uint64_t x, uint64_t y, uint64_t arr)
{
    drawArray(fontColor, backgroundColor, x, y, (char *) arr);
    return FINISH_SUCCESFULLY;
}

uint64_t ksys_getpid(){
    return my_getpid();
}


int64_t ksys_create_process(uint64_t function, uint64_t argv, uint64_t foreground, uint64_t read_fd, uint64_t write_fd)
{
    return my_create_process(function, (char **)argv, (uint8_t)foreground, (int)read_fd, (int)write_fd);
}

int64_t ksys_nice(uint64_t pid, uint64_t newPrio){
    my_nice((int16_t)pid, (uint8_t)newPrio);
    return FINISH_SUCCESFULLY;
}

int64_t ksys_kill(uint64_t pid){
    return  my_kill((int16_t)pid);
}

int64_t ksys_block(uint64_t pid){
    return my_block((int16_t)pid);
}

int64_t ksys_unblock(uint64_t pid){
    return my_unblock((int16_t)pid);
}

uint64_t ksys_yield(){
    my_yield();
    return FINISH_SUCCESFULLY;
}


uint64_t ksys_wait_processes(uint64_t pid){
    my_wait((int16_t)pid);

    return FINISH_SUCCESFULLY;
}

uint64_t ksys_get_process_info(uint64_t processes){
    return get_process_info((process_view *)processes);
}


uint64_t ksys_exit(){
    my_exit();
    return FINISH_SUCCESFULLY;
}

uint64_t ksys_malloc(uint64_t nbytes){
    return (uint64_t) mm_malloc(nbytes);
}

uint64_t ksys_realloc(uint64_t ptr, uint64_t new_size){
    return (uint64_t) mm_realloc((void *)ptr, new_size);
}

uint64_t ksys_free(uint64_t ptr){
    mm_free((void *)ptr);
    return FINISH_SUCCESFULLY;
}

uint64_t ksys_unused_space(){
    return (uint64_t) mm_unused_space();
}

uint64_t ksys_occupied_space(){
    return (uint64_t) mm_occupied_space();
}

uint64_t ksys_total_space(){
    return (uint64_t) mm_total_space();
}

int64_t ksys_sem_open(uint64_t name, uint64_t value){
    return my_sem_open((char *)name, (int)value);
}

uint64_t ksys_sem_close(uint64_t name){
    my_sem_close((char *)name);
    return FINISH_SUCCESFULLY;
}

uint64_t ksys_sem_post(uint64_t name){
    my_sem_post((char *)name);
    return FINISH_SUCCESFULLY;
}

uint64_t ksys_sem_wait(uint64_t name){
    my_sem_wait((char *)name);
    return FINISH_SUCCESFULLY;
}

uint64_t ksys_open_pipe(uint64_t file_descriptors[2]){
    return open_pipe((uint8_t *)file_descriptors);
}

uint64_t ksys_close_pipe(uint64_t pipe_id){
    return close_pipe((int16_t)pipe_id);
}

uint64_t ksys_write_pipe(uint64_t pipe_id, uint64_t buf, uint64_t to_write){
    return write_pipe((int16_t)pipe_id, (char *)buf, (int)to_write);
}

uint64_t ksys_read_pipe(uint64_t pipe_id, uint64_t buf, uint64_t to_read){
    return read_pipe((int16_t)pipe_id, (char *)buf, (int)to_read);
}

uint64_t ksys_open_fd(uint64_t type, uint64_t permission, uint64_t pipe_id){
    return open_fd((uint8_t)type, (uint8_t)permission, (int16_t)pipe_id);
}

uint64_t ksys_close_fd(uint64_t fd_number){
    close_fd((int16_t)fd_number);
    return FINISH_SUCCESFULLY;
}

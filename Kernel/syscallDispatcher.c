// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "syscallDispatcher.h"

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
            return ksys_wait(rdi);
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
            return ksys_ps();
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
    }



    return FINISH_SUCCESFULLY;
}

uint64_t ksys_read(uint64_t fd, uint64_t buffer, uint64_t count)
{
    int i = 0;
    char c;
    char *buff = (char *)buffer;
    int16_t fd_type = get_type((int16_t)fd);

    if(fd == -1){
        FINISH_ON_ERROR;
    }

    switch (fd_type){
    case STDIN:
        while(i<count){
            c = get_char_from_buffer();
            buff[i++] = c;
        }
        break;

    // por ahora, después capaz cambiarlo al fd
    case PIPE:
        int16_t pipe_id = get_id(fd);
        if(pipe_id != -1){
            i = read_pipe(pipe_id, (char *)buff, count);
        }
        break;
    
    default:
        break;
    }

    return i;

    /* if (fd == STDIN){
        while (i < count && (c = get_char_from_buffer()) != 0){
            buff[i++] = c;
        }
        return i;
    }
    else if (fd == LASTIN){
        while (i < count && (c = get_last_char_from_buffer()) != 0){
            buff[i++] = c;
        }
        return i;
    }
    return 0; */
}

// check error 
uint64_t ksys_write(uint64_t fd, uint64_t buffer, uint64_t count)
{
    int16_t fd_type = get_type((int16_t)fd);
    if(fd == -1){
        return FINISH_ON_ERROR;
    }
    switch (fd_type)
    {
    case STDERR:
        printArrayOfDimWithColor(RED, BLACK, (char *)buffer, count);
        break;
    
    case PIPE:
        int16_t pipe_id = get_id(fd);
        if(pipe_id != -1){
            return write_pipe(pipe_id, (char *)buffer, count);
        }
        break;
    
    default:
        printArrayOfDimWithColor(WHITE, BLACK, (char *)buffer, count);
        break;
    }
    return count;
}

uint64_t ksys_getTime()
{
    // char * reserve = "";
    char reserve[TIME_STR]; // en reserve queda guardado el time en formato hh:mm:ss
    timeToStr(reserve);
    // print(reserve);   ver cual funcion uso para imprimir el string
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

uint64_t ksys_wait(uint64_t ms)
{
    timer_wait_ms(ms);
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

uint64_t ksys_nice(uint64_t pid, uint64_t newPrio){
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

uint64_t ksys_ps(){
    my_ps();
    return FINISH_SUCCESFULLY;
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

uint64_t ksys_sem_open(uint64_t name, uint64_t value){
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


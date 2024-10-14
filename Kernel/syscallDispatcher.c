#include <stdint.h>
#include <videoDriver.h>
#include <registers.h>
#include <time.h>
#include <keyboardDriver.h> 
#include <clock.h>
#include <sound.h>
#include "processManager.h"

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
uint64_t ksys_create_process(uint64_t function, uint64_t ppid, uint64_t priority, uint64_t argc, uint8_t ** argv);
uint64_t ksys_nice(uint64_t pid, uint64_t newPrio);
uint64_t ksys_kill(uint64_t pid);
uint64_t ksys_block(uint64_t pid);
uint64_t ksys_unblock(uint64_t pid);
uint64_t ksys_yield();
uint64_t ksys_ps_wait(int64_t pid);

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t rax)
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
        return 0;
    case 16:
        return ksys_getpid();
    case 17:
        return ksys_create_process(rdi, rsi, rdx, rcx, (uint8_t **)r8);
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
        return ksys_ps_wait(rdi);
    }


    return 0;
}

uint64_t ksys_read(uint64_t fd, uint64_t buffer, uint64_t count)
{
    int i = 0;
    char c;
    char *buff = (char *)buffer;
    if (fd == STDIN)
    {
        while (i < count && (c = get_char_from_buffer()) != 0)
        {
            buff[i++] = c;
        }
        return i;
    }
    else if (fd == LASTIN)
    {
        while (i < count && (c = get_last_char_from_buffer()) != 0)
        {
            buff[i++] = c;
        }
        return i;
    }
    return 0;
}

uint64_t ksys_write(uint64_t fd, uint64_t buffer, uint64_t count)
{
    if (fd == STDOUT)
    {
        printArrayOfDimWithColor(WHITE, BLACK, (char *)buffer, count);
    }
    else if (fd == STDERR)
    {
        printArrayOfDimWithColor(RED, BLACK, (char *)buffer, count);
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
    return 0;
}

uint64_t ksys_change_draw_size(uint64_t size)
{
    changeDrawSize(size);
    return 0;
}

uint64_t ksys_print_array_of_draw_size(uint64_t fontColor, uint64_t backgroundColor, uint64_t x, uint64_t y, uint64_t buf)
{
    drawArrayOfSize(fontColor, backgroundColor, x, y, (char *)buf);
    return 0;
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
    return 0;
}

uint64_t ksys_clear_screen()
{
    clearScreen();
    return 0;
}

uint64_t ksys_change_font_size(uint64_t font)
{
    changeFontSize(font);
    return 0;
}

uint64_t ksys_get_font_size()
{
    return getFontSize();
}

uint64_t ksys_beep(uint64_t frec, uint64_t time_ms)
{
    beep((uint32_t)frec, (int)time_ms);
    return 0;
}

uint64_t ksys_draw_square(uint64_t color, uint64_t x, uint64_t y, uint64_t size)
{
    drawSquare(color, x, y, size);
    return 0;
}

uint64_t ksys_draw_rect(uint64_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y)
{
    drawRect(color, x, y, size_x, size_y);
    return 0;
}

uint64_t ksys_draw_array(uint64_t fontColor, uint64_t backgroundColor, uint64_t x, uint64_t y, uint64_t arr)
{
    drawArray(fontColor, backgroundColor, x, y, (char *) arr);
    return 0;
}

uint64_t ksys_getpid(){
    return my_getpid();
}


uint64_t ksys_create_process(uint64_t function, uint64_t ppid, uint64_t priority, uint64_t argc, uint8_t ** argv)
{
    return my_create_process(function, ppid, priority, argc, argv);
}

uint64_t ksys_nice(uint64_t pid, uint64_t newPrio){
    my_nice(pid, newPrio);
    return 0;
}

uint64_t ksys_kill(uint64_t pid){
    return  my_kill(pid);
}

uint64_t ksys_block(uint64_t pid){
    return my_block(pid);
}

uint64_t ksys_unblock(uint64_t pid){
    return my_unblock(pid);
}

uint64_t ksys_yield(){
    my_yield();
    return 0;
}

uint64_t ksys_ps_wait(int64_t pid){
    my_wait(pid);
    return 0;
}

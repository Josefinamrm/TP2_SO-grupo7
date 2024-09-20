#include <stdint.h>
#include <videoDriver.h>
#include <registers.h>
#include <time.h>
#include <keyboardDriver.h> 
#include <clock.h>
#include <sound.h>

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
        while (i < count && (c = getChar()) != 0)
        {
            buff[i++] = c;
        }
        return i;
    }
    else if (fd == LASTIN)
    {
        while (i < count && (c = getLastChar()) != 0)
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

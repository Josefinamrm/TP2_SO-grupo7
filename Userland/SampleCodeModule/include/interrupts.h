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


#endif



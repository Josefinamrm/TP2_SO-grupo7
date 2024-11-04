#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
char *cpuVendor(char *result);

int getHours();
int getMinutes();
int getSeconds();
char getScanCode();
void force_timer_tick();

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

void acquire(uint8_t * lock);
void release(uint8_t * lock);

void int_to_string(int num, char* buf, int dim);

#endif
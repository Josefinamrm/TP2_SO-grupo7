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

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

void _hlt(void);

#endif
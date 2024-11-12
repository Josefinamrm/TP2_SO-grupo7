// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <time.h>
#include <interrupts.h>
#include <lib.h>
#include <processManager.h>

#define TICKS_PER_SECOND 18    // Number of ticks per second (assuming a common setup)


static unsigned long ticks = 0;

void timer_handler()
{
	ticks++;
    remove_from_sleep_queue(ticks);
}

int ticks_elapsed()
{
	return ticks;
}

int seconds_elapsed()
{
	return ticks / 18;
}

void sleep_s(int seconds)
{
	uint64_t until_ticks = ticks_elapsed() + (seconds * 18);
    add_to_sleep_queue(my_getpid(), until_ticks);
}


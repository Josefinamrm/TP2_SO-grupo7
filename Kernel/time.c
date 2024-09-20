#include <time.h>
#include <interrupts.h>
#include <naiveConsole.h>
#include <lib.h>

#define TICKS_PER_SECOND 18    // Number of ticks per second (assuming a common setup)


static unsigned long ticks = 0;

void timer_handler()
{
	ticks++;
}

int ticks_elapsed()
{
	return ticks;
}

int seconds_elapsed()
{
	return ticks / 18;
}

void timer_wait(int seconds)
{
	int startTime = ticks_elapsed();
    while ((ticks_elapsed() - startTime) / TICKS_PER_SECOND < seconds)
    {
        _hlt();
    }
}

void timer_wait_ms(int ms)
{
	int startTime = ticks_elapsed();
    while ((ticks_elapsed() - startTime) * 1000 / TICKS_PER_SECOND < ms)
    {
        _hlt();
    }
}

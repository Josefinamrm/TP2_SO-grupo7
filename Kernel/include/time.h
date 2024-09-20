#ifndef _TIME_H_
#define _TIME_H_

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();



void timer_wait(int duration);
/* void get */
void timer_wait_ms(int ms);

#endif

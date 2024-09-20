/* Sacado de OSDEV : https://wiki.osdev.org/PC_Speaker#Through_the_Programmable_Interval_Timer_.28PIT.29 */

#include <stdint.h>
#include <sound.h>
#include <time.h>
#include <lib.h>
#include <videoDriver.h>

// Play sound using built-in speaker
void play_sound(uint32_t nFrequence)
{
    uint32_t Div;
    uint8_t tmp;

    // Set the PIT to the desired frequency
    Div = 1193180/nFrequence;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t)(Div));
    outb(0x42, (uint8_t)(Div >> 8));

    // And play the sound using the PC speaker
    tmp = inb(0x61);
    if (tmp != (tmp | 3))
    {
        outb(0x61, tmp | 3);
        // printArray("emitio sonido\n");
    }
}

// make it shut up
void nosound()
{
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
    // printArray("se calloo\n");
}

// Make a beep
void beep(uint32_t frec, int time_ms)
{
    play_sound(frec);
    timer_wait_ms(time_ms);
    nosound();
}
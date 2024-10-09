#include <clock.h>
#include <lib.h>

// Definición de constantes para las unidades de tiempo en el RTC

#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04

// Declaración de la función externa para obtener el tiempo desde el RTC
extern int getTime(int timeUnit);

// Función estática para reformatear el tiempo de BCD (Binary-Coded Decimal) a decimal
static unsigned int reformat(unsigned char time) {
    return (time >> 4) * 10 + (time & 0x0F); // Convertir BCD a decimal
}

// Función para obtener los segundos reformateados
unsigned int seconds() {
    return reformat(getTime(SECONDS));
}

// Función para obtener los minutos reformateados
unsigned int minutes() {
    return reformat(getTime(MINUTES));
}

// Función para obtener las horas reformateadas
// UTC 
unsigned int hours() {
    int hour = reformat(getTime(HOURS));  
    hour = (hour + 21) % 24; // Ajuste correcto para -3 horas

    return hour;
}

// Función para convertir la hora actual a una cadena de caracteres
void timeToStr(char* reserve) {
    reserve[2] = reserve[5] = ':';

    uint8_t s, m, h = hours();
    
    reserve[0] = (h/10) % 10 + '0';
    reserve[1] = h % 10 + '0';

    m = minutes();
    reserve[3] = (m/10) % 10 + '0';
    reserve[4] = m % 10 + '0';

    s = seconds();
    reserve[6] = (s/10) % 10 + '0';
    reserve[7] = s % 10 + '0';
    reserve[8] = '\n'; 
}

#include <user_syscalls.h>
#include <user_lib.h>


char getChar() {
    char v[]={0};
    usys_read(STDIN,v,1);
    return v[0];
}

char getLastChar() { 
    char v[] = {0};
    usys_read(LASTIN, v, 1); 
    return v[0];
}

void putChar(char c) {
    usys_write(STDOUT, &c, 1);
}

void printColor(int fontColor, char * string) {
    usys_write_color(fontColor, string, strlen(string));
}


void printError(char * string) {
    usys_write(STDERR, string, strlen(string)); 
}

void print(char * string) {
    usys_write(STDOUT, string, strlen(string)); 
}

int strlen(char *arr)
{
    int i = 0;
    while (arr[i++] != '\0')
        ;
    return i;
}

void strcpy(char *destination, const char *source)
{
    while (*source != '\0')
    {
        *destination++ = *source++;
    }
    *destination = '\0';
}

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    // Calculate characters for each digit
    do
    {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    // Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}

//función auxiliar
static void reverseString(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void intToString(int num, char* buf, int dim) {
    int index = 0;

    if (num == 0) {
        if (dim > 1) {
            buf[index++] = '0';
            buf[index] = '\0';
        }
        return;
    }

    while (num != 0 && index < dim - 1) {
        int digit = num % 10;
        buf[index++] = digit + '0';
        num /= 10;
    }

    buf[index] = '\0';
    reverseString(buf, index);
}

// Para el juego
void printScore(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, int num){
    char scoreBuf[13];
    intToString(num, scoreBuf, 13);
    usys_print_array_of_draw_size(fontColor, backgroundColor, x, y, scoreBuf);
}


int strcmp(const char *s1, const char *s2){
    while (*s1 == *s2++) {
        if (*s1++ == 0)
			return 0;
    }
	return (*s1 - *--s2);
}

int parseCommandArg(char * str) {
    int argC = 1;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            argC++; 
        } 
    }
    return argC;
}

void udrawFrame(uint32_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y, uint64_t thickness) {
	usys_print_rect(color, x, y, size_x, thickness); 
	usys_print_rect(color, x, y+size_y-thickness, size_x, thickness);
	usys_print_rect(color, x, y, thickness, size_y); 
	usys_print_rect(color, x + size_x - thickness, y, thickness, size_y);
}

void udrawArray(uint32_t fontColor, uint32_t backgroundColor, int x, int y, char * arr) {
    _draw_array(fontColor, backgroundColor, (uint64_t) x,(uint64_t) y, (uint64_t) arr);
}

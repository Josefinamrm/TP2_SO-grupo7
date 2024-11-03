// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <user_syscalls.h>
#include <user_lib.h>


char get_char() {
    char v[]={0};
    usys_read(STDIN,v,1);
    return v[0];
}

char get_last_char() { 
    char v[] = {0};
    usys_read(LASTIN, v, 1); 
    return v[0];
}

void put_char(char c) {
    usys_write(STDOUT, &c, 1);
}

void print_color(int fontColor, char * string) {
    usys_write_color(fontColor, string, strlen(string));
}


void print_error(char * string) {
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

uint32_t uint_to_base(uint64_t value, char *buffer, uint32_t base)
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
static void reverse_string(char* str, int length) {
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

void int_to_string(int num, char* buf, int dim) {
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
    reverse_string(buf, index);
}

// Para el juego
void print_score(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, int num){
    char scoreBuf[13];
    int_to_string(num, scoreBuf, 13);
    usys_print_array_of_draw_size(fontColor, backgroundColor, x, y, scoreBuf);
}


void print_dec(int num){
    char buf[256];
    int_to_string(num, buf, 256);
    print(buf);
}


int strcmp(const char *s1, const char *s2){
    while (*s1 == *s2++) {
        if (*s1++ == 0)
			return 0;
    }
	return (*s1 - *--s2);
}

int parse_command_arg(char * str, char * arguments[]) {
    int argC = 0;
    char *start = str;

    while (*str != '\0') {
        while(*start == ' ') { // ignore spaces
            start++;
        }
        if (*start == '\0') {
            break;
        }

        // calculate length of argument
        char *end = start;
        while (*end != ' ' && *end != '\0') {
            end++;
        }
        int len = end - start;

        // allocate memory for argument
        arguments[argC] = (char *) usys_malloc(len + 1);
        strcpy(arguments[argC], start);
        arguments[argC][len] = '\0'; // null-terminate

        // move to next argument
        argC++;
        start = end;
    }
    return argC;
}

// char * check_back(char * str, char * cmd) {
//     int i = 0;
    
//     while (str[i] != '\0' && str[i] != ' ') {
//         cmd[i] = str[i]; 
//         i++;
//     }

//     cmd[i] = '\0'; 

//     if (str[i] == ' ') {
//         return &str[i + 1];
//     }
    
//     return NULL;
// }


void udraw_frame(uint32_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y, uint64_t thickness) {
	usys_print_rect(color, x, y, size_x, thickness); 
	usys_print_rect(color, x, y+size_y-thickness, size_x, thickness);
	usys_print_rect(color, x, y, thickness, size_y); 
	usys_print_rect(color, x + size_x - thickness, y, thickness, size_y);
}

void udraw_array(uint32_t fontColor, uint32_t backgroundColor, int x, int y, char * arr) {
    _draw_array(fontColor, backgroundColor, (uint64_t) x,(uint64_t) y, (uint64_t) arr);
}

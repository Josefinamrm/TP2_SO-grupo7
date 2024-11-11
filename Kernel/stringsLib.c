#include <stringsLib.h>

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


// Compares 2 strings
int strcmp(const char *s1, const char *s2){
    while (*s1 == *s2++) {
        if (*s1++ == 0)
			return 0;
    }
	return (*s1 - *--s2);
}


int concat_str(char * dest, char * src){
    char* ptr = dest + stringlen(dest);
     while (*src != '\0') {
        *ptr++ = *src++;
    }
    *ptr = '\0';
    return 0;
}


int stringlen(const char *arr){
    const char *s = arr;
    while (*s != '\0') {
        s++;
    }
    return s - arr;
}


void stringcpy(char *destination, const char *source){
    while (*source != '\0'){
        *destination++ = *source++;
    }
    *destination = '\0';
}
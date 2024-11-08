#ifndef STRINGSLIB_H
#define STRINGSLIB_H

void int_to_string(int num, char* buf, int dim);

int strcmp(const char *s1, const char *s2);

int concat_str(char * dest, char * src);

int stringlen(const char *arr);

void stringcpy(char *destination, const char *source);
#endif 
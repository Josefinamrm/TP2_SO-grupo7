#ifndef STANDARD_TYPES_H
#define STANDARD_TYPES_H

#define FINISH_SUCCESFULLY 0
#define FINISH_ON_ERROR -1

typedef enum {STDIN = 0, STDOUT, STDERR, BACKGROUND, PIPE} Type;
typedef enum {READ = 0, WRITE} Permission;


#endif
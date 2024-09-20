#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include <stdint.h>
#include <colors.h>

// #define INITIAL_FONT_SIZE 1
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16
#define DEFAULT_FONT_SIZE 2
#define DEFAULT_DRAW_SIZE 3


// JUEGO
void drawArrayOfSize(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, char *arr);
void changeDrawSize(int new_draw_size);
void drawCharOfSize(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, char character);

// función general para escribir un píxel en pantalla
void drawPixel(uint32_t hexColor, uint64_t x, uint64_t y);

// función para copiar un pixel de un lugar a otro indicado
void copyPixel(uint64_t new_x, uint64_t new_y, uint64_t old_x, uint64_t old_y);

//función general para escribir un caracter en pantalla, en una posicion dada
void drawChar(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, char character);

// función general para escribir un array null terminated de caracteres, a partir de una posicion dada
void drawArray(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, char * arr) ;

// función para dibujar un rectangulo que arranca en las posiciones indicadas, con el tamaño de lado indicado, y el color indicado
void drawRect(uint32_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y);

// función para dibujar un cuadrado que arranca en las posiciones indicadas, con el tamaño de lado indicado, y el color indicado
void drawSquare(uint32_t color, uint64_t x, uint64_t y, uint64_t size);

// solo se permite entre 1, 2 y 3
void changeFontSize(int new_font_size);

// obtener el ancho de la pantalla
uint16_t getWidth();

// obtener el alto de la pantalla
uint16_t getHeight();

// obtener el tamaño de la fuente
int getFontSize();

// eliminar la tecla actual
void deleteChar();

// limpia la pantalla
void clearScreen();

// mover la pantalla (scroll)
void moveScreen();

// salto de línea
void newLine();

// dibujar el proximo caracter con color 
void putCharWithColor( uint32_t fontColor, uint32_t backgroundColor, char character);

// dibjuar el proximo caracter con color default
void putChar(char character);

// dibuja un array null terminated con color
void printArrayWithColor( uint32_t fontColor, uint32_t backgroundColor, char * arr);

// dibuja un array null terminated con colores default
void printArray(char * arr);

// dibuja un array de dimensión indicada con color
void printArrayOfDim(char * arr, uint64_t dim);

//dibuja un array de dimensión indicada con colores default
void printArrayOfDimWithColor( uint32_t fontColor, uint32_t backgroundColor, char * arr, uint64_t dim);

// función para transformar un uint64_t en la base indicada (sacada de naiveConsole.c)
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

// función para imprimir un decimal
void printDec(uint64_t value);

// función para imprimir un hexadecimal
void printHex(uint64_t value);

#endif
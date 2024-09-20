#ifndef ELIMINATOR_H
#define ELIMINATOR_H

#include <stdint.h>
#include <user_lib.h>
#include <ucolors.h>
#include <user_syscalls.h>


#define LEFT 0
#define RIGHT 1
#define DOWN 2
#define UP 3

#define TIMER 60000

#define INIT_HEIGHT 2
#define HEIGHT 190
#define INIT_WIDTH 2
#define WIDTH 254
#define SQUARE_WIDTH 190

#define MAX_WIDTH_PX 1024
#define MAX_HEIGHT_PX 768
#define MAX_SQUARE_WIDTH_PX 768

#define PLAYER_SIZE 4
#define BORDER_SIZE 8
#define P1_START_X ((SQUARE_WIDTH-INIT_WIDTH)/2 - 47)
#define P1_START_Y ((HEIGHT-INIT_HEIGHT)/2)
#define P2_START_X ((SQUARE_WIDTH-INIT_WIDTH)/2 + 47)
#define P2_START_Y ((HEIGHT-INIT_HEIGHT)/2)
#define P1_COLOR GREEN
#define P2_COLOR ORANGE
#define ESC 27

// Información del jugador
typedef struct
{
    int pos_x;
    int pos_y;
    uint32_t color;
    int current_direction;
    int pos_collision;
    int score;
} Player;


void eliminator();

void clearMatrix();

void init_eliminator();

void init_positions();

void restart_match();

void play_game();

void pauseGame();

void move_player(Player *player);

void drawStartingScreen();

void ControlScreen();

void drawEndGame();

void drawPlayerScreen();

void drawMainGameScreen();

void playAgainScreen();

void printScores();

void printPlayerScores();

void clear_playable_screen();

void print_time();

#endif
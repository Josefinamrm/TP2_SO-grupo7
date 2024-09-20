#include <eliminator.h>

static int middle_x = MAX_HEIGHT_PX / 2;

static int middle_y = MAX_HEIGHT_PX / 2;

static int matrix[HEIGHT - INIT_HEIGHT][SQUARE_WIDTH - INIT_WIDTH][2] = {{{0, 0}}};

static int directions[4][2] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};

static Player player1;
static Player player2;

static int player_count;
static int game_speed;
static int game_on = 0;
static int player1_timer = TIMER;
static int paused = 0;
static int exit = 0;

void eliminator()
{
    drawStartingScreen();
    if (!exit)
    {
        init_positions();
        init_eliminator();
    }
    usys_clear_screen();
    usys_change_draw_size(3);
    usys_print_array_of_draw_size(PINK, BLACK, middle_x - strlen("BYE!!") / 2, middle_y, "BYE!!");
    usys_wait(2000);
    clearMatrix();
    flush_buffer();
}

// inicializa el eliminator
void init_eliminator()
{
    game_on = 1;
    usys_wait(500);
    play_game();
}

// funcion auxiliar
void clearMatrix()
{
    for (int i = 0; i < HEIGHT - INIT_HEIGHT; i++)
    {
        for (int j = 0; j < SQUARE_WIDTH - INIT_WIDTH; j++)
        {
            matrix[i][j][0] = 0;
            matrix[i][j][1] = 0;
        }
    }
}

void init_positions()
{

    if (player_count == 2)
    {
        player2.pos_x = P2_START_X;
        player2.pos_y = P2_START_Y;
        player2.color = P2_COLOR;
        player2.current_direction = RIGHT;
        player2.pos_collision = 1;
        player2.score = 0;
        matrix[P2_START_X][P2_START_Y][1] = 1;
    }

    player1.pos_x = P1_START_X;
    player1.pos_y = P1_START_Y;
    player1.color = P1_COLOR;
    player1.current_direction = LEFT;
    player1.pos_collision = 0;
    player1.score = 0;
    matrix[P1_START_X][P1_START_Y][0] = 1;
}

// reestablece los valores de la partida
void restart_match()
{
    clear_playable_screen();
    clearMatrix();

    paused = 0;
    exit = 0;

    if (player_count == 2)
    {
        player2.pos_x = P2_START_X;
        player2.pos_y = P2_START_Y;
        player2.current_direction = RIGHT;
        matrix[P2_START_X][P2_START_Y][1] = 1;
    }

    player1.pos_x = P1_START_X;
    player1.pos_y = P1_START_Y;
    player1.current_direction = LEFT;
    matrix[P1_START_X][P1_START_Y][0] = 1;
    player1_timer = TIMER;
}

void play_game()
{
    while (game_on)
    {
        
        char c = getLastChar();

        switch (c)
        {
        case ESC:
            paused = 1;
            pauseGame();
            break;

        case 'w':
            if (player1.current_direction != UP && player1.current_direction != DOWN)
            {
                player1.current_direction = UP;
            }
            break;

        case 'a':
            if (player1.current_direction != LEFT && player1.current_direction != RIGHT)
            {
                player1.current_direction = LEFT;
            }
            break;

        case 's':
            if (player1.current_direction != DOWN && player1.current_direction != UP)
            {
                player1.current_direction = DOWN;
            }
            break;

        case 'd':
            if (player1.current_direction != RIGHT && player1.current_direction != LEFT)
            {
                player1.current_direction = RIGHT;
            }
            break;

        case 'i':
            if (player2.current_direction != UP && player2.current_direction != DOWN)
            {
                player2.current_direction = UP;
            }
            break;

        case 'j':
            if (player2.current_direction != LEFT && player2.current_direction != RIGHT)
            {
                player2.current_direction = LEFT;
            }
            break;

        case 'k':
            if (player2.current_direction != DOWN && player2.current_direction != UP)
            {
                player2.current_direction = DOWN;
            }
            break;

        case 'l':
            if (player2.current_direction != RIGHT && player2.current_direction != LEFT)
            {
                player2.current_direction = RIGHT;
            }
            break;

        default:
            break;
        }

        if (player_count == 2)
        {
            move_player(&player2);
        }
        if (player_count == 1)
        {
            if (player1_timer == 0)
            {
                player1.score += 1;
                printScores();
                clear_playable_screen();
                udrawArray(GREEN, BLACK, (SQUARE_WIDTH - INIT_WIDTH) / 2 - 47, middle_y + 120, "Restart en 3... ");
                usys_wait(1000);
                udrawArray(GREEN, BLACK, (SQUARE_WIDTH - INIT_WIDTH) / 2 - 47, middle_y + 120, "Restart en 3...2... ");
                usys_wait(1000);
                udrawArray(GREEN, BLACK, (SQUARE_WIDTH - INIT_WIDTH) / 2 - 47, middle_y + 120, "Restart en 3...2...1 ");
                usys_wait(1000);
                restart_match();
            }
            else
            {
                if ((player1_timer % 1000) == 0)
                {
                    print_time();
                }
                player1_timer -= 50;
            }
        }
        move_player(&player1);
        usys_wait(game_speed);
    }
}

void pauseGame()
{
    while (paused)
    {
        usys_change_draw_size(3);
        usys_print_array_of_draw_size(TURQUOISE, BLACK, 814, 210, "PAUSA");
        usys_change_draw_size(2);
        usys_print_array_of_draw_size(WHITE, BLACK, 790, 260, "Para Salir");
        usys_print_array_of_draw_size(WHITE, BLACK, 790, 290, "Del Juego");
        usys_print_array_of_draw_size(WHITE, BLACK, 790, 320, "Presione Q");

        usys_print_array_of_draw_size(WHITE, BLACK, 790, 370, "Para Seguir");
        usys_print_array_of_draw_size(WHITE, BLACK, 790, 400, "Presione [ESC]");
        usys_change_draw_size(3);
        udrawFrame(TURQUOISE, 0, 0, MAX_SQUARE_WIDTH_PX, MAX_HEIGHT_PX, BORDER_SIZE);
        udrawFrame(TURQUOISE, MAX_SQUARE_WIDTH_PX + 4, 0, (MAX_WIDTH_PX - MAX_SQUARE_WIDTH_PX) - 4, MAX_HEIGHT_PX, BORDER_SIZE);
        char c = getLastChar();
        if (c == ESC)
        {
            paused = 0;
        }
        else if (c == 'Q' || c == 'q')
        {
            game_on = 0;
            paused = 0;
        }
    }
    usys_print_rect(BLACK, 790, 210, 224, 1700);
    udrawFrame(RED, 0, 0, MAX_SQUARE_WIDTH_PX, MAX_HEIGHT_PX, BORDER_SIZE);
    udrawFrame(LIGHT_BLUE, MAX_SQUARE_WIDTH_PX + 4, 0, (MAX_WIDTH_PX - MAX_SQUARE_WIDTH_PX) - 4, MAX_HEIGHT_PX, BORDER_SIZE);
}

// Función auxiliar
int inside_board(int x, int y)
{
    return x >= INIT_WIDTH && x <= SQUARE_WIDTH && y >= INIT_HEIGHT && y <= HEIGHT;
}

void move_player(Player *player)
{
    int new_x = player->pos_x + directions[player->current_direction][0];
    int new_y = player->pos_y + directions[player->current_direction][1];

    if (inside_board(new_x, new_y) && !matrix[new_y][new_x][player1.pos_collision] && !matrix[new_y][new_x][player2.pos_collision])
    {
        player->pos_x = new_x;
        player->pos_y = new_y;
        matrix[new_y][new_x][player->pos_collision] = 1;
    }
    else
    {
        usys_beep(500, 500);
        if (player_count == 2)
        {
            // si la colisión fue del Player1 le sumo un punto al Player2
            if (player->pos_collision == 0)
            {
                player2.score += 1;
            }
            // si la colisión fue del Player2 le sumo un punto al Player1
            else if (player->pos_collision == 1)
            {
                player1.score += 1;
            }
            printPlayerScores();
        }

        clear_playable_screen();
        playAgainScreen();
        if (!exit)
        {
            udrawArray(GREEN, BLACK, (SQUARE_WIDTH - INIT_WIDTH) / 2 - 47, middle_y + 120, "Restart en 3... ");
            usys_wait(1000);
            udrawArray(GREEN, BLACK, (SQUARE_WIDTH - INIT_WIDTH) / 2 - 47, middle_y + 120, "Restart en 3...2... ");
            usys_wait(1000);
            udrawArray(GREEN, BLACK, (SQUARE_WIDTH - INIT_WIDTH) / 2 - 47, middle_y + 120, "Restart en 3...2...1 ");
            usys_wait(1000);

            restart_match();
        }

        return;
    }
    usys_print_square(player->color, player->pos_x * PLAYER_SIZE, player->pos_y * PLAYER_SIZE, PLAYER_SIZE);
}

void drawStartingScreen()
{
    int title_x = middle_x - (strlen("ELIMINATOR") / 2);
    int string_x = middle_x - (strlen("Presiona la tecla C para Comenzar") / 2);

    usys_clear_screen();
    udrawFrame(RED, 0, 0, MAX_WIDTH_PX, MAX_HEIGHT_PX, BORDER_SIZE); // outer frame
    usys_change_draw_size(3);
    usys_print_array_of_draw_size(RED, BLACK, title_x, middle_y - 100, "ELIMINATOR");
    usys_wait(1000);
    usys_change_draw_size(2);
    usys_print_array_of_draw_size(WHITE, RED, string_x - 120, middle_y, "Presione la tecla C para Comenzar");
    usys_print_array_of_draw_size(RED, BLACK, string_x - 130, middle_y + 200, "Presione [ESC] para salir del juego");
    char c;
    while ((c = getLastChar()) != 'c' && c != 'C')
    {
        if (c == ESC)
        {
            drawEndGame();
            return;
        }
    }
    drawPlayerScreen();
}

void ControlScreen()
{
    usys_clear_screen();
    udrawFrame(YELLOW, 0, 0, MAX_WIDTH_PX, MAX_HEIGHT_PX, BORDER_SIZE); // outer frame
    print("\n\n");
    usys_change_draw_size(3);
    usys_print_array_of_draw_size(YELLOW, BLACK, middle_x, middle_y - 250, "CONTROLES");
    usys_change_draw_size(2);
    if (player_count == 1)
    {
        usys_print_array_of_draw_size(WHITE, BLACK, middle_x - 120, middle_y - 100, "Jugador 1 solo: ASDW para moverse");
    }
    else
    {
        usys_print_array_of_draw_size(WHITE, BLACK, middle_x - 120, middle_y - 100, "Jugador 1: ASDW para moverse");
        usys_print_array_of_draw_size(WHITE, BLACK, middle_x - 120, middle_y - 20, "Jugador 2: LIJK para moverse");
    }
    usys_wait(3000);
}

void drawEndGame()
{
    usys_clear_screen();
    usys_change_draw_size(3);
    int end_x = middle_x - (strlen("SALIR DEL JUEGO")) / 2;
    usys_print_array_of_draw_size(WHITE, RED, end_x - 40, middle_y - 140, "SALIR DEL JUEGO");
    usys_change_draw_size(2);
    usys_print_array_of_draw_size(YELLOW, BLACK, end_x - 240, middle_y, "Desea salir del juego? Presione S (Si) o N (No)");
    while (1)
    {
        char c = getLastChar();
        if (c == 'S' || c == 's')
        {
            game_on = 0;
            exit = 1;
            return;
        }
        if (c == 'N' || c == 'n')
        {
            return;
        }
    }
}

void drawPlayerScreen()
{
    usys_clear_screen();
    usys_change_draw_size(3);
    udrawFrame(ORANGE, 0, 0, MAX_WIDTH_PX, MAX_HEIGHT_PX, BORDER_SIZE); // outer frame
    usys_print_array_of_draw_size(ORANGE, BLACK, middle_x + 30, middle_y - 300, "PLAYERS");
    usys_change_draw_size(2);
    usys_print_array_of_draw_size(WHITE, ORANGE, middle_x - 160, middle_y - 200, "Seleccione cantidad de jugadores (1 o 2):");
    usys_print_array_of_draw_size(WHITE, BLACK, middle_x - 100, middle_y - 120, "1 (jugador)     2 (jugadores)");
    char selection;
    while (1)
    {
        selection = getLastChar();
        if (selection == '1')
        {
            usys_print_array_of_draw_size(WHITE, BLACK, middle_x - 100, middle_y - 120, "1 (jugador)                  ");
            player_count = 1;
            break;
        }
        else if (selection == '2')
        {
            usys_print_array_of_draw_size(WHITE, BLACK, middle_x - 100, middle_y - 120, "                2 (jugadores)");
            player_count = 2;
            break;
        }
    }
    usys_print_array_of_draw_size(WHITE, ORANGE, middle_x - 40, middle_y - 40, "Seleccione nivel:");
    usys_change_draw_size(2);
    usys_print_array_of_draw_size(WHITE, BLACK, middle_x - 150, middle_y + 40, "1 (Facil), 2 (Medio), 3 (Dificil)");
    while (1)
    {
        selection = getLastChar();
        if (selection == '1')
        {
            usys_print_array_of_draw_size(WHITE, BLACK, middle_x - 150, middle_y + 40, "1 (Facil)                        ");
            game_speed = 60;
            break;
        }
        else if (selection == '2')
        {
            usys_print_array_of_draw_size(WHITE, BLACK, middle_x - 150, middle_y + 40, "           2 (Medio)             ");
            game_speed = 30;
            break;
        }
        else if (selection == '3')
        {
            usys_print_array_of_draw_size(WHITE, BLACK, middle_x - 150, middle_y + 40, "                      3 (Dificil)");
            game_speed = 5;
            break;
        }
    }
    usys_wait(1000);
    ControlScreen();
    usys_print_array_of_draw_size(GREEN, BLACK, middle_x - 150, middle_y + 120, "Empezando juego en 3... ");
    usys_wait(1000);
    usys_print_array_of_draw_size(GREEN, BLACK, middle_x - 150, middle_y + 120, "Empezando juego en 3...2...");
    usys_wait(1000);
    usys_print_array_of_draw_size(GREEN, BLACK, middle_x - 150, middle_y + 120, "Empezando juego en 3...2...1!");
    usys_wait(1000);
    drawMainGameScreen();
}

void drawMainGameScreen()
{
    usys_clear_screen();
    udrawFrame(RED, 0, 0, MAX_SQUARE_WIDTH_PX, MAX_HEIGHT_PX, BORDER_SIZE); // outer frame
    udrawFrame(LIGHT_BLUE, MAX_SQUARE_WIDTH_PX + 4, 0, (MAX_WIDTH_PX - MAX_SQUARE_WIDTH_PX) - 4, MAX_HEIGHT_PX, BORDER_SIZE);
    printScores();
}

void playAgainScreen()
{
    usys_change_draw_size(2);
    usys_print_array_of_draw_size(GREEN, BLACK, 100, middle_y, "Presione J para jugar de nuevo");
    usys_print_array_of_draw_size(RED, BLACK, 100, middle_y + 60, "Presione [ESC] para salir del juego");
    while (1)
    {
        char c = getLastChar();
        if (c == ESC)
        {
            drawEndGame();
            return;
        }
        else if (c == 'j' || c == 'J')
        {
            return;
        }
    }
}

void printScores()
{
    usys_change_draw_size(3);
    usys_print_array_of_draw_size(LIGHT_BLUE, BLACK, 814, 38, "PUNTAJE");
    usys_change_draw_size(2);
    usys_print_array_of_draw_size(GREEN, BLACK, 800, 106, "Player1 : ");
    if (player_count == 2)
    {
        usys_print_array_of_draw_size(ORANGE, BLACK, 800, 158, "Player2 : ");
    }
    printPlayerScores();
}

void printPlayerScores()
{
    printScore(GREEN, BLACK, 960, 106, player1.score);
    if (player_count == 2)
    {
        printScore(ORANGE, BLACK, 960, 158, player2.score);
    }
}

void clear_playable_screen()
{
    usys_print_square(BLACK, INIT_HEIGHT * PLAYER_SIZE, INIT_WIDTH * PLAYER_SIZE, (SQUARE_WIDTH - 2) * PLAYER_SIZE);
}

void print_time()
{
    char time_buf[3] = {0};
    intToString(player1_timer / 1000, time_buf, 3);
    usys_change_draw_size(2);
    usys_print_array_of_draw_size(BLACK, BLACK, 890, 158, "  ");
    usys_print_array_of_draw_size(YELLOW, BLACK, 890, 158, time_buf);
}
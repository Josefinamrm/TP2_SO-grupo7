#include "videoDriver.h"
#include "time.h"
#include "dory.h"

#define MAX_WIDTH_PX 1024
#define MAX_HEIGHT_PX 768

static int middle_x = MAX_HEIGHT_PX / 2;
static int middle_y = MAX_HEIGHT_PX / 2;

void dory_start(){

    drawArray(TURQUOISE, BLACK, middle_x - 82, middle_y - 12, "DORY OS");
    timer_wait(3);
    clearScreen();
}
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboardDriver.h>

/* Disposición del teclado en inglés (US) */
static char scanCodeTable[] =
    {
        0, ESC_KEY, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t',
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        LCTRL_KEY,
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '|',
        LSHIFT_KEY, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
        0 /*RShift*/, '*', 0 /*LAlt*/, ' ' /*Space bar*/, 0 /*Caps Lock*/,
        0 /*F1*/, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*All Fs*/
        0 /*NumLock*/, 0, 0, 0, 0, '-',
        0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*All others undefined*/
};

/* Disposición del teclado en inglés (US) con shift */
static char shiftScanCodeTable[] =
    {
        0, ESC_KEY, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
        '\t',
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
        LCTRL_KEY /* LCtrl */,
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '|',
        LSHIFT_KEY, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
        0 /* RShift */, '*', 0 /* LAlt */, ' ' /* Space bar */, 0 /* Caps Lock */,
        0 /* F1 */, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* All Fs */
        0 /* NumLock */, 0, 0, 0, 0, '-',
        0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* All others undefined */
};

static int shiftFlag = 0;
static int ctrlFlag = 0;
static int capsFlag = 0;

int writeIndex = 0, readIndex = 0;

// Buffer circular para almacenar caracteres
char buffer[BUFFER_SIZE] = {0};

// Semáforos
char * empty_slots = "keyboard_empty_slots";
char * filled_slots = "keyboard_filled_slots";     


void initialize_keyboard_driver(){
    my_sem_open(empty_slots, BUFFER_SIZE);
    my_sem_open(filled_slots, 0);
}



/* Función para agregar un carácter al buffer */
static void addToBuffer(char c)
{
    if (writeIndex >= BUFFER_SIZE){
        writeIndex = 0;
    }

    my_sem_wait(empty_slots);
    buffer[writeIndex++] = c;
    my_sem_post(filled_slots);

}

/* Función para obtener un carácter del buffer */
char get_char_from_buffer()
{
    if (readIndex >= BUFFER_SIZE){
        readIndex = 0;
    }

    char to_ret;
    my_sem_wait(filled_slots);
    to_ret = buffer[readIndex++];
    my_sem_post(empty_slots);
    return to_ret;
    /* if (readIndex == writeIndex)
        return 0;
    if (readIndex >= BUFFER_SIZE)
        readIndex = 0;
    return (buffer[readIndex++]); */
}

/* Función para obtener el último carácter ingresado en el buffer --> para el juego */
char get_last_char_from_buffer()
{
    if (writeIndex == 0)
    {
        return 0;
    }
    return buffer[--writeIndex];
}

// Uses a circular array
void keyboard_handler()
{

    unsigned char scanCode = getScanCode();

    if ((scanCode == LSHIFT_KEY) && !shiftFlag)
    {
        shiftFlag = 1;
    }
    else if (scanCode == LSHIFT_RELEASE_KEY)
    { 
        shiftFlag = 0;
    }
    else if (scanCode == LCTRL_KEY)
    {
        ctrlFlag = 1;
    }
    else if (scanCode == LCTRL_RELEASE_KEY)
    {
        ctrlFlag = 0;
    }
    else if (scanCode == CAPS_LOCK)
    {
        capsFlag = !capsFlag;
    }
    else if (scanCode < 90 && scanCodeTable[scanCode] != 0)
    {
        if (scanCodeTable[scanCode] >= 'a' && scanCodeTable[scanCode] <= 'z')
        { // si es una letra
            if (shiftFlag ^ capsFlag)
            { // mayuscula
                addToBuffer(shiftScanCodeTable[scanCode]);
            }
            if(ctrlFlag && scanCodeTable[scanCode] == 'c')
            {
                printArray("^C");

                if(my_getpid() > 2){
                    my_exit_foreground();
                }else{
                    my_exit();
                }
                return;
            }
            if (ctrlFlag && scanCodeTable[scanCode]=='d')
            {
                addToBuffer(EOF);
                return;
            } 
            else
            {
                addToBuffer(scanCodeTable[scanCode]);
            }
        }
        else
        { // no es cero ni una letra
            if (shiftFlag)
            {
                addToBuffer(shiftScanCodeTable[scanCode]);
            }
            else
            {
                addToBuffer(scanCodeTable[scanCode]);
            }
        }
    }
}

/* Función para vaciar el buffer */
void flushBuffer()
{
    writeIndex = 0;
    readIndex = 0;
}
#include <keyboardDriver.h>

/* Disposición del teclado en inglés (US) */
static char scanCodeTable[] =
    {
        0, ESC_KEY, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t',
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0 /*LCtrl*/,
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
        0 /* LCtrl */,
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '|',
        LSHIFT_KEY, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
        0 /* RShift */, '*', 0 /* LAlt */, ' ' /* Space bar */, 0 /* Caps Lock */,
        0 /* F1 */, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* All Fs */
        0 /* NumLock */, 0, 0, 0, 0, '-',
        0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* All others undefined */
};

static int shiftFlag = 0;
static int capsFlag = 0;

int writeIndex = 0, readIndex = 0;

// Buffer circular para almacenar caracteres
char buffer[BUFFER_SIZE] = {0};

/* Función para agregar un carácter al buffer */
static void addToBuffer(char c)
{
    if (writeIndex >= BUFFER_SIZE)
    {
        writeIndex = 0;
    }
    buffer[writeIndex++] = c;
}

/* Función para obtener un carácter del buffer */
char getChar()
{
    if (readIndex == writeIndex)
        return 0;
    if (readIndex >= BUFFER_SIZE)
        readIndex = 0;
    return (buffer[readIndex++]);
}

/* Función para obtener el último carácter ingresado en el buffer --> para el juego */
char getLastChar()
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
    else if (scanCode == CAPS_LOCK)
    {
        capsFlag = !capsFlag;
    }
    else if (scanCode >= 0 && scanCode < 128 && scanCodeTable[scanCode] != 0)
    {
        if (scanCodeTable[scanCode] >= 'a' && scanCodeTable[scanCode] <= 'z')
        { // si es una letra
            if ((shiftFlag && !capsFlag) || (!shiftFlag && capsFlag))
            { // mayuscula
                addToBuffer(shiftScanCodeTable[scanCode]);
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
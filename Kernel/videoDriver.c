/* videoDriver */
#include <videoDriver.h>
#include <bitmapFont.h>

static int cursor_x = 0;
static int cursor_y = 0;

static int font_size = DEFAULT_FONT_SIZE;
static int draw_size = DEFAULT_DRAW_SIZE;


struct vbe_mode_info_structure
{
	uint16_t attributes;  // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;	  // deprecated
	uint8_t window_b;	  // deprecated
	uint16_t granularity; // deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr; // deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;		   // number of bytes per horizontal line
	uint16_t width;		   // width in pixels
	uint16_t height;	   // height in pixels
	uint8_t w_char;		   // unused...
	uint8_t y_char;		   // ...
	uint8_t planes;
	uint8_t bpp;   // bits per pixel in this mode
	uint8_t banks; // deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size; // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;

	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer; // physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size; // size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__((packed));

typedef struct vbe_mode_info_structure *VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr)0x0000000000005C00;

void drawPixel(uint32_t hexColor, uint64_t x, uint64_t y)
{
	if (x >= 0 && x < VBE_mode_info->width && y >= 0 && y < VBE_mode_info->height)
	{
		uint8_t *framebuffer = (uint8_t *)VBE_mode_info->framebuffer;
		uint64_t offset = (x * ((VBE_mode_info->bpp) / 8)) + (y * VBE_mode_info->pitch);
		framebuffer[offset] = (hexColor) & 0xFF;
		framebuffer[offset + 1] = (hexColor >> 8) & 0xFF;
		framebuffer[offset + 2] = (hexColor >> 16) & 0xFF;
	}
}

void copyPixel(uint64_t new_x, uint64_t new_y, uint64_t old_x, uint64_t old_y)
{
	if (new_x >= 0 && new_x < VBE_mode_info->width && new_y >= 0 && new_y < VBE_mode_info->height)
	{
		uint8_t *framebuffer = (uint8_t *)VBE_mode_info->framebuffer;
		uint64_t oldOffset = (old_x * ((VBE_mode_info->bpp) / 8)) + (old_y * VBE_mode_info->pitch);
		uint64_t offset = (new_x * ((VBE_mode_info->bpp) / 8)) + (new_y * VBE_mode_info->pitch);
		framebuffer[offset] = framebuffer[oldOffset];
		framebuffer[offset + 1] = framebuffer[oldOffset + 1];
		framebuffer[offset + 2] = framebuffer[oldOffset + 2];
	}
}

// para el juego, cambia el tamaño del dibujo
void changeDrawSize(int new_draw_size){
	draw_size = new_draw_size;
}

// Para el juego, dibuja un char según el tamaño de draw_size
void drawCharOfSize(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, char character){
	int aux_font_size = font_size;
	font_size = draw_size;
	drawChar(fontColor,backgroundColor,x,y,character);
	font_size = aux_font_size;
}

void drawChar(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, char character)
{
	uint8_t *charGlyphAddress = getGlyph(character);
	// posiciones donde debo dibujar
	int pos_x = x;
	int pos_y = y;
	char isLetter;

	// con i j me muevo dentro del bitmap
	for (int i = 0; i < (CHAR_HEIGHT * font_size); i++, pos_y++)
	{
		for (int j = 0; j < (CHAR_WIDTH * font_size); j++, pos_x++)
		{
			// chequeo si el bit que dibujo es 1 (foreground) o 0 (background),
			isLetter = (1 << (CHAR_WIDTH - (j / font_size) - 1)) & charGlyphAddress[i / font_size];

			if (isLetter)
			{
				drawPixel(fontColor, pos_x, pos_y);
			}
			else
			{
				drawPixel(backgroundColor, pos_x, pos_y);
			}
		}
		// resetteo la posicion en x para pasar a la siguiente línea de píxeles de y
		pos_x = x;
	}
}

void drawArray(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, char *arr)
{
	uint64_t pos_x = x;
	uint64_t pos_y = y;
	for (int i = 0; arr[i] != '\0'; i++)
	{
		if (pos_x >= VBE_mode_info->width)
		{
			pos_x = 0;
			pos_y += (CHAR_HEIGHT * font_size);
		}
		drawChar(fontColor, backgroundColor, pos_x, pos_y, arr[i]);
		pos_x += (CHAR_WIDTH * font_size);
	}
}

// para el juego, dibuja un array del tamaño indicato por draw_size
void drawArrayOfSize(uint32_t fontColor, uint32_t backgroundColor, uint64_t x, uint64_t y, char *arr)
{
	uint64_t pos_x = x;
	uint64_t pos_y = y;
	for (int i = 0; arr[i] != '\0'; i++)
	{
		if (pos_x >= VBE_mode_info->width)
		{
			pos_x = 0;
			pos_y += (CHAR_HEIGHT * draw_size);
		}
		drawCharOfSize(fontColor, backgroundColor, pos_x, pos_y, arr[i]);
		pos_x += (CHAR_WIDTH * draw_size);
	}
}


void drawRect(uint32_t color, uint64_t x, uint64_t y, uint64_t size_x, uint64_t size_y)
{
	for (int i = 0; i < size_y; i++)
	{
		for (int j = 0; j < size_x; j++)
		{
			drawPixel(color, x + j, y + i);
		}
	}
}

void drawSquare(uint32_t color, uint64_t x, uint64_t y, uint64_t size)
{
	drawRect(color, x, y, size, size);
}

void changeFontSize(int new_font_size)
{
	if (font_size <= 3 && font_size >= 1)
		font_size = new_font_size;
}

uint16_t getWidth()
{
	return VBE_mode_info->width;
}

uint16_t getHeight()
{
	return VBE_mode_info->height;
}

int getFontSize()
{
	return font_size;
}

void deleteChar()
{

	// muevo el cursor a la posición anterior, donde está el caracter a sobreescribir o borrar
	if (cursor_x == 0)
	{
		if (cursor_y == 0)
			return;
		cursor_y -= (CHAR_HEIGHT * font_size);
		cursor_x = VBE_mode_info->width - (CHAR_WIDTH * font_size);
	}
	else
	{
		cursor_x -= (CHAR_WIDTH * font_size);
	}

	// sobreescribo el caracter dibujado con un espacio
	drawChar(BLACK, BLACK, cursor_x, cursor_y, ' ');
}

void clearScreen()
{

	// vuelvo a pintar la pantalla de negro
	for (int y = 0; y < VBE_mode_info->height; y++)
	{
		for (int x = 0; x < VBE_mode_info->width; x++)
		{
			drawPixel(BLACK, x, y);
		}
	}

	// resetteo los cursores
	cursor_x = 0;
	cursor_y = 0;
}

// función auxiliar para limpiar la última línea de la pantalla
static void clearLine()
{
	for (int x = 0; x < getWidth(); x++)
	{
		drawChar(BLACK, BLACK, x, VBE_mode_info->height - font_size * CHAR_HEIGHT, ' ');
	}

	cursor_y -= font_size * CHAR_HEIGHT;
}

// Comenzando en la segunda linea, copio la current a la anterior
void moveScreen()
{
	int y = font_size * CHAR_HEIGHT;

	while (y < VBE_mode_info->height)
	{
		for (int j = 0; j < font_size * CHAR_HEIGHT; j++)
		{
			for (int i = 0; i < VBE_mode_info->width; i++)
			{
				copyPixel(i, y - (font_size * CHAR_HEIGHT) + j , i , y + j );
			}
		}

		y += font_size * CHAR_HEIGHT;
	}

	clearLine();
}

void newLine()
{
	cursor_x = 0;
	cursor_y += (CHAR_HEIGHT * font_size);
}

void putCharWithColor(uint32_t fontColor, uint32_t backgroundColor, char character)
{

	if (character == '\b')
	{
		deleteChar();
	}

	else if (character == '\n')
	{
		newLine();
	}

	else
	{
		if (cursor_x >= (VBE_mode_info->width))
		{
			cursor_x = 0;
			cursor_y += (CHAR_HEIGHT * font_size);
		}
		if (cursor_y >= (VBE_mode_info->height))
		{
			moveScreen();
		}
		drawChar(fontColor, backgroundColor, cursor_x, cursor_y, character);
		cursor_x += (CHAR_WIDTH * font_size);
	}
}

void putChar(char character)
{
	putCharWithColor(WHITE, BLACK, character);
}

void printArrayWithColor(uint32_t fontColor, uint32_t backgroundColor, char *arr)
{
	for (int i = 0; arr[i] != '\0'; i++)
	{
		putCharWithColor(fontColor, backgroundColor, arr[i]);
	}
}

void printArray(char *arr)
{
	printArrayWithColor(WHITE, BLACK, arr);
}

void printArrayOfDim(char *arr, uint64_t dim)
{
	for (int i = 0; i < dim && arr[i] != '\0'; i++)
	{
		putChar(arr[i]);
	}
}

void printArrayOfDimWithColor(uint32_t fontColor, uint32_t backgroundColor, char *arr, uint64_t dim)
{
	for (int i = 0; i < dim && arr[i] != '\0'; i++)
	{
		putCharWithColor(fontColor, backgroundColor, arr[i]);
	}
}

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
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

// función auxiliar para imprimir un número según la base
static void printBase(uint64_t value, uint32_t base)
{
	char buff[20];
	uintToBase(value, buff, base);
	printArray(buff);
}

void printDec(uint64_t value)
{
	printBase(value, 10);
}

void printHex(uint64_t value)
{
	printBase(value, 16);
}

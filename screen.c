#include "screen.h"


uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

uint16_t make_vgaentry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

size_t strlen(const char* str)
{
	size_t ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}

static void scroll()
{
	uint8_t attributeByte = ( COLOR_BLACK << 4 ) | ( COLOR_WHITE & 0x0F );
	uint16_t blank = 0x20 | (attributeByte << 8);

	if (terminal_column > VGA_HEIGHT)
	{
		int i;
		for(i = 0; i < VGA_WIDTH*VGA_HEIGHT; i++)
		{
			terminal_buffer[i] = terminal_buffer[i+VGA_HEIGHT];
		}

		for(i = VGA_WIDTH*VGA_HEIGHT; i < (VGA_WIDTH+1)*VGA_HEIGHT; i++)
		{
			terminal_buffer[i] = blank;
		}

		terminal_row = VGA_HEIGHT;
	}
}

static void move_cursor()
{
	uint16_t cursorLocation = terminal_row * VGA_WIDTH + terminal_column;
	outb(0x3D4, 14);
	outb(0x3D5, cursorLocation >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, cursorLocation);
}

void terminal_initialize()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_putchar(char c)
{
	if ( c == '\n' )
	{
		terminal_column = 0;
		if ( ++terminal_row == VGA_HEIGHT )
		{
			terminal_row = 0;
		}
	}
	else
	{	
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if ( ++terminal_column == VGA_WIDTH )
		{
			terminal_column = 0;
			if ( ++terminal_row == VGA_HEIGHT )
			{
				terminal_row = 0;
			}
		}
	}

	scroll();
	move_cursor();
}

void terminal_writestring(const char* data)
{
	size_t datalen = strlen(data);
	for ( size_t i = 0; i < datalen; i++)
		terminal_putchar(data[i]);
}

void terminal_writedec(uint32_t num)
{
	if(num==0) {
		terminal_putchar('0');
		return;
	}

	int32_t acc = num;
	char c[32];
	int i = 0;
	while(acc > 0)
	{
		c[i] = '0' + acc%10;
		acc /= 10;
		i++;
	}

	c[i] = 0;

	char c2[32];
	c2[i--] = 0;
	int j = 0;
	while(i >= 0)
	{
		c2[i--] = c[j++];
	}

	terminal_writestring(c2);
}

void terminal_writehex(uint32_t n)
{
	int32_t tmp;
	
	terminal_writestring("0x");
	
	char noZeroes = 1;
	
	int i;
	for(i=28; i > 0; i -= 4)
	{
		tmp = (n >> i) & 0xF;
		if(tmp == 0 && noZeroes != 0)
		{
			continue;
		}
		
		if(tmp > 0xA)
		{
			noZeroes = 0;
			terminal_putchar(tmp-0xA+'a');
		}
		else
		{
			noZeroes = 0;
			terminal_putchar(tmp+'0');
		}
	}
}

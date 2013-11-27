#include "common.h"


void outb(uint16_t port, uint8_t value)
{
	asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8_t inb(uint16_t port)
{
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

uint16_t inw(uint16_t port)
{
	uint16_t ret;
	asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

void memset(uint8_t *dest, uint8_t val, uint32_t len)
{
	uint8_t *temp = (uint8_t*) dest;
	for(; len != 0; len--) *temp++ = val;
}

extern void panic(const char* message, const char* file, uint32_t line)
{
	// We hit an error
	asm volatile("cli"); // Disable interrupts
	
	terminal_writestring("PANIC(");
	terminal_writestring(message);
	terminal_writestring(") at");
	terminal_writestring(file);
	terminal_writestring(": ");
	terminal_writedec(line);
	terminal_writestring("\n");
	
	for(;;); // Halt, using an infinite loop
}

extern void panic_assert(const char *file, uint32_t line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    terminal_writestring("ASSERTION-FAILED(");
    terminal_writestring(desc);
    terminal_writestring(") at ");
    terminal_writestring(file);
    terminal_writestring(":");
    terminal_writestring(line);
    terminal_writestring("\n");
    // Halt by going into an infinite loop.
    for(;;);
}

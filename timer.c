#include "timer.h"
#include "isr.h"
#include "screen.h"

uint32_t tick = 0;

static void timer_callback(registers_t regs)
{
	tick++;
	terminal_writestring("Tick: ");
	terminal_writedec(tick);
	terminal_writestring("\n");
}

void init_timer(uint32_t frequency)
{
	register_interrupt_handler(IRQ0, &timer_callback);

	// Devide max speed. Must be 16 bits max.
	uint32_t divisor = 1193180 / frequency;

	// Command byte
	outb(0x43, 0x36);

	uint8_t l = (uint8_t)(divisor & 0xFF);
	uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

	outb(0x40, l);
	outb(0x40, h);
}

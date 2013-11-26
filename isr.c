#include "isr.h"

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler)
{
	interrupt_handlers[n] = handler;
}

void isr_handler(registers_t regs)
{
	terminal_writestring("Received interrupt: ");
	terminal_writedec(regs.int_no);
	terminal_writestring("\n");
}

void irq_handler(resgisters_t regs)
{
	if(regs.int_no >= 40)
	{
		// Send reset signal to slave
		outb(0xA0, 0x20);
	}

	// Send reset signal to master
	outb(0x20, 0x20);

	if(interrupt_handlers[regs.int_no] != 0)
	{
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}

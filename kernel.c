
#include "common.h"
#include "screen.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"

/* Check if the compiler thinks we are targeting the wrong OS  */
#if defined(__linux__)
#error "You are not using a cross compiler. You will run into trouble."
#endif

/* Hardware text mode color constants */


#if defined(__cplusplus)
extern "C"
#endif

void kernel_main()
{
	init_descriptor_tables();
	init_timer(50);
	terminal_initialize();

	initialize_paging();
	terminal_writestring("Hello, kernel World!\n");

	uint32_t *ptr = (uint32_t)0xA0000000;
	uint32_t page_fault = *ptr;
}

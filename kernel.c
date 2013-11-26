
#include "common.h"
#include "screen.h"
#include "descriptor_tables.h"
#include "timer.h"

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
	terminal_initialize();
	/* Currently no support for newlines. \n will do something else. */
	terminal_writestring("Hello, kernel World!\n");

	init_timer(50);
}

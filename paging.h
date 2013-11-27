#ifndef PAGING_H
#define PAGING_H

#include "common.h"
#include "isr.h"

typedef struct page
{
	uint32_t present	: 1; // Page present in memory
	uint32_t rw			: 1; // Read-only if clear, read-write if set
	uint32_t user		: 1; // User-mode if set, Kernel if clear
	uint32_t accessed	: 1; // Has the page been accessed since the last refresh
	uint32_t dirty		: 1; // Has the page been written since the last refresh
	uint32_t unused		: 7; // Unused bits
	uint32_t frame		: 20;// Frame address (Shifted right 12 bits)
} page_t;

typedef struct page_table
{
	page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
	// Array of pointers to tables
	page_table_t *tables[1024];
	uint32_t tablesPhysical[1024];
	
	uint32_t physicalAddress; // of the tablesPhysical array
} page_directory_t;

void initialize_paging();
page_t *get_page(uint32_t address, int make, page_directory_t *dir);

void page_fault(registers_t regs);


#endif
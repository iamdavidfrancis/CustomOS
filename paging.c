#include "paging.h"
#include "kheap.h"
#include "screen.h"

page_directory_t *kernel_directory = 0;
page_directory_t *current_directory = 0;

uint32_t *frames;
uint32_t nframes;

extern uint32_t placement_address;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void set_frame(uint32_t frame_addr)
{
	uint32_t frame = frame_addr/0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

static void clear_frame(uint32_t frame_addr)
{
	uint32_t frame = frame_addr/0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}

static uint32_t test_frame(uint32_t frame_addr)
{
	u32int frame = frame_addr/0x1000;
    u32int idx = INDEX_FROM_BIT(frame);
    u32int off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (0x1 << off));
}

static uint32_t first_frame()
{
	uint32_t i, j;
	for(i=0; i < INDEX_FROM_BIT(nframes);i++)
	{
		if(frames[i] != 0xFFFFFFFF)
		{
			for(j=0; j < 32; j++)
			{
				uint32_t toTest = 0x1 << j;
				if(!(frames[i]&toTest))
					return i*4*8+j;
			}
		}
	}
}

void alloc_frame(page_t *page, int isKernel, int isWriteable)
{
	if(page->frame != 0)
	{
		return;
	}
	else 
	{
		uint32_t idx = first_frame();
		if(idx == (uint32_t)-1)
		{
			// PANIC! no free frames
		}
		set_frame(idx*0x1000);
		page->present = 1;
		page->rw = (isWriteable)?1:0;
		page->user = (isUser)?0:1;
		page->frame = idx;
	}
}

void free_frame(page_t *page)
{
    u32int frame;
    if (!(frame=page->frame))
    {
        return;
    }
    else
    {
        clear_frame(frame);
        page->frame = 0x0;
    }
}

void initialize_paging()
{
	// The size of physical memory
	// Currently 16MB
	uint32_t mem_end_page = 0x1000000;
	
	nframes = mem_end_page/0x1000;
	frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
	memset(frames, 0, INDEX_FROM_BIT(nframes));
	
	kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
	current_directory = kernel_directory;
	
	int i = 0;
	while(i < placement_address)
	{
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
	}
	
	register_interrupt_handler(14, page_fault);
	
	switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t *dir)
{
	current_directory = dir;
	asm volatile("mov %0, %%cr3":: "r" (&dir->tablesPhysical));
	
	uint32_t cr0;
	asm volatile("mov %%cr0, %0": "=r" (cr0));
	
	cr0 |= 0x80000000;
	asm volatile("mov %0,%%cr0"::"r" (cr0));
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir)
{
	// Turn the address into an index.
    address /= 0x1000;
    // Find the page table containing this address.
    u32int table_idx = address / 1024;
    if (dir->tables[table_idx]) // If this table is already assigned
    {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else if(make)
    {
        u32int tmp;
        dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else
    {
        return 0;
    }
}

void page_fault(registers_t regs)
{
	uint32_t faulting_address;
	asm volatile("mov %%cr2,%0" : "=r" (faulting_address));
	
	int present = !(regs.err_code & 0x1);
	int rw = regs.err_code & 0x2;
	int us = regs.err_code & 0x4;
	int reserved = regs.err_code & 0x8;
	int id = regs.err_code & 0x10;
	
	terminal_writestring("Page fault! (");
	if(present) { terminal_writestring("present "); }
	if(rw) { terminal_writestring("read-only "); }
	if(us) { terminal_writestring("user-mode "); }
	if(reserved) { terminal_writestring("reserved "); }
	terminal_writestring(") at ");
	terminal_writehex(faulting_address);
	terminal_writestring("\n");
	PANIC("Page fault");
}
// Sets up Global Descriptor Tables


struct gdt_entry_struct
{
	uint16_t limit_low;    // Lower 16 bits of limit.
	uint16_t base_low;     // Lower 16 bits of base.
	uint8_t base_middle;   // Next 8 bits of base.
	uint8_t access;        // Access flags.
	uint8_t granularity;
	uint8_t base_high;     // Last 8 bits of base.
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

struct gdt_ptr_struct
{
	uint16_t limit;   // Upper 16 bits of selector limits.
	uint32_t base;    // The address of the first gdt_entry_t struct.
} __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;


void init_descriptor_tables(); // Initialize GDT


// Sets up the Interrupt Descriptor Tables

struct idt_entry_struct
{
	uint16_t base_lo;	// The lower 16 bits of the address to jump.
	uint16_t sel;		// Kernel segment selector.
	uint8_t  always0;	// This flag must always be 0.
	uint8_t  flags;		// More flags. See documentation.
	uint16_t base_hi;	// The upper 16 bits of the address to jump.
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct
{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

// Extern directives let us access the addresses of ASM ISR handlers
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

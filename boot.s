# Declare constants used for creating a multiboot header.
.set ALIGN,	1<<0 			# align loaded modules on page boundaries
.set MEMINFO,	1<<1			# provide memory map
.set FLAGS,	ALIGN | MEMINFO		# this is the Multiboot 'flag' field
.set MAGIC,	0x1BADB002		#'magic number' lets bootloader find the header
.set CHECKSUM,	-(MAGIC + FLAGS)	# checksum of above, to prove we are multiboot

# Declare header as defined in Multiboot Standard. 
# You don't need to understand this, it's defined for us.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# We need to set the stack pointer register to prevent harm. We will
# make a small stack and allocate 16 KiB to it.
.section .bootstrap_stack
stack_bottom:
.skip 16384
stack_top:

# The linker specifies _start as the entry point to the kernel. The
# bootloader will jump here once the kernel has been loaded.
.section .text
.global _start
.type _start, @function
_start:
	# The kernel is now set up. Because we want to program in
	# something other than Assembly, we need to set up a few
	# things so we can program in C.

	# First off, we set the stack by moving our esp register
	# to point to the top of our stack. (It grows downward)
	movl $stack_top, %esp

	# Now we are ready to execute C code. We can't embed C
 	# code here, so we'll make an entry point for it called
	# kernel_main and call it here.
	call kernel_main

	# In case the function returns, we want to put the
	# computer into an infinite loop. To do that, we use
	# the clear interrupt ('cli') instruction to disable
	# interrupts, the halt ('hlt') instruction to stop the
	# CPU until the next interrupt arrives, and jumping to
	# the halt instruction if it ever continues execution.
	cli
	hlt
.Lhang:
	jmp .Lhang

# Set the size of the _start symbol to the current location '.'
# minus its start. Useful for debugging or call tracing.
.size _start, . - _start

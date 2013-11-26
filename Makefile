CC=i586-elf-gcc
AC=nasm
LC=i586-elf-gcc
LFLAGS=-ffreestanding -O2 -nostdlib
DEPS = common.h screen.h descriptor_tables.h isr.h
OBJ= common.o screen.o descriptor_tables.o kernel.o isr.o interrupt.s.o gdt.s.o boot.s.o
LINKER= linker.ld
BIN= myos.bin
ISODIR=isodir
ISO=myos.iso

all:  $(OBJ) link build

%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	
%.s.o: %.s
	$(AC) -felf $< -o $@

link: 
	$(LC) -T $(LINKER) -o $(BIN) $(LFLAGS) $(OBJ) -lgcc
	
build:
	mkdir -p $(ISODIR)
	mkdir -p $(ISODIR)/boot
	cp $(BIN) $(ISODIR)/boot/$(BIN)
	mkdir -p $(ISODIR)/boot/grub
	cp grub.cfg $(ISODIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISODIR)
	

clean:
	-rm *.o
	-rm $(BIN)
	-rm $(ISO)
	-rm -r $(ISODIR)

run:
	qemu-system-i386 -d cpu_reset -cdrom $(ISO)

run-int:
	qemu-system-i386 -d int -cdrom $(ISO)

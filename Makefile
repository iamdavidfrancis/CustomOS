CC=i586-elf-gcc
AC=i586-elf-as
LC=i586-elf-gcc
LFLAGS=-ffreestanding -O2 -nostdlib
DEPS = common.h screen.h
OBJ= common.o screen.o kernel.o boot.s.o 
LINKER= linker.ld
BIN= myos.bin
ISODIR=isodir
ISO=myos.iso

all:  $(OBJ) link build

%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	
%.s.o: %.s
	$(AC) $< -o $@

link: 
	$(LC) -T $(LINKER) -o $(BIN) $(LFLAGS) $(OBJ) -lgcc
	
build:
	cp $(BIN) $(ISODIR)/boot/$(BIN)
	grub-mkrescue -o $(ISO) $(ISODIR)

clean:
	-rm *.o
	-rm $(BIN)
	-rm $(ISO)

run:
	qemu-system-i386 -cdrom $(ISO)

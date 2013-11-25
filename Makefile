CC=i586-elf-gcc
AC=i586-elf-as
LC=i586-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
LFLAGS=-ffreestanding -O2 -nostdlib
DEPS = common.h screen.h
OBJ= common.o kernel.o screen.o boot.s.o 
LINKER= linker.ld
BIN= myos.bin

all:  $(OBJ) link

%.o: %.c $(DEPS)
	$(CC) -c @< -o $@ $(CFLAFGS)
	
%.s.o: %.s
	$(AC) @< -o $@

link: 
	$(LC) -T $(LINKER) -o $(BIN) $(LFLAGS) $(OBJ) -lgcc
	
clean:
	-rm *.o $(BIN)
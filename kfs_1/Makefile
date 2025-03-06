CC = gcc
LD = ld
NASM = nasm

CFLAGS = -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs -m32
LDFLAGS = -m elf_i386

DISK_IMAGE = myos.img

all: kernel.bin create_disk_image run

boot.o: boot.asm
	$(NASM) -f elf32 boot.asm -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

kernel.bin: boot.o kernel.o linker.ld
	$(LD) $(LDFLAGS) -T linker.ld -o kernel.bin boot.o kernel.o

create_disk_image: kernel.bin
	@echo "Creating GRUB disk image..."
	sudo ./create_disk_image.sh

run: create_disk_image
	sudo qemu-system-i386 -drive format=raw,file=$(DISK_IMAGE)

clean:
	rm -f *.o kernel.bin $(DISK_IMAGE)


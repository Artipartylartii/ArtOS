# Makefile for PolyOS

TARGET = polyos
ISDIR = build

CC = gcc
AS = nasm
LD = x86_64-linux-gnu-ld

CFLAGS = -std=gnu99 -Wall -O2 -ffreestanding -nostdinc -m64 -mno-red-zone -fno-common -fno-stack-protector
LDFLAGS = -T linker.ld -nostdlib -m elf_x86_64
INCLUDES = -I include

ISO_DIR = $(ISDIR)/iso
ISO_FILE = $(ISDIR)/$(TARGET).iso

HEADERS = $(wildcard include/*.h)

BOOT_OBJ = build/boot/stage1.o build/boot/gdt_flush.o build/boot/idt_flush.o
KERNEL_OBJ = build/kernel/gdt.o build/kernel/idt.o build/kernel/vga.o build/kernel/main.o

all: create_dirs $(ISDIR)/$(TARGET).elf

create_dirs:
	mkdir -p build/boot build/kernel build/iso/boot/grub

# Implicit rules for building objects (depends on create_dirs)
build/boot/%.o: src/boot/%.asm | create_dirs
	$(AS) -f elf64 -o $@ $<

build/boot/%.o: src/boot/%.S | create_dirs
	$(CC) $(CFLAGS) $(INCLUDES) -x assembler-with-cpp -c -o $@ $<

build/kernel/%.o: src/kernel/%.c $(HEADERS) | create_dirs
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(ISDIR)/$(TARGET).elf: $(BOOT_OBJ) $(KERNEL_OBJ) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(BOOT_OBJ) $(KERNEL_OBJ)

iso: $(ISDIR)/$(TARGET).elf | create_dirs
	mkdir -p $(ISO_DIR)/boot/grub
	echo 'set timeout=5' > $(ISO_DIR)/boot/grub/grub.cfg
	echo 'set default=0' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo 'menuentry "PolyOS" {' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '    multiboot /boot/polyos.elf' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '    boot' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '}' >> $(ISO_DIR)/boot/grub/grub.cfg
	cp $(ISDIR)/$(TARGET).elf $(ISO_DIR)/boot/
	grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)

qemu: iso
	qemu-system-x86_64 -cdrom $(ISDIR)/$(TARGET).iso -serial stdio -no-reboot -m 256M

clean:
	rm -rf $(ISDIR)

# Makefile for PolyOS - x86_64 Bare Metal OS

ARCH = x86_64
TARGET = polyos
ISDIR = build

CC = gcc
AS = nasm
LD = x86_64-linux-gnu-ld

CFLAGS = -std=gnu99 -Wall -Wextra -O2 -ffreestanding -nostdinc -m64 -mno-red-zone -fno-common -fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables
ASFLAGS = -f elf64
LDFLAGS = -T linker.ld -nostdlib -m elf_x86_64

INCLUDES = -I include

ISO_DIR = $(ISDIR)/iso
ISO_FILE = $(ISDIR)/$(TARGET).iso
GRUB_DIR = $(ISO_DIR)/boot/grub

BOOT_ASM_SOURCES = $(wildcard src/boot/*.asm)
BOOT_AS_SOURCES = $(wildcard src/boot/*.S)
KERNEL_SOURCES = $(wildcard src/kernel/*.c)
HEADERS = $(wildcard include/*.h)

BOOT_ASM_OBJ = $(BOOT_ASM_SOURCES:src/boot/%.asm=$(ISDIR)/boot/%.o)
BOOT_AS_OBJ = $(BOOT_AS_SOURCES:src/boot/%.S=$(ISDIR)/boot/%.o)
KERNEL_OBJ = $(KERNEL_SOURCES:src/kernel/%.c=$(ISDIR)/kernel/%.o)

BOOT_OBJ = $(BOOT_ASM_OBJ) $(BOOT_AS_OBJ)

.PHONY: all clean iso qemu dirs

all: dirs $(ISDIR)/$(TARGET).elf

dirs:
	@mkdir -p $(ISDIR)/boot $(ISDIR)/kernel $(ISO_DIR)/boot/grub

$(ISDIR)/boot/%.o: src/boot/%.asm linker.ld
	@echo "[AS] $< -> $@"
	@$(AS) $(ASFLAGS) -o $@ $<

$(ISDIR)/boot/%.o: src/boot/%.S $(HEADERS)
	@echo "[CC] $< -> $@"
	@$(CC) $(CFLAGS) $(INCLUDES) -x assembler-with-cpp -c -o $@ $<

$(ISDIR)/kernel/%.o: src/kernel/%.c $(HEADERS)
	@echo "[CC] $< -> $@"
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(ISDIR)/$(TARGET).elf: $(BOOT_OBJ) $(KERNEL_OBJ) linker.ld
	@echo "[LD] -> $@"
	@$(LD) $(LDFLAGS) -o $@ $(BOOT_OBJ) $(KERNEL_OBJ)

iso: $(ISDIR)/$(TARGET).elf
	@echo "[ISO] Creating bootable ISO..."
	@mkdir -p $(GRUB_DIR)
	@echo 'set timeout=0' > $(GRUB_DIR)/grub.cfg
	@echo 'set default=0' >> $(GRUB_DIR)/grub.cfg
	@echo '' >> $(GRUB_DIR)/grub.cfg
	@echo 'menuentry "PolyOS" {' >> $(GRUB_DIR)/grub.cfg
	@echo '    multiboot /boot/polyos.elf' >> $(GRUB_DIR)/grub.cfg
	@echo '    boot' >> $(GRUB_DIR)/grub.cfg
	@echo '}' >> $(GRUB_DIR)/grub.cfg
	@cp $(ISDIR)/$(TARGET).elf $(ISO_DIR)/boot/
	@grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)

qemu: iso
	@echo "[QEMU] Starting PolyOS..."
	@qemu-system-x86_64 -cdrom $(ISDIR)/$(TARGET).iso -display none -serial stdio -no-reboot -machine pc -m 256M

clean:
	@echo "[CLEAN] Removing build artifacts..."
	@rm -rf $(ISDIR)
	@echo "Done."

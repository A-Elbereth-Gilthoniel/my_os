ARCH = i686
CC = i686-elf-gcc

PREF_SRC = kernel/src/
PREF_OBJ = kernel/obj/
ISO = my_os.iso
BIN = myos.bin
ISO_DIR = iso_dir
GRUB_DIR = $(ISO_DIR)/boot/grub

SRC_C := $(wildcard $(PREF_SRC)*.c)
SRC_S := $(wildcard $(PREF_SRC)*.s)

OBJ_S := $(patsubst $(PREF_SRC)%.s, $(PREF_OBJ)%.o, $(SRC_S))
OBJ_C := $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC_C))

OBJ := $(OBJ_C) $(OBJ_S)
OBJ := $(filter-out kernel/obj/boot.o, $(OBJ))

include arch/$(ARCH)/make.config

CFLAGS := $(KERNEL_ARCH_CFLAGS) -ffreestanding -Wall -Wextra
LIBS := $(KERNEL_ARCH_LIBS) -nostdlib -lgcc  -lk


.PHONY : all clean run built-i686

all: $(ISO)

$(ISO) : kernel/obj/$(BIN)
	mkdir -p $(GRUB_DIR)
	cp kernel/obj/$(BIN) $(ISO_DIR)/boot/$(BIN)
	cp kernel/src/grub.cfg $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)

kernel/obj/$(BIN) : kernel/obj/boot.o $(OBJ) kernel/src/linker.ld
	$(CC) -T kernel/src/linker.ld -o kernel/obj/$(BIN) $(CFLAGS) $(LIBS) kernel/obj/boot.o $(OBJ)
	if grub-file --is-x86-multiboot kernel/obj/$(BIN); then \
		echo multiboot confirmed; \
	else \
		echo the file is not multiboot; \
	fi

$(PREF_OBJ)%.o: $(PREF_SRC)%.s
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ -m32 -ffreestanding

$(PREF_OBJ)%.o: $(PREF_SRC)%.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS)

run:
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -f $(PREF_OBJ)*.o $(ISO) $(PREF_OBJ)*.bin
	rm -rf $(ISO_DIR)

built-i686:
	chmod +x built.sh
	./built.sh

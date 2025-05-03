ARCH = i686
CC = i686-elf-gcc

PREF_SRC = src/
PREF_OBJ = obj/
ISO = my_os.iso
BIN = myos.bin
ISO_DIR = iso_dir
GRUB_DIR = $(ISO_DIR)/boot/grub

SRC_C := $(wildcard $(PREF_SRC)*.c)
SRC_S := $(wildcard $(PREF_SRC)*.s)

OBJ_S := $(patsubst $(PREF_SRC)%.s, $(PREF_OBJ)%.o, $(SRC_S))
OBJ_C := $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC_C))

OBJ := $(OBJ_C) $(OBJ_S)

include arch/$(ARCH)/make.config

CFLAGS := $(KERNEL_ARCH_CFLAGS) -ffreestanding -Wall -Wextra
LIBS := $(KERNEL_ARCH_LIBS) -nostdlib -lgcc # -lk

.PHONY : all clean run built-i686

all: $(ISO)

$(ISO) : obj/$(BIN)
	mkdir -p $(GRUB_DIR)
	cp obj/$(BIN) $(ISO_DIR)/boot/$(BIN)
	cp src/grub.cfg $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)

obj/$(BIN) : obj/boot.o obj/kernel.o src/linker.ld
	$(CC) -T src/linker.ld -o obj/$(BIN) $(CFLAGS) $(LIBS) obj/boot.o obj/kernel.o
	if grub-file --is-x86-multiboot obj/$(BIN); then \
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



#    i686-elf-gcc -m32 -ffreestanding -c src/boot.s -o obj/boot.o
#    i686-elf-gcc -c src/kernel.c -o obj/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
#    i686-elf-gcc -T src/linker.ld -o obj/myos.bin -ffreestanding -O2 -nostdlib obj/boot.o obj/kernel.o -lgcc
#    if grub-file --is-x86-multiboot obj/myos.bin; then
#       echo multiboot confirmed;
#    else
#       echo the file is not multiboot;
#    fi
#    mkdir -p iso/boot/grub
#    cp obj/myos.bin isodir/boot/myos.bin
#    cp src/grub.cfg isodir/boot/grub/grub.cfg
#    grub-mkrescue -o myos.iso isodir
#
#    qemu-system-i386 -cdrom myos.iso

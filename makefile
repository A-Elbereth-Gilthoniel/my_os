all:
   i686-elf-gcc -m32 -ffreestanding -c src/boot.s -o obj/boot.o
   i686-elf-gcc -c src/kernel.c -o obj/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
   i686-elf-gcc -T src/linker.ld -o obj/myos.bin -ffreestanding -O2 -nostdlib obj/boot.o obj/kernel.o -lgcc
   if grub-file --is-x86-multiboot obj/myos.bin; then
      echo multiboot confirmed;
   else
      echo the file is not multiboot;
   fi
   mkdir -p iso/boot/grub
   cp obj/myos.bin isodir/boot/myos.bin
   cp src/grub.cfg isodir/boot/grub/grub.cfg
   grub-mkrescue -o myos.iso isodir

   qemu-system-i386 -cdrom myos.iso

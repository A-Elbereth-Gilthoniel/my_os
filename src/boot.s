.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set VIDEO_MODE, 1<<2
.set FLAGS,    ALIGN | MEMINFO | VIDEO_MODE
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.long 0              # header_addr
.long 0              # load_addr
.long 0              # load_end_addr
.long 0              # bss_end_addr
.long 0              # entry_addr

.long 1              # mode_type
.long 80             # width
.long 25             # height
.long 0              # depth

.section .bss
; .align 16
stack_bottom:
.skip 16382
stack_top:

.global stack_top
.section .text
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp
    call kernel_main
    cli
    1: hlt
    jmp 1b

.size _start, . - _start

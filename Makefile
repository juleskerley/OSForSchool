makefile:
all:
    boot2
    boot1

boot2:
    kernel.exe
    objcopy -j .text* -j .data* -j .rodata* -S -O binary kernel.exe boot2

boot1:
    kernel.exe
    nasm -DENTRY=0x$(shell nm kernel.exe | grep main | cut -d " " -f 1) boot1.asm

kernel.exe:
    kernel_c.o
    kernel_asm.o
    buddy_c.o
    conver_c.o
    ld -g -melf_i386 -Ttext 0x10000 -e main -o kernel.exe kernel_c.o kernel_asm.o convert_c.o buddy_c.o

kernel_c.o:
    gcc -g -m32 -fno-stack-protector -c -o kernel_c.o kernel.c

kernel_asm.o:
    nasm -g -f elf -F dwarf -o kernel_asm.o kernel.asm

buddy_c.o:
    gcc -g -m32 -fno-stack-protector -c -o buddy_c.o buddy.c

convert_c.o:
    gcc -g -m32 -fno-stack-protector -c -o convert_c.o convert.c



clean:
    rm *.o *.exe
    rm boot1 boot2

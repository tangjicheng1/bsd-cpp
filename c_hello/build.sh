gcc -ffreestanding -nostdlib -c hello.c -o hello.o
ld -T linker.ld -o hello.bin hello.o -nostdlib
objcopy -O binary hello.bin hello.raw

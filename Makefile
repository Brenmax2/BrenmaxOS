sources = $(wildcard Scripts/*.c Scripts/Others/*.c) kernel.c
flags = -ffreestanding -O3 -Wall -Wextra -fno-stack-protector -g
ofiles = $(sources:.c=.o) # this basically says every c file is a .o file if you can't read
compiler = i686-elf-gcc # change for dif compiler, such as clang or like x86_64-elf-gcc (if conv to 64-bit os)
objcopy = i686-elf-objcopy 
lflags = -T Others/linker.ld -nostdlib
asm = nasm 
aflags = -f bin
all: BrenmaxOS.img

BrenmaxOS.img: kernel.bin BrenmaxOS.asm
	$(asm) $(aflags) "BrenmaxOS.asm" -o "BrenmaxOS.img" # asm file includes incbin so it puts kernel.bin and bl together automatically
kernel.bin: kernel.elf
	$(objcopy) -O binary kernel.elf kernel.bin # going to elf and then .bin because you can use the elf file for gdb, etc
kernel.elf: $(ofiles)
	$(compiler) $(lflags) -o kernel.elf $(ofiles) # use the compiler as linker because i686-elf-gcc also functions as a linker
%.o: %.c
	$(compiler) $(flags) -c $< -o $@
clean:
	rm -rf $(ofiles) kernel.bin kernel.elf BrenmaxOS.img
run: all
	qemu-system-x86_64 -drive format=raw,file="BrenmaxOS.img" -display sdl  -machine pcspk-audiodev=speaker -audiodev sdl,id=speaker

.PHONY: all clean run

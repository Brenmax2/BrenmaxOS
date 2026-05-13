@echo off
setlocal enabledelayedexpansion

set compiler=i686-elf-gcc
set objcopy=i686-elf-objcopy
set asm=nasm
set flags=-ffreestanding -O3 -Wall -Wextra -fno-stack-protector -g
set lflags=-T Others/linker.ld -nostdlib
set aflags=-f bin

echo cleaning up compiled files (if they exist)
if exist *.o del /s /q *.o >nul
if exist kernel.bin del kernel.bin
if exist kernel.elf del kernel.elf
if exist BrenmaxOS.img del BrenmaxOS.img

echo compiling kernel.c
%compiler% %flags% -c kernel.c -o kernel.o
set ofiles=kernel.o
echo compiling in scripts\
for %%f in (Scripts\*.c) do (
    %compiler% %flags% -c "%%f" -o "%%~dpnf.o"
    set ofiles=!ofiles! "%%~dpnf.o"
)
echo compiling in scripts\others
for %%f in (Scripts\Others\*.c) do (
    %compiler% %flags% -c "%%f" -o "%%~dpnf.o"
    set ofiles=!ofiles! "%%~dpnf.o"
)

echo linking
%compiler% %lflags% -o kernel.elf %ofiles%

echo creating kernel.bin from kernel.elf
%objcopy% -O binary kernel.elf kernel.bin

echo making brenmaxos.img from kernel.bin and brenmaxos.asm
%asm% %aflags% "BrenmaxOS.asm" -o "BrenmaxOS.img"

echo completed

echo BrenmaxOS is starting...
qemu-system-x86_64.exe -drive format=raw,file="BrenmaxOS.img" -display sdl

pause
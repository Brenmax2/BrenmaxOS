:: Brenmax Corporation
:: Build System Created by:
:: Jawad Abderrahmane

@echo off
setlocal enabledelayedexpansion

set compiler=i686-elf-gcc
set objcopy=i686-elf-objcopy
set asm=nasm
set flags=-ffreestanding -O3 -Wall -Wextra -fno-stack-protector -g -w
set lflags=-T Others/linker.ld -nostdlib -Wl,--no-warn-rwx-segments
set aflags=-f bin

echo Disposing of old garbage...
    if exist *.o del /s /q *.o >nul
    if exist kernel.bin del kernel.bin
    if exist kernel.elf del kernel.elf
    if exist BrenmaxOS.img del BrenmaxOS.img

echo Compiling kernel.c...
    %compiler% %flags% -c kernel.c -o kernel.o
set ofiles=kernel.o

echo Compiling in scripts...
    for %%f in (Scripts\*.c) do (
        %compiler% %flags% -c "%%f" -o "%%~dpnf.o"
        set ofiles=!ofiles! "%%~dpnf.o"
    )

echo Compiling in scripts\others...
    for %%f in (Scripts\Others\*.c) do (
        %compiler% %flags% -c "%%f" -o "%%~dpnf.o"
        set ofiles=!ofiles! "%%~dpnf.o"
    )

echo Linking...
    %compiler% %lflags% -o kernel.elf %ofiles%

echo Creating kernel.bin...
    %objcopy% -O binary kernel.elf kernel.bin

echo Making brenmaxos.img...
    %asm% %aflags% "BrenmaxOS.asm" -o "BrenmaxOS.img"

echo BrenmaxOS is starting...
    qemu-system-x86_64.exe  -drive format=raw,file="BrenmaxOS.img" -display sdl  -machine pcspk-audiodev=speaker -audiodev sdl,id=speaker


pause
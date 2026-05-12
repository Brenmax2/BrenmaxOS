#include "headers/window.h"
#include "headers/console.h"
#include "headers/disk.h"
#include "headers/input.h"
#include "headers/misc.h"
#include "headers/gui.h"
#include "headers/globals.h"
#include <stdint.h>
uint32_t address = 0x00320000;
    uint8_t disk_readSector(uint32_t target_address, uint32_t LBA) {
        uint8_t val = 0b11100000 | ((LBA >> 24) & 0x0F);
        outb(0x1F6, val);

        outb(0x1F2, 1); //1 sector
        outb(0x1F3, (uint8_t)(LBA >> 0)); //LBA low
        outb(0x1F4, (uint8_t)(LBA >> 8)); //LBA mid
        outb(0x1F5, (uint8_t)(LBA >> 16)); //LBA hig

        outb(0x1F7, 0x20); //read
        inb(0x1F7); inb(0x1F7); inb(0x1F7); inb(0x1F7);//dlay

        while ((inb(0x1F7) & 0x88) != 0x08) {
            uint8_t status = inb(0x1F7);
            if (status == 0xFF) return 1; // flying bus
            if (status & 0x01)  return 1; // faliuer
            if (status & 0x20)  return 1; // drive faliure
        }

        uint16_t *ptr = (uint16_t *)target_address;
        for (uint32_t i = 0; i < 256; i++) {
            ptr[i] = inw(0x1F0); 
        }

        return 0; 
    }

    uint8_t disk_readByte(uint32_t LBA, uint32_t theByte) {
        uint8_t val = 0b11100000 | ((LBA >> 24) & 0x0F);
        outb(0x1F6, val);
        outb(0x1F2, 1); //1 sector
        outb(0x1F3, (uint8_t)(LBA >> 0)); //LBA low
        outb(0x1F4, (uint8_t)(LBA >> 8)); //LBA mid
        outb(0x1F5, (uint8_t)(LBA >> 16)); //LBA hig
        outb(0x1F7, 0x20); //read
        inb(0x1F7); inb(0x1F7); inb(0x1F7); inb(0x1F7);//dlay

        while ((inb(0x1F7) & 0x88) != 0x08) {}
        uint16_t sector_data[256]; 
        for (int i = 0; i < 256; i++) {
            sector_data[i] = inw(0x1F0); 
        }

        uint8_t* bytePtr = (uint8_t*)sector_data;
        return bytePtr[theByte]; 
    }

    uint8_t disk_writeSector(uint16_t *ptr,uint32_t LBA) {
        uint8_t val = 0b11100000 | ((LBA >> 24) & 0x0F);
        outb(0x1F6, val);

        outb(0x1F2, 1); // 1 sector
        outb(0x1F3, (uint8_t)(LBA >> 0));
        outb(0x1F4, (uint8_t)(LBA >> 8));
        outb(0x1F5, (uint8_t)(LBA >> 16));
        outb(0x1F7, 0x30);

        while ((inb(0x1F7) & 0x88) != 0x08) {
            uint8_t status = inb(0x1F7);
            if (status == 0xFF) return 1; // flying bus
            if (status & 0x01)  return 1; // faliuer
            if (status & 0x20)  return 1; // drive faliure
        }

        for (int i = 0; i < 256; i++) {
            outw(0x1F0, ptr[i]);
        }
        return 0;
    }
    
    uint8_t disk_detectIDE() {
        outb(0x1F6, 0xA0); 
        for(int i = 0; i < 4; i++) { inb(0x3F6); }
        uint8_t status = inb(0x1F7);

        if (status == 0xFF) {
            return 0;
        } else if (status == 0) {
            return 1;
        } else {
            return 2;
        }
    }

    void jiouFS_list(){
        uint16_t* table = (uint16_t*)0x00320000;
        uint8_t result = disk_readSector((uint32_t)table, Origin);//rdsc from org

        if (result != 0) {
            printToConsole(21, "Oh no. Error occured.");
            ConsoleNewLine();
        }

        if (table[0] != 0x6767) {//check disc
            printToConsole(16, "Not a BrenmaxFS");
            ConsoleNewLine();
        }

        for (int i = 1; i < 250; i += 12) {
            if ((table[i] & 0xFF) == 0) {break;}
            // print name
            char* fileName = (char*)&table[i];
            printToConsole(9, fileName);
            //print extension
            printToConsole(1, ".");
            char* fileExt = (char*)&table[i + 5];
            printToConsole(4, fileExt);
            if (table[i + 7] == 1) {printToConsole(6, " <DIR>");}

            ConsoleNewLine();
        }

        printToConsole(4, "Done");
        ConsoleNewLine();
        clearInputBuf();
    }

    void jiouFS_readFileTo(uint32_t LBAFO, uint32_t* address, uint32_t lenB){
        uint16_t sextoload = lenB / 512; sextoload ++;
        uint8_t currSec = LBAFO;

        for(int i=0;i<sextoload;i++){
            disk_readSector((uint32_t)address, LBAFO + 201);
            LBAFO ++;
        }
    }
    
    void jiouFS_readFile(int len, char *targetName, uint32_t* address) {
        uint8_t* table = (uint8_t*)0x00320000; 
        disk_readSector((uint32_t)table, Origin);

        for (int i = 2; i < 500; i += 22) { 
            if (table[i] == 0) break; 
            int match = 1;
            char* diskName = (char*)&table[i];
            char* searchName = "BrenmaxOS";

            for(int j = 0; j < 9; j++) { 
                if (diskName[j] != searchName[j]) {
                    match = 0;
                    break;
                }
            }

            if (match) {
                uint32_t startSector = ((uint32_t)table[10] << 16) | table[9];
                uint32_t fileSize = ((uint32_t)table[11] << 16) | table[12];
                disk_readSector(0x00320000, startSector + 201);
                // __asm__ __volatile__ ("int $0x10");
            }
        }
    }

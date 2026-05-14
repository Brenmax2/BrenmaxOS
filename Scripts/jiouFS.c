#include "headers/window.h"
#include "headers/console.h"
#include "headers/disk.h"
#include "headers/input.h"
#include "headers/misc.h"
#include "headers/gui.h"
#include "headers/globals.h"
#include "headers/API.h"
#include <stdint.h>
uint32_t address = 0x00320000; // removing this breaks the os and im not bothered to fix it...

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

    // while ((inb(0x1F7) & 0x88) != 0x08) {
    //     uint8_t status = inb(0x1F7);
    //     if (status == 0xFF) return 1; // flying bus
    //     if (status & 0x01)  return 1; // faliuer
    //     if (status & 0x20)  return 1; // drive faliure
    // }

    wait(1000);

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

void jiouFS_readFile(char *targetName, uint32_t* address) {
    uint8_t* table = (uint8_t*)0x00320000; 
    disk_readSector((uint32_t)table, Origin);

    for (int i = 2; i < 500; i += 24) { 
        if (table[i] == 0) break; 
        int match = 1;
        char* diskName = (char*)&table[i];
        char* searchName = "BrenmaxOS";

        for(int j = 0; j < strlen(targetName); j++) { 
            if (diskName[j] != targetName[j]) {
                match = 0;
                break;
            }
        }

        if (match) {
            // c o n c a t e n a t e
            uint16_t lba_low = (uint16_t)table[i + 16] | ((uint16_t)table[i + 17] << 8);
            uint16_t lba_hi  = (uint16_t)table[i + 18] | ((uint16_t)table[i + 19] << 8);
            uint32_t startSector = (uint32_t)lba_low | ((uint32_t)lba_hi << 16);

            uint16_t size_low = (uint16_t)table[i + 20] | ((uint16_t)table[i + 21] << 8);
            uint16_t size_hi  = (uint16_t)table[i + 22] | ((uint16_t)table[i + 23] << 8);
            uint32_t fileSize = (uint32_t)size_low | ((uint32_t)size_hi << 16);
            disk_readSector(0x00320000, startSector + Origin);

            tostr(startSector);
            print("Start Sector: ");
            print(strres);

            tostr(fileSize);
            print("Bytes: ");
            print(strres);
        }
    }
}

void jiouFS_FormatDisk(){
    uint16_t buffer[256];//declare things
    for(int i=0;i<256;i++){buffer[i]=0x0000;}//clstheram
    
    //show the disk is BrenmaxFS
    buffer[0]=0x6767; //brenmaxfs identifier
    int lbathing = 0;
    int entryOrg = 0;
    //name
    buffer[entryOrg+1]=0x6574;
    buffer[entryOrg+2]=0x7473;
    buffer[entryOrg+3]=0x6E69;
    buffer[entryOrg+4]=0x0067;
    buffer[entryOrg+5]=0x0000;
    //extension file
    buffer[entryOrg+6] = 0x7874; // t x 
    buffer[entryOrg+7] = 0x0074; // t 0 
    //isFolder
    buffer[entryOrg+8]=0x0000;//0 NO
    //LBA Start from origin and size
    buffer[entryOrg+9]=0x0001;//
    buffer[entryOrg+10]=0x0000;//
    buffer[entryOrg+11]=0x0200 ;//bytes extended low
    buffer[entryOrg+12]=0x0000 ;//bytes extended hi

    entryOrg = 12;

    //name
    buffer[entryOrg+1]=0x6973;
    buffer[entryOrg+2]=0x7378;
    buffer[entryOrg+3]=0x7665;
    buffer[entryOrg+4]=0x6E65;
    buffer[entryOrg+5]=0x0000;
    //extension file
    buffer[entryOrg+6] = 0x7874; // t x 
    buffer[entryOrg+7] = 0x0074; // t 0 
    //isFolder
    buffer[entryOrg+8]=0x0000;//0 NO
    //LBA Start from origin and size
    buffer[entryOrg+9]=0x0001;//
    buffer[entryOrg+10]=0x0000;//
    buffer[entryOrg+11]=0x0200 ;//bytes extended low
    buffer[entryOrg+12]=0x0000 ;//bytes extended hi

    entryOrg = 24;

    //name
    buffer[entryOrg+1]=0x696A;
    buffer[entryOrg+2]=0x756F;
    buffer[entryOrg+3]=0x5346;
    buffer[entryOrg+4]=0x0000;
    buffer[entryOrg+5]=0x0000;
    //extension file
    buffer[entryOrg+6] = 0x7874; // t x 
    buffer[entryOrg+7] = 0x0074; // t 0 
    //isFolder
    buffer[entryOrg+8]=0x0000;//0 NO
    //LBA Start from origin and size
    buffer[entryOrg+9]=0x0001;//
    buffer[entryOrg+10]=0x0000;//
    buffer[entryOrg+11]=0x0200 ;//bytes extended low
    buffer[entryOrg+12]=0x0000 ;//bytes extended hi

    lbathing += Origin;
    uint8_t result = disk_writeSector(buffer,lbathing);

    if(result == 0){printToConsole(4, "Done");}else{printToConsole(21, "Oh no. Error occured.");}
}

void jiouFS_createFile(uint32_t bytes, char* name){
    uint16_t buffalo[256];
    uint32_t secstoload = (bytes + 511) / 512;
    disk_readSector((uint32_t)buffalo, 201);

    int entryStart = -1;

    for (int i = 1; i < 256; i += 12) {
        if (buffalo[i] == 0) {
            entryStart = i;
            break;
        }
    }

    if (entryStart == -1) {
        print("Sector is full!");
        return;
    }

    for (uint32_t i = 0; i < 5; i++) { 
        uint8_t char1 = name[i * 2];
        uint8_t char2 = name[i * 2 + 1];
        buffalo[entryStart + i] = (char2 << 8) | char1;
        if (char1 == '\0' || char2 == '\0') break;
    }

    //extens
    buffalo[entryStart + 5] = 0x7874; 
    buffalo[entryStart + 6] = 0x0074;
    //isfolder
    buffalo[entryStart + 7] = 0x0001; 
    //sex
    buffalo[entryStart + 8] = (uint16_t)(secstoload & 0xFFFF);       
    buffalo[entryStart + 9] = (uint16_t)((secstoload >> 16) & 0xFFFF); 
    //bits in sise
    buffalo[entryStart + 10] = (uint16_t)(bytes & 0xFFFF);       
    buffalo[entryStart + 11] = (uint16_t)((bytes >> 16) & 0xFFFF); 
    disk_writeSector(buffalo, 201);
    
    print("Created at index: ");
    tostr(entryStart);
    print(strres);
}
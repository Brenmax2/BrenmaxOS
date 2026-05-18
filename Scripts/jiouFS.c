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

uint8_t disk_writeSectorPRO(uint32_t *ptr,uint32_t LBA) {
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

    uint16_t *ptr16 = (uint16_t *)ptr;
    for (int i = 0; i < 256; i++) {
        outw(0x1F0, ptr16[i]);
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

    print("Done");
    ConsoleNewLine();
    clearInputBuf();
}

void jiouFS_writeFile(char* name, uint8_t* arrayOfBytes, uint32_t bytesCount) {
    uint8_t table[512]; 
    if (disk_readSector((uint32_t)table, 201) != 0) {
        print("Disk Error reading table");
        return;
    }

    int targetLen = strlen(name);

    for (int i = 2; i < 500; i += 24) { 
        if (table[i] == 0) continue;
        int match = 1;
        char* diskName = (char*)&table[i];

        for(int j = 0; j < targetLen; j++) { 
            if (diskName[j] != name[j]) {
                match = 0;
                break;
            }
        }

        if (match && diskName[targetLen] != '\0' && diskName[targetLen] != ' ') {
            match = 0;
        }

        if (match) {
            uint16_t lba_low = (uint16_t)table[i + 16] | ((uint16_t)table[i + 17] << 8);
            uint16_t lba_hi  = (uint16_t)table[i + 18] | ((uint16_t)table[i + 19] << 8);
            uint32_t startSector = (uint32_t)lba_low | ((uint32_t)lba_hi << 16);

            uint32_t sectorsToWrite = (bytesCount + 511) / 512;

            for (uint32_t s = 0; s < sectorsToWrite; s++) {
                uint16_t* sectorPtr = (uint16_t*)(arrayOfBytes + (s * 512));
                disk_writeSector(sectorPtr, startSector + 201 + s); 
            }

            print("File data written to LBA: ");
            tostr(startSector + 201);
            print(strres);
            ConsoleNewLine();
            return;
        }
    }
    print("File not found for writing.");
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
            // c o n c a t e n a t e or cacoonate as i like to say it
            uint32_t SexToLoad = 0;
            uint16_t lba_low = (uint16_t)table[i + 16] | ((uint16_t)table[i + 17] << 8);
            uint16_t lba_hi  = (uint16_t)table[i + 18] | ((uint16_t)table[i + 19] << 8);
            uint32_t startSector = (uint32_t)lba_low | ((uint32_t)lba_hi << 16);

            uint16_t size_low = (uint16_t)table[i + 20] | ((uint16_t)table[i + 21] << 8);
            uint16_t size_hi  = (uint16_t)table[i + 22] | ((uint16_t)table[i + 23] << 8);
            uint32_t fileSize = (uint32_t)size_low | ((uint32_t)size_hi << 16);

            SexToLoad = (fileSize + 511) / 512;
            tostr(SexToLoad);
            print("Loading ");
            print(strres);
            
            for(uint32_t i = 0; i < SexToLoad ; i++){
                ConsoleNewLine();
                print("Load Sector");
                tostr(startSector+i-1+Origin);
                print(strres);
                disk_readSector((uint32_t)address + (512*i), startSector + i + Origin);
            }

            ConsoleNewLine();
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
    uint16_t buffer[256];
    for(int i=0;i<256;i++){buffer[i]=0x0000;}
    //show the disk is BrenmaxFS
    buffer[0]=0x6767; //brenmaxfs identifier i love 67
    int lbathing = 0;
    int entryOrg = 0;
    lbathing += Origin;
    uint8_t result = disk_writeSector(buffer,lbathing);
    if(result == 0){printToConsole(4, "Done");}else{printToConsole(21, "Oh no. Error occured.");}
}

void jiouFS_createFile(uint32_t bytes, char* name, char* extension){
    uint16_t buffalo[256];
    uint32_t secstoload = (bytes + 511) / 512;
    disk_readSector((uint32_t)buffalo, 201);

    int entryStart = -1;
    uint32_t nextAvailableLBA = 1; 

    for (int i = 1; i < 256; i += 12) {
        if (buffalo[i] == 0) {
            if (entryStart == -1) entryStart = i; 
        } else {
            uint32_t entryLBA = buffalo[i + 8] | (buffalo[i + 9] << 16);
            uint32_t entryBites = buffalo[i + 10] | (buffalo[i + 11] << 16);
            uint32_t entrySectors = (entryBites + 511) / 512;

            if (entryLBA + entrySectors > nextAvailableLBA) {
                nextAvailableLBA = entryLBA + entrySectors;
            }
        }
    }

    if (entryStart == -1) {
        print("Oh no.");
        return;
    }

    // name
    for (uint32_t i = 0; i < 5; i++) { 
        uint8_t char1 = name[i * 2];
        uint8_t char2 = name[i * 2 + 1];
        buffalo[entryStart + i] = (char2 << 8) | char1;
        if (char1 == '\0' || char2 == '\0') break;
    }

    // extens
    buffalo[entryStart + 5] = (extension[1] << 8) | extension[0]; 
    buffalo[entryStart + 6] = (0x00 << 8) | extension[2];
    // folder?
    buffalo[entryStart + 7] = 0x0000; 
    // lba start
    buffalo[entryStart + 8] = (uint16_t)(nextAvailableLBA & 0xFFFF);       
    buffalo[entryStart + 9] = (uint16_t)((nextAvailableLBA >> 16) & 0xFFFF); 
    // size in bites
    buffalo[entryStart + 10] = (uint16_t)(bytes & 0xFFFF);       
    buffalo[entryStart + 11] = (uint16_t)((bytes >> 16) & 0xFFFF); 
    
    disk_writeSector(buffalo, 201);
    
    print("Entry start: ");
    tostr(entryStart);
    print(strres);
}

uint8_t jiouFS_deleteFile(char* name) {
    uint16_t buffalo[256];
    disk_readSector((uint32_t)buffalo, 201);

    for (int i = 1; i < 256; i += 12) {
        bool match = true;
        for (int j = 0; j < 5; j++) {
            uint8_t char1 = name[j * 2];
            uint8_t char2 = name[j * 2 + 1];
            uint16_t nameWord = (char2 << 8) | char1;
            if (buffalo[i + j] != nameWord) {match = false;break;}
            if (char1 == '\0' || char2 == '\0') break;
        }

        if (match && buffalo[i] != 0) {
            for (int k = 0; k < 12; k++) {buffalo[i + k] = 0;}
            uint8_t result = disk_writeSector(buffalo, 201);
            if (result == 0) {return 0;} else {return 1;}
        }
    }
    return 2;
}

uint8_t jiouFS_renameFile(char* oldName, char* newName) {
    uint16_t buffalo[256];
    disk_readSector((uint32_t)buffalo, 201);

    for (int i = 1; i < 256; i += 12) {
        int match = 1;
        
        for (int j = 0; j < 5; j++) {
            uint8_t char1 = oldName[j * 2];
            uint8_t char2 = oldName[j * 2 + 1];
            uint16_t nameWord = (char2 << 8) | char1;

            if (buffalo[i + j] != nameWord) {
                match = 0;
                break;
            }
            if (char1 == '\0' || char2 == '\0') break;
        }

        if (match==1 && buffalo[i] != 0) {
            for (int k = 0; k < 5; k++) {
                buffalo[i + k] = 0;
            }

            for (uint32_t i_name = 0; i_name < 5; i_name++) { 
                uint8_t n_char1 = newName[i_name * 2];
                uint8_t n_char2 = newName[i_name * 2 + 1];
                buffalo[i + i_name] = (n_char2 << 8) | n_char1;
                if (n_char1 == '\0' || n_char2 == '\0') break;
            }

            uint8_t result = disk_writeSector(buffalo, 201);
            
            if (result == 0) {
                print("Renamed file");
                return 0;
            } else {
                print("Oh no. Error occured.");
                return 1;
            }
        }
    }

    print("Oh no. File not found");
    return 2;
}

void openTxtView(char* name){
    jiouFS_readFile(name, (uint32_t*)0x00770000);
    add_window(8, 100, 100, 640, 480, "File content");
    for(uint8_t i = 0; i < strlen(name) ; i++){
        filenamecurrtextviewdawgwtf[i] = name[i];
        filenamecurrtextviewdawgwtf[i+1] = '\0';
    }
}

void TxtView_Loop(){
    uint16_t nexty = 12;
    drawStringToWindow(8, filenamecurrtextviewdawgwtf, 3, 3, 0x0000);
    for(int i = 0; i < 100; i++){
        drawStringToWindow(8, "the vacumm clearner as exploded successfully", 3, nexty, 0x0000);
    }
}
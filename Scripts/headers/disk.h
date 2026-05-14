#pragma once
#include <stdint.h>
uint8_t disk_readSector(uint32_t target_address, uint32_t LBA);
uint8_t disk_writeSector(uint16_t *ptr,uint32_t LBA);
uint8_t disk_detectIDE();
void jiouFS_readFileTo(uint32_t LBAFO, uint32_t* address, uint32_t lenB);
void jiouFS_readFile(char *targetName, uint32_t* address);
void jiouFS_list();
void tostr(uint32_t number);
void jiouFS_FormatDisk();
void jiouFS_createFile(uint32_t bytes, char* name);
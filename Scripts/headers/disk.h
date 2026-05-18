#pragma once
#include <stdint.h>
uint8_t disk_readSector(uint32_t target_address, uint32_t LBA);
uint8_t disk_writeSector(uint16_t *ptr,uint32_t LBA);
uint8_t disk_writeSectorPRO(uint32_t *ptr,uint32_t LBA);
uint8_t disk_detectIDE();

void jiouFS_readFile(char *targetName, uint32_t* address);
void jiouFS_list();
void jiouFS_FormatDisk();
void jiouFS_createFile(uint32_t bytes, char* name, char* extension);
void jiouFS_writeFile(char* name, uint8_t* arrayOfBytes, uint32_t bytesCount);
uint8_t jiouFS_deleteFile(char* name);
uint8_t jiouFS_renameFile(char* oldName, char* newName);

void openTxtView(char* name);
void TxtView_Loop();
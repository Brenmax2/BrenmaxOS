#pragma once
#include <stdint.h>

void System_restart();
void System_beep();
void tostr(uint32_t number);
uint8_t strcmp(char *str1, char *str2);
uint32_t strlen(const char* s);

#pragma once
#include <stdint.h>
void Console_clearConsole();
void Console_loop();
void Console_setColor(uint8_t color);
void ConsoleNewLine();
void printToConsole(uint32_t len, char *text);
void clearInputBuf();
void openConsole();
void print(char *text);
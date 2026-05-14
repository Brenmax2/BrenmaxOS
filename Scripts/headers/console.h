#pragma once
#include <stdint.h>
void ClearConsole();
void ConsoleLoop();
void ConsoleNewLine();
void printToConsole(uint32_t len, char *text);
void clearInputBuf();
void openConsole();
void print(char *text);
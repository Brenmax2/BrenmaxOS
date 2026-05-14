#include "headers/window.h"
#include "headers/console.h"
#include "headers/disk.h"
#include "headers/input.h"
#include "headers/misc.h"
#include "headers/gui.h"
#include "headers/globals.h"
#include <stdint.h>

void System_beep(){
    beep();
}

void System_restart(){
    __asm__ __volatile__ ("int $0x10");
}

uint32_t strlen(const char* s) {
    if (!s) return 0;
    const char *p = s;
    while (*p != '\0') {
        p++;
    }
    return p-s;
}

uint8_t strcmp(char *str1, char *str2){
    uint16_t len1 = strlen(str1);
    uint16_t len2 = strlen(str2);
    uint16_t len = 0;

    if(len1 < len2){
        len = len2;
    }else{
        len = len1;
    }

    for(uint8_t i = 0; i<len ; i++){
        if(str1[i] != str2[i]) {
            return 0;
        }else{return 1;}
    }
}

//fahh this was a painful one...
void tostr(uint32_t number) {
    int i = 0;

    if (number == 0) {
        strres[i++] = '0';
        strres[i] = '\0';
        return;
    }

    while (number > 0) {
        uint32_t digit = number % 10;
        strres[i++] = (char)(digit + '0');
        number = number / 10;
    }

    strres[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = strres[start];
        strres[start] = strres[end];
        strres[end] = temp;
        start++;
        end--;
    }
}
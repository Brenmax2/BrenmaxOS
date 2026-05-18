#include "headers/window.h"
#include "headers/console.h"
#include "headers/disk.h"
#include "headers/input.h"
#include "headers/misc.h"
#include "headers/gui.h"
#include "headers/globals.h"
#include <stdint.h>

void krestart(){
    __asm__ __volatile__ ("int $0x10");
}


uint32_t kstrlen(const char* s) {
    if (!s) return 0;
    const char *p = s;
    while (*p != '\0') {
        p++;
    }
    return p-s;
}
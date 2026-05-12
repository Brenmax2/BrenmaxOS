//   ____                                      
//  | __ ) _ __ ___ _ __  _ __ ___   __ ___  __
//  |  _ \| '__/ _ \ '_ \| '_ ` _ \ / _` \ \/ /
//  | |_) | | |  __/ | | | | | | | | (_| |>  < 
//  |____/|_|  \___|_| |_|_| |_| |_|\__,_/_/\_\ 
//

#include "Scripts/headers/window.h"
#include "Scripts/headers/console.h"
#include "Scripts/headers/disk.h"
#include "Scripts/headers/input.h"
#include "Scripts/headers/misc.h"
#include "Scripts/headers/gui.h"
#include "Scripts/headers/globals.h"
#include <stdint.h>
__attribute__((section(".text.entry")))
void kernel_main() {
    init_mouse();
    add_window(1, 10, 10, 300, 200, title);

    __asm__ __volatile__(
        "1:\n"            
        "call loop\n"     
        "jmp 1b\n"        
    );

}


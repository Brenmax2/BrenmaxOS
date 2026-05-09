//   ____                                      
//  | __ ) _ __ ___ _ __  _ __ ___   __ ___  __
//  |  _ \| '__/ _ \ '_ \| '_ ` _ \ / _` \ \/ /
//  | |_) | | |  __/ | | | | | | | | (_| |>  < 
//  |____/|_|  \___|_| |_|_| |_| |_|\__,_/_/\_\ 
//

#include <stdint.h>
#include "Scripts/Others/fontFamily.c"
#include "Scripts/Others/images.c"
#include "Scripts/functionDeclare.c"
#include "Scripts/varDeclare.c"

void kernel_main() {
    init_mouse();
    add_window(1, 10, 10, 300, 200, title);

    __asm__ __volatile__(
        "1:\n"            
        "call loop\n"     
        "jmp 1b\n"        
    );

}

#include "Scripts/GUI.c"
#include "Scripts/windows.c"
#include "Scripts/misc.c"
#include "Scripts/jiouFS.c"
#include "Scripts/command.c"
#include "Scripts/loop.c"

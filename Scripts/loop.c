#include "headers/window.h"
#include "headers/console.h"
#include "headers/disk.h"
#include "headers/input.h"
#include "headers/misc.h"
#include "headers/gui.h"
#include "headers/globals.h"
#include "headers/API.h"
#include <stdint.h>

int readyto = 0;
int readyto1 = 0;
int readyto2 = 0;
int readyto3 = 0;
int ready1 = 1;

void loop() {
    if(ready1 == 1){
        ready1 = 0;
        // thewayyouunderstandme();
    }
    drawDesktop();
    drawAnimation();
    handle_ps2_input();
    ManageTheKB();
    last_scancode1 = last_scancode;
    updateRTC();
    
    // Taskbar
    draw_RectTRANS(768 - 35, 0, 1024, 35, 0x52AA, 3);
    drawButton(768 - 28, 4, 22, 22, 0xFFFF, "", 2);
    draw_Rect(768 - 35, 0, 1024, 1, 0xC618);

    drawButton(768 - 28, 26 + 5, 90, 22, 0xFFFF, "CMD Prompt", 3);
    drawButton(768 - 28, 129 + 5, 90, 22, 0xFFFF, "Welcome", 5);
    drawButton(768 - 28, 232 + 5, 90, 22, 0xFFFF, "Piano", 6);
    drawButton(768 - 28, 335 + 5, 90, 22, 0xFFFF, "Settings", 7);
    
    drawString1(time, 1024 - 77, 768 - 8 - 15, 0xFFFF);
    
    for (int i = 0; i < 9; i++) {
        uint16_t grid_row = i / 3;
        uint16_t grid_col = i % 3;
        uint16_t square_l = 4 + 2 + (grid_col * 7); 
        uint16_t square_t = 768 - 28 + 2 + (grid_row * 7);
        draw_Rect(square_t, square_l, 5, 5, 0xFFFF);
    }

    if (inb(0x1F7) & 0x08) {
        if (address != 0) {
            uint16_t* ptr = (uint16_t*)address;
            for (int i = 0; i < 256; i++) {
                ptr[i] = inw(0x1F0);
            }
            System_beep();
            address = 0;
        }
    }

    if(buttonStates[7] == 1){
        if(readyto3 == 1){
            add_window(7, 50, 150, 340, 240, "GUI Preferences");
            readyto3 = 0;
        }
    }else{
        readyto3 = 1;
    }

    if(buttonStates[3] == 1){
        if(readyto == 1){
            openConsole();
            readyto = 0;
        }
    }else{
        readyto = 1;
    }

    if(buttonStates[6] == 1){
        if(readyto2 == 1){
            add_window(5, 100, 100, 340, 160, "Piano");
            readyto2 = 0;
        }
    }else{
        readyto2 = 1;
    }
    
    if(buttonStates[5] == 1){
        if(readyto1 == 1){
            add_window(1, 10, 10, 300, 200, title);
            readyto1 = 0;
        }
    }else{
        readyto1 = 1;
    }
    
    if (buttonStates[2] == 1) {
        if (runned == 0) {
            runned = 1; 
            
            if (startOpened) {
                startOpened = 0;
            } else {
                startOpened = 1;
            }
        }
    } else {
        runned = 0;
    }

    if(startOpened){
        draw_Rect(768 - 200 - 25, 0, 150, 200, 0x52AA);
    }

    if (last_scancode == 0x3B) {
        last_scancode = 0;
        add_window(3, winX, winY, 70, 70, title2);        
    }

    if (last_scancode == 0x3C) {
        last_scancode = 0;
        openConsole();
    }

    if (last_scancode == 0x3E) {
        last_scancode = 0;
        closeTopWindow();
    }
    
    drawAllWindows(); 
    handleWindowDragging();                
    drawMouse(mouse_x, mouse_y);
    swapBuffers();
}

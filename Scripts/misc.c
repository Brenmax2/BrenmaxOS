#include "headers/window.h"
#include "headers/console.h"
#include "headers/disk.h"
#include "headers/input.h"
#include "headers/misc.h"
#include "headers/gui.h"
#include "headers/globals.h"
#include <stdint.h>


static void play_sound(uint32_t nFrequence) {
    uint32_t Div;
    uint8_t tmp;

    Div = 1193180 / nFrequence;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t) (Div) );
    outb(0x42, (uint8_t) (Div >> 8));

    tmp = inb(0x61);
    if (tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
}

static void nosound() {
    uint8_t tmp = inb(0x61) & 0xFC;
    
    outb(0x61, tmp);
}

void beep(){
    play_sound(1000);
    wait(100);
    nosound();
}

void updateRTC() {
    unsigned char regB;
    unsigned char min_bcd;
    int min_int;

    outb(0x70, 0x0B);
    regB = inb(0x71);

    outb(0x70, 0x02);
    min_bcd = inb(0x71);

    if (!(regB & 0x04)) {min_int = (min_bcd & 0x0F) + ((min_bcd / 16) * 10);} else {min_int = min_bcd;}

    time[3] = (min_int / 10) + '0'; 
    time[4] = (min_int % 10) + '0'; 

    outb(0x70, 0x04);
    min_bcd = inb(0x71);

    if (!(regB & 0x04)) {min_int = (min_bcd & 0x0F) + ((min_bcd / 16) * 10);} else {min_int = min_bcd;}

    if (!(regB & 0x02) && (min_bcd & 0x80)) {
        min_int = (min_int + 12) % 24;
    }

    time[0] = (min_int / 10) + '0'; 
    time[1] = (min_int % 10) + '0'; 

    outb(0x70, 0x00);
    min_bcd = inb(0x71);

    if (!(regB & 0x04)) {min_int = (min_bcd & 0x0F) + ((min_bcd / 16) * 10);} else {min_int = min_bcd;}
    time[6] = (min_int / 10) + '0'; 
    time[7] = (min_int % 10) + '0'; 
}

void WelcomeLoop(){
    int charCount = 0;
    char pressf4toclose[] = "Press F4 to close window";
    drawStringToWindow(1, "Welcome to BrenmaxOS!\n\nThis operating system is a 32\nbit protected mode OS with\na GUI.\n\nAlso if your mouse is not working\nuse the arrow keys to move the\nmouse and press F5 to activate click.\n\nFEATURES:\n- Piano\n- PS/2 Drivers\n- 640x480@16bit color\n\nIMPORTANT:Mouse not working?\nTry using the arrow keys.", 2, 2, 0x0000);
}

void drawAllWindows() {
    for (int i = 0; i < window_count; i++) {
        uint16_t top    = windows[i][0];
        uint16_t left   = windows[i][1];
        uint16_t width  = windows[i][2];
        uint16_t height = windows[i][3];
        uint8_t  id     = (uint8_t)windows[i][4];

        if(width == 0)return;

        if(transUI == 1){
            draw_RectTRANS(top, left, width + 8, height + 20, 0xA514, 2);
        }else{
            draw_Rect(top - 1, left - 1, width + 10, height + 22, 0x0000);
            draw_RectTRANS(top, left, width + 8, height + 20, 0x4208, 2);
        }
        draw_Rect(top + 16, left + 4, width, height, 0xFFFF);
        drawStringToWindowTitle(id, window_title_ptrs[i], 2, 0, 0xFFFF);

        drawbuttonPRO(top + 2, left + width - 5, 10, 10, 0xFFFF, "x", 4, 1, 1);


        if (id == 1) {
            WelcomeLoop();
        } else if (id == 2) {
            drawStringToWindow(2, "This is a new window", 2, 2, 0x0000);
        } else if (id == 3){
            uint16_t amount = 0;

            for (int y = 0; y < 70; y++) {
                for (int x = 0; x < 70; x++) {
                    if (amount < 4900) { 
                        unsigned short color = ArchPic[amount];
                        drawPixelToWindow(3, 0 + x, 0 + y, color);
                        amount++;
                    }
                }
            }
        } else if (id == 4){
            ConsoleLoop();
        } else if (id == 5){
            PianoLoop();
        } else if (id == 6){
            uint16_t amount = 0;

            for (int y = 10; y < 40; y++) {
                for (int x = 10; x < 40; x++) {
                    if (amount < 30*30) { 
                        unsigned short color = Trophy[amount];
                        drawPixelToWindow(6, 0 + x, 0 + y, color);
                        amount++;
                    }
                }
            }

            drawStringToWindow(6, "HONOURABLE MENTIONS\nThese people have contributed\nin testing the of the operating\nSystem. These people deserve\nrecognition. Here are their\nusernames on DC!", 50, 10, 0x0000);
            drawStringToWindow(6, "Usernames on discord\n - t3ck\n - eta knight\n - !@ kitsy\n - Puzzle Master\n\nJoin discord to be part of here!", 5, 80, 0x0000);
        } else if (id == 7){
            char option[2] = " ";

            if (transUI == 1) { option[0] = 'X';
            } else {
                option[0] = ' ';
            }
            option[1] = '\0'; 
            drawStringToWindow(7, "SETTINGS", 4, 4, 0x0000);
            drawStringToWindow(7, "These settings will NOT be saved.\nTry out these wallpapers.", 4, 14, 0x0000);
            drawButtonToWindow(7, 40, 4, 85, 50, 0xffff, "Default 1", 9);
            drawRectToWindow(7, 54, 8, 30, 30, 0x07E0); 
            drawRectToWindow(7, 54, 38, 30, 30, 0x001F); 

            drawButtonToWindow(7, 40 + 50 + 5, 4, 85, 50, 0xffff, "WebOS Grad", 8);
            drawRectToWindow(7, 54 + 50 + 5, 8, 30, 30, 0x001F); 
            drawRectToWindow(7, 54 + 50 + 5, 38, 30, 30, 0x881F); 
            
            drawStringToWindow(7, "GUI Transparency", 4, (54 + 50) + 50 + 2, 0x0000);
            drawButtonToWindow(7,((54+50)+50+2)+10, 5, 14, 14, 0xffff, option, 10);

            if (buttonStates[10] == 1) {
                if (runned959 == 0) {
                    runned959 = 1; 
                    if (transUI) {
                        transUI = 0;
                    } else {
                        transUI = 1;
                    }
                }
            } else {runned959 = 0;}

            //T L W H
            if(buttonStates[9] == 1){
                Wallpaper = 0;
            }
            
            if(buttonStates[9] == 1){
                Wallpaper = 0;
            }

            if(buttonStates[8] == 1){
                Wallpaper = 1;
            }

        }
    }
}

void PianoLoop() {
    drawStringToWindow(5, "Welcome to Piano!\nPress a the labeled key on keyboard.", 2, 2, 0x0000);
    drawRectToWindow(5, 20, 1, 338, 140, 0x8430); 

    char* whiteLabels[] = {"TB", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "\\"};
    char* blackLabels[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};

    
    for (uint16_t i = 0; i < 14; i++) {
        uint16_t xPos = 2 + (i * 24);
        drawRectToWindow(5, 22, xPos, 23, 136, 0xFFFF);
        drawStringToWindow(5, whiteLabels[i], xPos + 4, 22 + 120, 0xFFFF);
    }
    
    drawRectToWindow(5, 22, 18, 14, 80, 0x0000);
    drawStringToWindow(5, "1", 22, 22 + 60, 0xFFFF);

    drawRectToWindow(5, 22, 42, 14, 80, 0x0000);
    drawStringToWindow(5, "2", 46, 22 + 60, 0xFFFF);

    drawRectToWindow(5, 22, 90, 14, 80, 0x0000);
    drawStringToWindow(5, "3", 94, 22 + 60, 0xFFFF);

    drawRectToWindow(5, 22, 114, 14, 80, 0x0000);
    drawStringToWindow(5, "4", 118, 22 + 60, 0xFFFF);

    drawRectToWindow(5, 22, 138, 14, 80, 0x0000);
    drawStringToWindow(5, "5", 142, 22 + 60, 0xFFFF);

    drawRectToWindow(5, 22, 18 + 168, 14, 80, 0x0000);
    drawStringToWindow(5, "6", 18 + 172, 22 + 60, 0xFFFF);

    drawRectToWindow(5, 22, 42 + 168, 14, 80, 0x0000);
    drawStringToWindow(5, "7", 42 + 172, 22 + 60, 0xFFFF);

    drawRectToWindow(5, 22, 90 + 168, 14, 80, 0x0000);
    drawStringToWindow(5, "8", 90 + 172, 22 + 60, 0xFFFF);

    drawRectToWindow(5, 22, 114 + 168, 14, 80, 0x0000);
    drawStringToWindow(5, "9", 114 + 172, 22 + 60, 0xFFFF);

    drawRectToWindow(5, 22, 138 + 168, 14, 80, 0x0000);
    drawStringToWindow(5, "0", 138 + 172, 22 + 60, 0xFFFF);


    if(last_scancode1 == 0x0F){         
        play_sound(261);
    }else if(last_scancode1 == 0x02){   
        play_sound(277);
    }else if(last_scancode1 == 0x10){   
        play_sound(293);
    }else if(last_scancode1 == 0x03){   
        play_sound(311);
    }else if(last_scancode1 == 0x11){  
        play_sound(329);
    }else if(last_scancode1 == 0x12){   
        play_sound(349);
    }else if(last_scancode1 == 0x04){   
        play_sound(369);
    }else if(last_scancode1 == 0x13){   
        play_sound(392);
    }else if(last_scancode1 == 0x05){ 
        play_sound(415);
    }else if(last_scancode1 == 0x14){   
        play_sound(440);
    }else if(last_scancode1 == 0x06){   
        play_sound(466);
    }else if(last_scancode1 == 0x15){   
        play_sound(493);
    }else if(last_scancode1 == 0x16){   
        play_sound(523);
    }else if(last_scancode1 == 0x07){   
        play_sound(554);
    }else if(last_scancode1 == 0x17){ 
        play_sound(587);
    }else if(last_scancode1 == 0x08){  
        play_sound(622);
    }else if(last_scancode1 == 0x18){   
        play_sound(659);
    }else if(last_scancode1 == 0x19){
        play_sound(698);
    }else if(last_scancode1 == 0x09){  
        play_sound(739);
    }else if(last_scancode1 == 0x1A){
        play_sound(783);
    }else if(last_scancode1 == 0x0A){   
        play_sound(830);
    }else if(last_scancode1 == 0x1B){   
        play_sound(880);
    }else if(last_scancode1 == 0x0B){   
        play_sound(932);
    }else if(last_scancode1 == 0x2B){   
        play_sound(987);
    }else{
        nosound();
    }

}

void init_mouse() {
    outb(0x64, 0xA8);
    outb(0x64, 0xD4);
    outb(0x60, 0xF4);
    wait(1);
}

void ManageTheKB(){
    if (last_scancode1 == 0x48) {
        mouse_y -= veloy;
    }

    if (last_scancode1 == 0x50) {
        mouse_y += veloy;
    }

    if (last_scancode1 == 0x4B) {
        mouse_x -= velox;
    }

    if (last_scancode1 == 0x4D) {
        mouse_x += velox;
    }

    if (mouse_x < 0) mouse_x = 0;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_x > 1024 - 1) mouse_x = 1024 - 1;
    if (mouse_y > 768 - 1) mouse_y = 768 - 1;

    if (last_scancode1 == 0x3F) {
        mouseDown = 1; 
    } else if (last_scancode1 == 0xBF) {
        mouseDown = 0;
        last_scancode1 = 0; 
    }
}

void handle_ps2_input() {
    uint8_t status = inb(0x64);
    if (!(status & 0x01)) return; 

    if (status & 0x20) {
        updateMouse();
    } else {
        uint8_t scancode = inb(0x60);

        if (scancode == 0xE0) {
            is_extended = 1;
            return;
        }

        if (is_extended) {
            if (scancode == 0x48 || scancode == 0x50 || scancode == 0x4B || scancode == 0x4D) {
                last_scancode = scancode; 
            } 
            else if (scancode == 0xC8 || scancode == 0xD0 || scancode == 0xCB || scancode == 0xCD) {
                last_scancode = 0;
            }
            is_extended = 0;
        } else {
            if (scancode == 0x3F) mouseDown = 1;
            else if (scancode == 0xBF) mouseDown = 0;
            else if (scancode < 0x80) last_scancode = scancode;
            else last_scancode = 0;
        }
    }
}

void updateMouse() {
    for (int i = 0; i < 3; i++) { 
        if (!(inb(0x64) & 1)) {
            return; 
        }

        uint8_t data = inb(0x60);

        if (mouse_cycle == 0) {
            if (!(data & 0x08)) continue; 
            mouse_bytes[0] = data;
            mouse_cycle = 1;
        } 
        else if (mouse_cycle == 1) {
            mouse_bytes[1] = data;
            mouse_cycle = 2;
        } 
        else if (mouse_cycle == 2) {
            mouse_bytes[2] = data;
            mouse_cycle = 0; 

            uint8_t flags = mouse_bytes[0];
            int8_t rel_x = (int8_t)mouse_bytes[1];
            int8_t rel_y = (int8_t)mouse_bytes[2];

            mouseDown = (flags & 0x01); 
            mouse_x += rel_x;
            mouse_y -= rel_y; 

            if (mouse_x < 0) mouse_x = 0;
            if (mouse_y < 0) mouse_y = 0;
            if (mouse_x > 1024 - 1) mouse_x = 1024 - 1;
            if (mouse_y > 768 - 1) mouse_y = 768 - 1;
        }
    }
}

static inline void cpu_relax(void) {
    __asm__ __volatile__("nop");
}

void wait(uint32_t seconds) {
    volatile uint32_t i, j;

    for (i = 0; i < seconds; i++) {
        for (j = 0; j < 100000; j++) {
            cpu_relax();
        }
    }
}

void thewayyouunderstandme(){
    for (int i = 0; i < 1024*768; i++) {
        disp[i] = 0;
    }
    int startX = 270;
    int startY = 190;

    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 100; x++) {

            int sourceX = x / 2;
            int sourceY = y / 2;
            
            int sourceIndex = (sourceY * 50) + sourceX;
            if (sourceIndex < 2500) {
                unsigned short color = iconBoot[sourceIndex];
                drawPixel(startX + x, startY + y, color);
            }
        }
    }

    swapBuffers();
    FamilyMart();
    nosound();
    wait(3000);
}

void FamilyMart(){
    play_sound(739);
    wait(1500);
    play_sound(587);
    wait(1500);
    play_sound(440);
    wait(1500);
    play_sound(587);
    wait(1500);
    play_sound(659);
    wait(1500);
    play_sound(880); 
    wait(2000); 
    nosound();
    wait(1000); 

    play_sound(440); 
    wait(1500);
    play_sound(659);
    wait(1500);
    play_sound(739);
    wait(1500);
    play_sound(659); 
    wait(1500);
    play_sound(440); 
    wait(1500);
    play_sound(587);
    wait(3000);

    nosound();
    wait(3000);
}

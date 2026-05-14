#include "headers/window.h"
#include "headers/console.h"
#include "headers/disk.h"
#include "headers/input.h"
#include "headers/misc.h"
#include "headers/gui.h"
#include "headers/globals.h"
#include "headers/API.h"
#include <stdint.h>
int started = 0;

void ClearConsole(){
    for (int i = 0; i < 37*37; i++) {
        console_buffer[i] = ' ';
    }

    for (int i = 0; i < 60; i++) {
        inputBufferIndex = 0;
        inputBuffer[i] = ' ';
    }
}

void ConsoleLoop() {
    if(started == 0){
        started = 1;
        print("BrenmaxOS> ");
        buffer_index--;
    }
    drawRectToWindow(4, 0, 0, 305, 300, 0x0000);
    uint16_t currX1 = 0;
    uint16_t currY1 = 0;
    for (int i = 0; i < 37*37; i++) {
        char str[2] = {console_buffer[i], '\0'}; 
        drawStringToWindow(4, str, currX1*8, currY1, 0xFFFF);
        currX1 += 1;
        if(currX1 > 37){
            currY1 += 8;
            currX1 = 0;
            line ++;
        }
    }
    uint8_t code = last_scancode;
    last_scancode = 0;
    if (code == 0) return;
    if (code >= 0x80) return; 
    if (code < 128) {
        if (code == 0x0E) {
            if (buffer_index > 0) {
                buffer_index--;
                console_buffer[buffer_index] = '_'; 
                console_buffer[buffer_index + 1] = ' ';
                inputBufferIndex -= 1;
                inputBuffer[inputBufferIndex] = ' ';
            }
            return;             
        }
    if (code == 0x2A) {
        if (shift_held == 0) {
            shift_held = 1;
        } else {
            shift_held = 0;
        }
    }
    char c;
    if (shift_held) {
        c = scancode_to_ascii_shift[code];
    } else {
        c = scancode_to_ascii[code];
    }
        char foundnot[] = "CMD Not Found:";
        if (code == 0x1C) { 
            System_beep();
            console_buffer[buffer_index] = ' '; 
            ConsoleNewLine();
            
            if (inputBuffer[0] == 'c' &&
            inputBuffer[1] == 'l' &&
            inputBuffer[2] == 's') {
                //cls
                ClearConsole();
                started = 0;
                buffer_index = 0;
                inputBufferIndex = 0;
                inputBuffer[0] = ' ';
            }else if(inputBuffer[0] == 'h' &&
            inputBuffer[1] == 'e' &&
            inputBuffer[2] == 'l' &&
            inputBuffer[3] == 'p'){
                ConsoleNewLine();
                printToConsole(9, "COMMANDS:");
                ConsoleNewLine();
                printToConsole(15, "---------------");
                ConsoleNewLine();
                printToConsole(22, " - cls (clears screen)");
                ConsoleNewLine();
                printToConsole(24, " - help (lists commands)");
                ConsoleNewLine();
                printToConsole(28, " - about (displays OS info)");
                ConsoleNewLine();                    
                printToConsole(24, " - rdsc (Test read sec0)");
                ConsoleNewLine();
                printToConsole(24, " - wdsc (Test writ sec0)");
                ConsoleNewLine();
                printToConsole(21, " - die (crash the pc)");
                ConsoleNewLine();
                printToConsole(29, " - hmtn (Honourable mentions)");
                ConsoleNewLine();
                clearInputBuf();
            }else if(inputBuffer[0] == 'h' &&
            inputBuffer[1] == 'm' &&
            inputBuffer[2] == 't' &&
            inputBuffer[3] == 'n'){
                add_window(6, 20, 20, 300, 200, People);

                ConsoleNewLine();
                clearInputBuf();
            }else if(inputBuffer[0] == 's' &&
            inputBuffer[1] == 't' &&
            inputBuffer[2] == 'r' &&
            inputBuffer[3] == '1'){
                char banana[] = "pengi";
                char banana1[] = "pengi";
                if(strcmp(banana, banana1) == 1){
                    print("Same thing");
                }else{
                    print("diff thing");
                }
                ConsoleNewLine();
                clearInputBuf();
            }else if(inputBuffer[0] == 't' &&
            inputBuffer[1] == 'd' &&
            inputBuffer[2] == 's' &&
            inputBuffer[3] == 'c'){
                //tdsc
                int working = 0;
                uint8_t status = 0xFF;

                if(disk_detectIDE() == 0){
                    printToConsole(17, "IDE Not Supported");                        
                    ConsoleNewLine();
                }else if(disk_detectIDE() == 1){
                    printToConsole(18, "IDE Not Plugged In");                        
                    ConsoleNewLine();
                }else{
                    printToConsole(13, "IDE Supported");                        
                    ConsoleNewLine();
                }
                jiouFS_createFile(512, "justinC");
                jiouFS_createFile(512, "Brenden");
                ConsoleNewLine();
                clearInputBuf();
            }else if(inputBuffer[0] == 'd' &&
            inputBuffer[1] == 'i' &&
            inputBuffer[2] == 'e'){
                System_restart();
            }else if(inputBuffer[0] == 't' &&
            inputBuffer[1] == 'f' &&
            inputBuffer[2] == 's'){
                //op //tfs

                jiouFS_readFile("sixseven", (uint32_t*)0x00770000);
                ConsoleNewLine();
                printToConsole(512, (char*)0x00320000);  

                ConsoleNewLine();
                clearInputBuf();
            }else if(inputBuffer[0] == 'w' &&
            inputBuffer[1] == 'd' &&
            inputBuffer[2] == 's' &&
            inputBuffer[3] == 'c'){
                //wdsc
                print("Writing...");             
                ConsoleNewLine();

                uint16_t test_buffer[256];
                for(int i=0;i<256;i++){test_buffer[i]=0x0000;}

                test_buffer[0]=0x6548;
                test_buffer[1]=0x6C6C;
                test_buffer[2]=0x206F;
                test_buffer[3]=0x7266;
                test_buffer[4]=0x6D6F;
                test_buffer[5]=0x4A20;
                test_buffer[6]=0x6F69;
                test_buffer[7]=0x4675;
                test_buffer[8]=0x0053;
                uint8_t result=disk_writeSector(test_buffer,Origin+1);
                
                if(result == 0){
                    printToConsole(4, "Done");  
                }else{
                    printToConsole(21, "Oh no. Error occured.");  
                }
                ConsoleNewLine();
                clearInputBuf();

            
            }else if(inputBuffer[0] == 'r' &&
            inputBuffer[1] == 'd' &&
            inputBuffer[2] == 's' &&
            inputBuffer[3] == 'c'){
                uint8_t secID = 0;
                char* memory_ptr = (char*)0x00320000;
                uint8_t disk_result = disk_readSector(0x00320000, 202);

                if (disk_result == 0) {
                    ConsoleNewLine();
                    printToConsole(20, "Done ---------------");
                    ConsoleNewLine();
                    printToConsole(512, (char*)0x00320000); 
                } 
                else {
                    ConsoleNewLine();
                    printToConsole(6, "Oh No.");
                    
                    if (disk_result == 1) {
                        printToConsole(17, "IDE Not Supported"); 
                    } else if (disk_result == 2) {
                        printToConsole(22, " Error: Drive Busy   "); 
                    } else if (disk_result == 3) {
                        printToConsole(22, " Error: Drive ERR    "); 
                    } else if (disk_result == 5) {
                        printToConsole(22, " Error: Data Timeout ");
                    }
                }
                ConsoleNewLine();
                clearInputBuf();
            }else if(inputBuffer[0] == 'f' &&
            inputBuffer[1] == 'm' &&
            inputBuffer[2] == 'a' &&
            inputBuffer[3] == 't'){
                //fmat
                printToConsole(10, "Formatting");                        
                ConsoleNewLine();

                jiouFS_FormatDisk();

                ConsoleNewLine();
                clearInputBuf();

            }else if(inputBuffer[0] == 'l' &&
            inputBuffer[1] == 's'){
                //ls
                jiouFS_list();
                clearInputBuf();

            }else if(inputBuffer[0] == 'a' &&
            inputBuffer[1] == 'b' &&
            inputBuffer[2] == 'o' &&
            inputBuffer[3] == 'u' &&
            inputBuffer[4] == 't'){
                ConsoleNewLine();
                printToConsole(14, " B R E N M A X");
                ConsoleNewLine();
                printToConsole(14, "  Corporation");
                ConsoleNewLine();
                printToConsole(16, " ---------------");
                ConsoleNewLine();
                printToConsole(9, " RAM: 4GB");
                ConsoleNewLine();
                printToConsole(16, " CPU: 32bit mode");
                ConsoleNewLine();
                printToConsole(25, " Holiday Season Creations");
                ConsoleNewLine();
                clearInputBuf();
            }else if(inputBuffer[0] == 'f' &&
            inputBuffer[1] == 'm' &&
            inputBuffer[2] == 'a' &&
            inputBuffer[3] == 'r' &&
            inputBuffer[4] == 't'){
                FamilyMart();
                ConsoleNewLine();
                printToConsole(5, "Done.");
                ConsoleNewLine();
                clearInputBuf();
            }else{
                
                for(int i = 0; i < 15; i++){
                    console_buffer[buffer_index] = foundnot[i];
                    console_buffer[buffer_index + 1] = ' ';
                    buffer_index++;
                }

                for(int i = 0; i < 60; i++){
                    console_buffer[buffer_index] = inputBuffer[i];
                    console_buffer[buffer_index + 1] = ' ';
                    buffer_index++;
                    inputBufferIndex++;
                }
                clearInputBuf();
            }
        }   
        // input
        if (c != 0 && buffer_index < 37*37) {
            console_buffer[buffer_index] = c;
            console_buffer[buffer_index + 1] = '_';
            inputBuffer[inputBufferIndex] = c;
            buffer_index++;
            inputBufferIndex++;
        }
    }
}

void printToConsole(uint32_t len, char *text){
    len = strlen(text);
    for(int i = 0; i < len; i++){
        console_buffer[buffer_index] = text[i];
        console_buffer[buffer_index + 1] = ' ';
        buffer_index++;
    }
}

void print(char *text){
    uint32_t len = strlen(text);
    for(int i = 0; i < len; i++){
        console_buffer[buffer_index] = text[i];
        console_buffer[buffer_index + 1] = ' ';
        buffer_index++;
    }
}

void clearInputBuf(){
    for (int i = 0; i < 60; i++) {
        inputBuffer[i] = ' ';
        inputBufferIndex = 1;
    }

    inputBufferIndex = 0;
    ConsoleNewLine();
    print("BrenmaxOS> ");
    buffer_index--;
}

void ConsoleNewLine(){
    buffer_index = ((buffer_index / 37) + 1) * 37;
    buffer_index = buffer_index + 1*(buffer_index / 37);
    if (buffer_index >= 37 * 37) {
        buffer_index = 0; 
    }
}

void openConsole(){
    ClearConsole();
    add_window(4, 10, 10, 305, 300, title3);
}

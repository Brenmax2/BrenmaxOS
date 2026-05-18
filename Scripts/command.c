
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

void Console_setColor(uint8_t color){

}

void Console_clearConsole(){
    for (int i = 0; i < 37*37; i++) {
        console_buffer[i] = ' ';
    }

    for (int i = 0; i < 60; i++) {
        inputBufferIndex = 0;
        inputBuffer[i] = ' ';
    }
}

void Console_loop() {
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
    //btw still using the old printToConsole() instead of new print() because im not bothered adding it all yet.
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
                Console_clearConsole();
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
                //hmtn
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
                jiouFS_createFile(512*2, "justinC", "exe");
                jiouFS_createFile(512, "Jess", "hon");
                ConsoleNewLine();
                clearInputBuf();
            }else if(inputBuffer[0] == 'w' &&
        inputBuffer[1] == 'c' &&
        inputBuffer[2] == 'n' &&
        inputBuffer[3] == 't'){
            //wcnt
            char filename[11];
            int currentPos = 5;

            //file name
            int i = 0;
            while (i < 10) {
                char c = inputBuffer[currentPos];
                if (c == ' ' || c == '\0' || c == '\n') {
                    filename[i] = '\0';
                    break;
                }
                filename[i] = c;
                i++;
                currentPos++;
            }
            filename[i] = '\0';

            if (inputBuffer[currentPos] == ' ') {currentPos++;}

            int contentSize = 0;
            int scanPos = currentPos;
            while (inputBuffer[scanPos] != '\0' && 
                   inputBuffer[scanPos] != '\n' && 
                   inputBuffer[scanPos] != '\r') {
                contentSize++;
                scanPos++;
            }
            
            char content[contentSize + 1]; 

            //cnt
            int j = 0;
            while (j < contentSize) {
                char c = inputBuffer[currentPos];
                if (c == '\0' || c == '\n' || c == '\r') {
                    break;
                }
                content[j] = c;
                j++;
                currentPos++;
            }
            content[j] = '\0';

            jiouFS_writeFile(filename, content, j);
            print("written:");
            print (content);
            ConsoleNewLine();
            clearInputBuf();
        }else if(inputBuffer[0] == 'r' && 
            inputBuffer[1] == 'c' && 
            inputBuffer[2] == 'n' && 
            inputBuffer[3] == 't') {
                //rcnt
                char stringthing[7];

                for (int i = 0; i < 6; i++) {
                    char c = inputBuffer[5 + i];
                    if (c == ' ' || c == '\0' || c == '\n') {
                        if(stringthing[i] == ' ' && i==0){
                            print("Usage: rcnt [name]");
                            ConsoleNewLine();
                            clearInputBuf();
                            return;
                        }
                        stringthing[i] = '\0';
                        break;
                    }
                    stringthing[i] = c;
                    if (i == 5) stringthing[6] = '\0'; 
                }
                print("Reading: ");
                print(stringthing);

                ConsoleNewLine();
                // openTxtView(stringthing);
                // ConsoleNewLine();

                jiouFS_readFile(stringthing, (uint32_t*)0x00770000);
                printToConsole(512*3, (char*)0x00770000);  
                
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

                char testtest[] = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui. Etiam rhoncus. Maecenas tempus, tellus eget condimentum rhoncus, sem quam semper libero, sit amet adipiscing sem neque sed ipsum. Nam quam nunc, blandit vel, luctus pulvinar, hendrerit id, lorem. Maecenas nec odio et ante tincidunt tempus. Donec vitae sapien ut libero venenatis faucibus. Nullam quis ante. Etiam sit amet orci eget eros faucibus tincidunt. Duis leo. Sed fringilla mauris sit amet nibh. Donec sodales sagittis magna. Sed consequat, leo eget bibendum sodales, augue velit cursus nunc, quis gravida magna mi a libero. Fusce vulputate eleifend sapien. Vestibulum purus quam, scelerisque ut, mollis sed, nonummy id, met";
                jiouFS_createFile(1500, "long\0", "txt");
                jiouFS_writeFile("long", testtest, 1500);
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

                test_buffer[0]=0x6162;
                test_buffer[1]=0x616E;
                test_buffer[2]=0x616E;
                test_buffer[3]=0x616D;
                test_buffer[4]=0x006E;
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
                        print("Oh no. Drive crashed out...");
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

            } else if(inputBuffer[0] == 'm' && 
            inputBuffer[1] == 'k' && 
            inputBuffer[2] == 'f' && 
            inputBuffer[3] == 'l') {
                //mkfl
                char stringthing[7];

                for (int i = 0; i < 6; i++) {
                    char c = inputBuffer[5 + i];
                    if (c == ' ' || c == '\0' || c == '\n') {
                        if(stringthing[i] == ' ' && i==0){
                            print("Usage: mkfl [name]");
                            ConsoleNewLine();
                            clearInputBuf();
                            return;
                        }
                        stringthing[i] = '\0';
                        break;
                    }
                    stringthing[i] = c;
                    if (i == 5) stringthing[6] = '\0'; 
                }

                jiouFS_createFile(1000, stringthing, "txt");
                print(" Created.");
                ConsoleNewLine();
                clearInputBuf();
            }else if(inputBuffer[0] == 'r' && 
            inputBuffer[1] == 'n' && 
            inputBuffer[2] == 'f' && 
            inputBuffer[3] == 'l') {
                //rnfl
                char oldName[11];
                char newName[11];
                int currentPos = 5;

                // old name
                int i = 0;
                while (i < 10) {
                    char c = inputBuffer[currentPos];
                    if (c == ' ' || c == '\0' || c == '\n') {
                        oldName[i] = '\0';
                        break;
                    }
                    oldName[i] = c;
                    i++;
                    currentPos++;
                }
                oldName[i] = '\0';

                if (inputBuffer[currentPos] == ' ') {currentPos++;}

                // new name
                int j = 0;
                while (j < 10) {
                    char c = inputBuffer[currentPos];
                    if (c == ' ' || c == '\0' || c == '\n' || c == '\r') {
                        newName[j] = '\0';
                        break;
                    }
                    newName[j] = c;
                    j++;
                    currentPos++;
                }
                newName[j] = '\0';

                //rename try
                if (oldName[0] != '\0' && newName[0] != '\0') {
                    uint8_t res = jiouFS_renameFile(oldName, newName);
                    if (res == 0) {
                        print("Rename Successful");
                    } else if (res == 2) {
                        print("Oh no. File not found.");
                    } else {
                        print("Oh No.");
                    }
                } else {
                    print("Usage: rnfl [old] [new]");
                }

                ConsoleNewLine();
                clearInputBuf();
            } else if(inputBuffer[0] == 'd' && 
            inputBuffer[1] == 'e' && 
            inputBuffer[2] == 'f' && 
            inputBuffer[3] == 'l') {
                //defl
                char stringthing[7];

                for (int i = 0; i < 6; i++) {
                    char c = inputBuffer[5 + i];
                    if (c == ' ' || c == '\0' || c == '\n') {
                        stringthing[i] = '\0';
                        break;
                    }
                    stringthing[i] = c;
                    if (i == 5) stringthing[6] = '\0'; 
                }

                if(jiouFS_deleteFile(stringthing) == 0){
                    print("Deleted file.");
                }
                ConsoleNewLine();
                clearInputBuf();
            }else if(inputBuffer[0] == 'l' &&
            inputBuffer[1] == 's'){
                //ls
                jiouFS_list();
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
    Console_clearConsole();
    add_window(4, 10, 10, 305, 300, title3);
}

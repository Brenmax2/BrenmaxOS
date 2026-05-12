#include "headers/window.h"
#include "headers/console.h"
#include "headers/disk.h"
#include "headers/input.h"
#include "headers/misc.h"
#include "headers/gui.h"
#include "headers/globals.h"
#include <stdint.h>

void drawStringToWindow(uint8_t id, char *text, uint16_t x, uint16_t y, uint16_t color) {
    int idx = -1;

    for (int j = 0; j < window_count; j++) {
        if ((uint8_t)windows[j][4] == id) {
            idx = j;
            break;
        }
    }

    if (idx == -1 || text == 0) return;
    uint16_t top  = windows[idx][0];
    uint16_t left = windows[idx][1];
    int column = 0;
    int row_offset = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];

        if (c == '\n') {
            column = 0;
            row_offset += 9;
            continue;
        }

        uint16_t draw_x = left + 4 + x + (column * 8);
        uint16_t draw_y = top + 16 + y + row_offset;
        drawChar_THIN(draw_x, draw_y, text[i], color);
        column++;
    }
}

void drawStringToWindowBIG(uint8_t id, char *text, uint16_t x, uint16_t y, uint16_t color) {
    int idx = -1;
    for (int j = 0; j < window_count; j++) {
        if ((uint8_t)windows[j][4] == id) {
            idx = j;
            break;
        }
    }

    if (idx == -1 || text == 0) return;
    uint16_t top  = windows[idx][0];
    uint16_t left = windows[idx][1];
    int column = 0;
    int row_offset = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];

        if (c == '\n') {
            column = 0;
            row_offset += 9;
            continue;
        }

        uint16_t draw_x = left + 3 + x + (column * 16);
        uint16_t draw_y = top + 16 + y + row_offset;
        drawChar_THIN(draw_x, draw_y, text[i], 0xFFFF);
    }
}

void drawString(char *text, uint16_t x, uint16_t y, uint16_t color) {
    int column = 0;
    int row_offset = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];

        if (c == '\n') {
            column = 0;
            row_offset += 9;
            continue;
        }

        uint16_t draw_x = x + (column * 8);
        uint16_t draw_y = y + row_offset;
        drawChar_THIN(draw_x, draw_y, text[i], 0xFFFF);
        column++;
    }
}


void drawString1(char *text, uint16_t x, uint16_t y, uint16_t color) {
    int column = 0;
    int row_offset = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];

        if (c == '\n') {
            column = 0;
            row_offset += 9;
            continue;
        }

        uint16_t draw_x = x + (column * 8);
        uint16_t draw_y = y + row_offset;
        drawChar(draw_x, draw_y, text[i], 0xFFFF);
        column++;
    }
}

void drawChar_THIN(int x, int y, unsigned char c, int color) {
    for (int row = 0; row < 8; row++) {
        unsigned char rowData = font8x8_THIN[c][row];
        for (int col = 0; col < 8; col++) {
            if (rowData & (0x80 >> col)) {
                drawPixel(x + col, y + row, color);
            }
        }
    }
}

void drawChar(int x, int y, unsigned char c, int color) {
    if (c > 127) return;
    for (int row = 0; row < 16; row++) {
        uint16_t rowData = font8x16[c][row];
        for (int col = 0; col < 16; col++) {
            if (rowData & (0x8000 >> col)) {
                drawPixel(x + col, y + row, color);
            }
        }
    }
}

void drawPixelToWindow(uint8_t id, uint16_t x, uint16_t y, uint16_t color) {
    int idx = -1;

    for (int i = 0; i < window_count; i++) {
        if ((uint8_t)windows[i][4] == id) {
            idx = i;
            break;
        }
    }

    if (idx == -1) return;
    uint16_t win_top    = windows[idx][0];
    uint16_t win_left   = windows[idx][1];
    uint16_t win_width  = windows[idx][2];
    uint16_t win_height = windows[idx][3];

    if (x < win_width && y < win_height) {
        drawPixel(win_left + 4 + x, win_top + 16 + y, color);
    }
}

void drawStringToWindowTitle(uint8_t id, char *text, uint16_t x, uint16_t y, uint16_t color) {
    int idx = -1;

    for (int j = 0; j < window_count; j++) {
        if ((uint8_t)windows[j][4] == id) {
            idx = j;
            break;
        }
    }

    uint16_t top  = windows[idx][0];
    uint16_t left = windows[idx][1];
    uint16_t current_rel_x = x;
    uint16_t current_rel_y = y;

    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];

        if (c == '\n') {
            current_rel_x = x;    
            current_rel_y += 8;   
            continue;               
        }

        uint16_t draw_x = left + 4 + current_rel_x;
        uint16_t draw_y = top + 4 + current_rel_y;
        drawChar_THIN(draw_x, draw_y, text[i], 0xFFFF);
        current_rel_x += 8;
    }
}

void drawMouse(int x_offset, int y_offset) {
    for (int y = 0; y < 16; y+=1) {
        for (int x = 0; x < 16; x+=1) {
            unsigned short color = mouse[y * 16 + x];

            if (color != 0x0000) {
                if(mouseDown != 0){
                    drawPixel(x_offset + x, y_offset + y, color);
                }else{
                    drawPixel(x_offset + x, y_offset + y, color);
                }
        }
        }
    }
}

void drawDesktop() {
    if(Wallpaper == 0){
        int g1 = 66, b1 = 116;
        int g2 = 23, b2 = 70;

        int dg = g2 - g1;
        int db = b2 - b1;

        for (uint32_t y = 0; y < 768; y++) {
            for (uint32_t x = 0; x < 1024; x++) {
                int t = ((y * 3) - (x * 2) + 2048) / 16; 
                
                if (t < 0) t = 0;
                if (t > 255) t = 255;

                int currG = g1 + (t * dg / 255);
                int currB = b1 + (t * db / 255);

                uint16_t g_bits = (currG * 63) / 255;
                uint16_t b_bits = (currB * 31) / 255;

                disp[y * 1024 + x] = (g_bits << 5) | b_bits;
            }
        }
    }else{
        for (uint32_t y = 0; y < 768; y++) {
            for (uint32_t x = 0; x < 1024; x++) {
                uint16_t r = 31 - (x * 31 / 1023);
                uint16_t g = 0;
                uint16_t b = 31;
                uint16_t color = (r << 11) | (g << 5) | b;
                disp[y * 1024 + x] = color;
            }
        }
    }
}

void drawDesktop1() {
    for (uint32_t y = 0; y < 768; y++) {
        for (uint32_t x = 0; x < 1024; x++) {
            uint16_t r = 31 - (x * 31 / 1023);
            uint16_t g = 0;
            uint16_t b = 31;
            uint16_t color = (r << 11) | (g << 5) | b;
            disp[y * 1024 + x] = color;
        }
    }
}

void drawPixel(uint16_t x, uint16_t y, uint16_t color){
    if(x >= 1024 || y >= 768){return;}
    uint32_t fb_addr = *(volatile uint32_t *)0x7028;
    uint16_t *videoBuffer = (uint16_t *)fb_addr;
    uint32_t where = x + y * 1024;
    disp[where] = color;
}

void draw_Rect(uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color) {
    uint32_t fb_addr = *(volatile uint32_t *)0x7028;
    uint16_t *videoBuffer = (uint16_t *)fb_addr;

    for (uint32_t y = 0; y < h; y++) {
        for (uint32_t x = 0; x < w; x++) {
            drawPixel(l + x, t + y, color);
        }
    }
}

void draw_RectTRANS(uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color, uint16_t multer) {
    if(transUI){
        for (uint32_t y = 0; y < h; y++) {
            for (uint32_t x = 0; x < w; x++) {
                uint16_t bg = disp[(x + l) + (y + t) * 1024];

                uint16_t r = ((color >> 11) + (bg >> 11)) / multer;
                uint16_t g = (((color >> 5) & 0x3F) + ((bg >> 5) & 0x3F)) / multer;
                uint16_t b = ((color & 0x1F) + (bg & 0x1F)) / multer;

                uint16_t colorNEW = (r << 11) | (g << 5) | b;
                drawPixel(l + x, t + y, colorNEW);
            }
        }
    }else{
        draw_Rect(t, l, w, h, color);
    }
}

void drawButtonToWindow(uint8_t id, uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color, char *text, uint16_t Buttid) {
    int idx = -1;
    
    for (int j = 0; j < window_count; j++) {
        if ((uint8_t)windows[j][4] == id) {
            idx = j;
            break;
        }
    }

    if (idx == -1) return;
    uint16_t abs_l = windows[idx][1] + 10;
    uint16_t abs_r = abs_l + w;
    uint16_t abs_t = windows[idx][0] + t + 15;
    uint16_t abs_b = abs_t + h;

    drawRectToWindow(id, t - 1, l - 1, w + 2, h + 2, 0xA514);

    if (mouse_x >= abs_l && mouse_x <= abs_r && mouse_y >= abs_t && mouse_y <= abs_b) {
        if(mouseDown){
            drawRectToWindow(id, t, l, w, h, 0xC638);
            buttonStates[Buttid] = 1;
        }else{
            drawRectToWindow(id, t, l, w, h, 0x6B6D);
            buttonStates[Buttid] = 0;
        }
    } else {
        drawRectToWindow(id, t, l, w, h, 0x3186);
    }

    drawStringToWindow(id, text, l + 3, t + 3, color);
}

void drawButton(uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color, char *text, uint16_t Buttid) {
    uint16_t abs_l = l;
    uint16_t abs_r = l + w;
    uint16_t abs_t = t;
    uint16_t abs_b = t + h;

    draw_Rect(t - 1, l - 1, w + 2, h + 2, 0xA514);
    
    if (!mouseDown) done = 0;

    if (mouse_x >= abs_l && mouse_x <= abs_r && mouse_y >= abs_t && mouse_y <= abs_b) {
        if (mouseDown && done == 0) {
            draw_Rect(t, l, w, h, 0xC638);
            buttonStates[Buttid] = 1;
            done = 1; 
        } else if (mouseDown && done == 1) {
            draw_Rect(t, l, w, h, 0xC638);
        } else {
            draw_Rect(t, l, w, h, 0x6B6D);
            buttonStates[Buttid] = 0;
        }
    } else {
        draw_Rect(t, l, w, h, 0x3186);
        buttonStates[Buttid] = 0;
    }

    drawString(text, abs_l + 3, abs_t + 3, 0xFFFF);
}

void drawbuttonPRO(uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color, char *text, uint16_t Buttid, uint8_t offXT, uint8_t offYT) {
    uint16_t abs_l = l;
    uint16_t abs_r = l + w;
    uint16_t abs_t = t;
    uint16_t abs_b = t + h;

    draw_Rect(t - 1, l - 1, w + 2, h + 2, 0xA514);
    
    if (!mouseDown) done = 0;

    if (mouse_x >= abs_l && mouse_x <= abs_r && mouse_y >= abs_t && mouse_y <= abs_b) {
        if (mouseDown && done == 0) {
            draw_Rect(t, l, w, h, 0xC638);
            buttonStates[Buttid] = 1;
            done = 1; 
        } else if (mouseDown && done == 1) {
            draw_Rect(t, l, w, h, 0xC638);
        } else {
            draw_Rect(t, l, w, h, 0x6B6D);
            buttonStates[Buttid] = 0;
        }
    } else {
        draw_Rect(t, l, w, h, 0x3186);
        buttonStates[Buttid] = 0;
    }

    drawString(text, abs_l + offXT, abs_t + offYT, 0xFFFF);
}

void drawRectToWindow(uint8_t id, uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color) {
    int idx = -1;
    l = l + 4;
    for (int j = 0; j < window_count; j++) {
        if ((uint8_t)windows[j][4] == id) {
            idx = j;
            break;
        }
    }

    if (idx == -1) return;

    uint16_t window_top  = windows[idx][0];
    uint16_t window_left = windows[idx][1];

    for (uint32_t y = 0; y < h; y++) {
        for (uint32_t x = 0; x < w; x++) {
            drawPixel(window_left + l + x, window_top + 16 + t + y, color);
        }
    }
}

void swapBuffers() {
    uint32_t fb_addr = *(volatile uint32_t *)0x7028;
    uint16_t *videoBuffer = (uint16_t *)fb_addr;
    
    for (uint32_t i = 0; i < (1024 * 768); i++) {
        videoBuffer[i] = disp[i];
    }
}

void drawAnimation() {
    for (int i = 0; i < 10; i++) {
        draw_Rect(by[i], bx[i], 20, 20, 0x0000);
        bx[i] += bdx[i];
        by[i] += bdy[i];
        if (bx[i] <= 0 || bx[i] >= (1024 - 10)) {
            bdx[i] = -bdx[i];
            bcol[i] += 0x1234; 
        }

        if (by[i] <= 0 || by[i] >= (738 - 10)) {
            bdy[i] = -bdy[i];
            bcol[i] += 0x4321;
        }
        draw_Rect(by[i] + 2, bx[i] + 2, 20, 20, bcol[i]);
    }
}


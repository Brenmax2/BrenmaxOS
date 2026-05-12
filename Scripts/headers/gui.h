#pragma once
#include <stdint.h>
void drawDesktop();
void drawAnimation();
void drawChar_THIN(int x, int y, unsigned char c, int color);
void drawChar(int x, int y, unsigned char c, int color);
void drawPixel(uint16_t x, uint16_t y, uint16_t color);
void draw_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void draw_RectTRANS(uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color, uint16_t multer);
void drawMouse(int x_offset, int y_offset);
void drawButton(uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color, char *text, uint16_t Buttid);
void drawbuttonPRO(uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color, char *text, uint16_t Buttid, uint8_t offXT, uint8_t offYT);
void drawString(char *text, uint16_t x, uint16_t y, uint16_t color);
void drawString1(char *text, uint16_t x, uint16_t y, uint16_t color);
void swapBuffers();
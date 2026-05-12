#pragma once
#include <stdint.h>
void add_window(uint8_t id, uint16_t t, uint16_t l, uint16_t w, uint16_t h, char *title);
void drawAllWindows();
void handleWindowDragging();
void bring_to_front(int idx);
void closeTopWindow();
void drawCharToWindow(uint8_t id, const uint8_t glyph[8], uint16_t x, uint16_t y, uint16_t color);
void drawStringToWindowTitle(uint8_t id, char *text, uint16_t x, uint16_t y, uint16_t color);
void drawStringToWindow(uint8_t id, char *text, uint16_t x, uint16_t y, uint16_t color);
void drawStringToWindowBIG(uint8_t id, char *text, uint16_t x, uint16_t y, uint16_t color);
void drawPixelToWindow(uint8_t id, uint16_t x, uint16_t y, uint16_t color);
void drawRectToWindow(uint8_t id, uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color);
void drawButtonToWindow(uint8_t id, uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color, char *text, uint16_t Buttid);
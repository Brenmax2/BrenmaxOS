#include "headers/window.h"
#include "headers/console.h"
#include "headers/disk.h"
#include "headers/input.h"
#include "headers/misc.h"
#include "headers/gui.h"
#include "headers/globals.h"
#include <stdint.h>

void add_window(uint8_t id, uint16_t t, uint16_t l, uint16_t w, uint16_t h, char *title) {
    if (window_count < 100) {
        windows[window_count][0] = t;
        windows[window_count][1] = l;
        windows[window_count][2] = w;
        windows[window_count][3] = h;
        windows[window_count][4] = id;
        window_title_ptrs[window_count] = title;
        window_count++;
    }
}

void bring_to_front(int idx) { 
    if (idx < 0 || idx >= window_count) return;
    if (idx == window_count - 1) return;

    uint16_t temp0 = windows[idx][0];
    uint16_t temp1 = windows[idx][1];
    uint16_t temp2 = windows[idx][2];
    uint16_t temp3 = windows[idx][3];
    uint16_t temp4 = windows[idx][4];
    char *temp_title = window_title_ptrs[idx];

    for (int i = idx; i < window_count - 1; i++) {
        windows[i][0] = windows[i + 1][0];
        windows[i][1] = windows[i + 1][1];
        windows[i][2] = windows[i + 1][2];
        windows[i][3] = windows[i + 1][3];
        windows[i][4] = windows[i + 1][4];
        window_title_ptrs[i] = window_title_ptrs[i + 1];
    }

    windows[window_count - 1][0] = temp0;
    windows[window_count - 1][1] = temp1;
    windows[window_count - 1][2] = temp2;
    windows[window_count - 1][3] = temp3;
    windows[window_count - 1][4] = temp4;
    window_title_ptrs[window_count - 1] = temp_title;
}

void handleWindowDragging() {
    if (mouseDown && !prev_mouseDown) {
        active_window = -1;

        for (int i = window_count - 1; i >= 0; i--) {
            uint16_t top    = windows[i][0];
            uint16_t left   = windows[i][1];
            uint16_t width  = windows[i][2];
            uint16_t height = windows[i][3];

            if (mouse_x >= left &&
                mouse_x < left + width + 8 &&
                mouse_y >= top &&
                mouse_y < top + 16) {
                drag_offset_x = mouse_x - left;
                drag_offset_y = mouse_y - top;
                bring_to_front(i);
                active_window = window_count - 1;
                break;
            }
        }
    }

    if (mouseDown && active_window != -1) {
        int new_left = mouse_x - drag_offset_x;
        int new_top  = mouse_y - drag_offset_y;
        uint16_t width  = windows[active_window][2];
        uint16_t height = windows[active_window][3];

        if (new_left < 0) new_left = 0;
        if (new_top < 0) new_top = 0;
        if (new_left > 1024 - (width + 12)) new_left = 1024 - (width + 12);
        if (new_top > 768 - (height + 20)) new_top = 768 - (height + 20);
        windows[active_window][1] = (uint16_t)new_left;
        windows[active_window][0] = (uint16_t)new_top;
    }

    if (!mouseDown) {
        active_window = -1;
    }

    prev_mouseDown = mouseDown;
}

void closeTopWindow() {
    if (window_count > 0) {
        int topIdx = window_count - 1;
        window_title_ptrs[topIdx] = 0; 
        window_count--;
    }
}

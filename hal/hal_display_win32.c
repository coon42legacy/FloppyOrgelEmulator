#include <windows.h>
#include "../common/config.h"
#include "hal_display.h"

extern HDC g_hDisplayDC;
extern HWND g_hEmuWnd;

void display_clear(uint8_t red, uint8_t green, uint8_t blue) {
  for (int y = 0; y < DISPLAY_RESOLUTION_Y; y++)
    for (int x = 0; x < DISPLAY_RESOLUTION_X; x++)
      display_setPixel(x, y, red, green, blue);
}

void display_setPixel(uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue) {
  SetPixel(g_hDisplayDC, x, y, RGB(red, green, blue));
}

void display_redraw() {
  RedrawWindow(g_hEmuWnd, NULL, NULL, RDW_INVALIDATE);
}
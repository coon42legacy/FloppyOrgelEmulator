#include <windows.h>
#include "config.h"
#include "hal_gui.h"

extern HDC hDisplayDC;
extern HWND hEmuWnd;

void gui_clear(uint8_t red, uint8_t green, uint8_t blue) {
  for (int y = 0; y < DISPLAY_RESOLUTION_Y; y++) {
    for (int x = 0; x < DISPLAY_RESOLUTION_X; x++) {
      gui_setPixel(x, y, red, green, blue);
    }
  }

  RedrawWindow(hEmuWnd, NULL, NULL, RDW_INVALIDATE);
}

void gui_setPixel(uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue) {
  SetPixel(hDisplayDC, x, y, RGB(red, green, blue));
}

#include <stdint.h>
#include "hal_display.h"
#include "LCD/SSD1289.h"

#define RGB565(R,G,B) ((uint16_t)((((R) / 8) << 11) + (((G) / 4) << 5) + ((B) / 8)))

void display_setPixel(uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue) {
  SSD1289_SetPoint(x, y, RGB565(red, green, blue)); // TODO: calc 8:8:8 color to 5:6:5 color
}

void display_redraw() {
  // TODO: redraw here!
}

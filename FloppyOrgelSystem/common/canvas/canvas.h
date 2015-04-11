#include <stdlib.h>

void canvas_clear(uint8_t red, uint8_t green, uint8_t blue);
void canvas_drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t red, uint8_t green, uint8_t blue);
void canvas_textFont(uint16_t x, uint16_t y, char* str,
  uint8_t txtRed, uint8_t txtGreen, uint8_t txtBlue,
  uint8_t bkRed, uint8_t bkGreen, uint8_t bkBlue,
  uint16_t FONTx);
void canvas_drawText(uint16_t x, uint16_t y, char* str,
  uint8_t txtRed, uint8_t txtGreen, uint8_t txtBlue,
  uint8_t bkRed, uint8_t bkGreen, uint8_t bkBlue);
void canvas_drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t red, uint8_t green, uint8_t blue);

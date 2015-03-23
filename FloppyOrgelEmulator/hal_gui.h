#ifndef __HAL_GUI_H
#define __HAL_GUI_H

#include <stdint.h>

void gui_setPixel(uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue);
void gui_clear(uint8_t red, uint8_t green, uint8_t blue);
void gui_drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t red, uint8_t green, uint8_t blue);
void gui_drawText(uint16_t x, uint16_t y, uint8_t *str, 
  uint8_t txtRed, uint8_t txtGreen, uint8_t txtBlue, 
  uint8_t bkRed, uint8_t bkGreen, uint8_t bkBlue);
void gui_drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t red, uint8_t green, uint8_t blue);
void gui_redraw();

#endif
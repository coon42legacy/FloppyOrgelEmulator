#ifndef __CANVAS_H
#define __CANVAS_H

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "images.h"

#define CENTER USHRT_MAX

void canvas_clear(uint8_t red, uint8_t green, uint8_t blue);
void canvas_drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t red, uint8_t green, uint8_t blue);
void canvas_textFont(uint16_t x, uint16_t y, const char* str, uint8_t txtRed, uint8_t txtGreen, uint8_t txtBlue,
  uint8_t bgRed, uint8_t bgGreen, uint8_t bgBlue, uint16_t FONTx);
void canvas_drawText(uint16_t x, uint16_t y, const char* str, uint8_t txtRed, uint8_t txtGreen, uint8_t txtBlue,
  uint8_t bgRed, uint8_t bgGreen, uint8_t bgBlue);

void canvas_drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t red, uint8_t green, uint8_t blue);
void canvas_drawImage(uint16_t xPos, uint16_t yPos, const uint8_t* pImg);
#endif  // __CANVAS_H

#ifndef __HAL_DISPLAY_H
#define __HAL_DISPLAY_H

#include <stdint.h>

void display_clear(uint8_t red, uint8_t green, uint8_t blue);
void display_setPixel(uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue);
void display_redraw();

#endif

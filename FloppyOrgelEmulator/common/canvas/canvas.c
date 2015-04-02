#include "../../hal/hal_display.h"
#include "../AsciiLib/AsciiLib.h"
#include "../config.h"
#include "canvas.h"

void canvas_putCharFont(uint16_t x, uint16_t y, uint8_t ASCII,
  uint8_t txtRed, uint8_t txtGreen, uint8_t txtBlue,
  uint8_t bkRed, uint8_t bkGreen, uint8_t bkBlue,
  uint16_t FONTx) {
  uint16_t i, j;
  uint8_t buffer[16], tmp_char;
  uint8_t len_x, len_y;

  switch (FONTx) {
  case FONT6x8:
    len_x = 6;
    len_y = 8;
    break;
  case FONT8x8:
    len_x = 8;
    len_y = 8;
    break;
  case MS_GOTHIC_8x16:
  case SYSTEM_8x16:
  default:
    len_x = 8;
    len_y = 16;
    break;
  }

  getASCIICode(buffer, ASCII, FONTx);
  for (i = 0; i < len_y; i++) {
    tmp_char = buffer[i];
    for (j = 0; j<len_x; j++) {
      if (((tmp_char >> (7 - j)) & 0x01) == 0x01)
        display_setPixel(x + j, y + i, txtRed, txtGreen, txtBlue);
      else
        display_setPixel(x + j, y + i, bkRed, bkGreen, bkBlue);
    }
  }
}

void canvas_textFont(uint16_t x, uint16_t y, uint8_t *str,
  uint8_t txtRed, uint8_t txtGreen, uint8_t txtBlue,
  uint8_t bkRed, uint8_t bkGreen, uint8_t bkBlue,
  uint16_t FONTx) {
  uint8_t TempChar;
  uint8_t delta_x, delta_y;

  switch (FONTx) {
  case FONT6x8:
    delta_x = 6;
    delta_y = 8;
    break;
  case FONT8x8:
    delta_x = 8;
    delta_y = 8;
    break;
  case MS_GOTHIC_8x16:
  case SYSTEM_8x16:
  default:
    delta_x = 8;
    delta_y = 16;
    break;
  }

  do {
    TempChar = *str++;
    canvas_putCharFont(x, y, TempChar,
      txtRed, txtGreen, txtBlue,
      bkRed, bkGreen, bkBlue, FONTx);
    if (x < DISPLAY_RESOLUTION_X - delta_x) {
      x += delta_x;
    }
    else if (y < DISPLAY_RESOLUTION_Y - delta_y) {
      x = 0;
      y += delta_y;
    }
    else {
      x = 0;
      y = 0;
    }
  } while (*str != 0);
}

void canvas_drawText(uint16_t x, uint16_t y, uint8_t *str,
  uint8_t txtRed, uint8_t txtGreen, uint8_t txtBlue,
  uint8_t bkRed, uint8_t bkGreen, uint8_t bkBlue) {
  canvas_textFont(x, y, str, txtRed, txtGreen, txtBlue, bkRed, bkGreen, bkBlue, SYSTEM_8x16);
}

void canvas_clear(uint8_t red, uint8_t green, uint8_t blue) {
  for (int y = 0; y < DISPLAY_RESOLUTION_Y; y++) {
    for (int x = 0; x < DISPLAY_RESOLUTION_X; x++) {
      display_setPixel(x, y, red, green, blue);
    }
  }
}

void canvas_drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t red, uint8_t green, uint8_t blue) {
  int delta_x = x2 - x1;
  // if x1 == x2, then it does not matter what we set here
  signed char const ix = (delta_x > 0) - (delta_x < 0);
  delta_x = abs(delta_x) << 1;

  int delta_y = y2 - y1;
  // if y1 == y2, then it does not matter what we set here
  signed char const iy = (delta_y > 0) - (delta_y < 0);
  delta_y = abs(delta_y) << 1;
  display_setPixel(x1, y1, red, green, blue);

  if (delta_x >= delta_y) {
    // error may go below zero
    int error = delta_y - (delta_x >> 1);

    while (x1 != x2){
      if ((error >= 0) && (error || (ix > 0))) {
        error -= delta_x;
        y1 += iy;
      }
      // else do nothing

      error += delta_y;
      x1 += ix;
      display_setPixel(x1, y1, red, green, blue);
    }
  }
  else {
    // error may go below zero
    int error = delta_x - (delta_y >> 1);

    while (y1 != y2) {
      if ((error >= 0) && (error || (iy > 0))) {
        error -= delta_y;
        x1 += ix;
      }
      // else do nothing

      error += delta_x;
      y1 += iy;

      display_setPixel(x1, y1, red, green, blue);
    }
  }
}

void canvas_drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
  uint8_t red, uint8_t green, uint8_t blue) {
  if (x > DISPLAY_RESOLUTION_X)
    x = DISPLAY_RESOLUTION_X;
  if (y > DISPLAY_RESOLUTION_Y)
    y = DISPLAY_RESOLUTION_Y;

  if ((x + w) > DISPLAY_RESOLUTION_X)
    w = DISPLAY_RESOLUTION_X - x;

  if ((y + h) > DISPLAY_RESOLUTION_Y)
    h = DISPLAY_RESOLUTION_Y - y;

  canvas_drawLine(x, y, x, y + h, red, green, blue);
  canvas_drawLine(x, y, x + w, y, red, green, blue);
  canvas_drawLine(x + w, y + h, x, y + h, red, green, blue);
  canvas_drawLine(x + w, y + h, x + w, y, red, green, blue);
}
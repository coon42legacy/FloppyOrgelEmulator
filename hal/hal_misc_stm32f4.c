#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "stm32f4xx.h"
#include "hal_misc.h"

uint32_t hal_clock() {
  return TIM5->CNT / 1000;
}

static void _print(const char* text) {
  printf("%s\n\r", text);
}

void hal_printf(char* format, ...) {
  char formattedText[512];

  va_list args;
  va_start(args, format);
  vsnprintf(formattedText, sizeof(formattedText), format, args);
  _print(formattedText);
  va_end(args);
}

void hal_printfError(const char* format, ...) {
  char formattedText[512];

  va_list args;
  va_start(args, format);
  vsnprintf(formattedText, sizeof(formattedText), format, args);
  _print(formattedText);
  va_end(args);
}

void hal_printfWarning(char* format, ...) {
  char formattedText[512];

  va_list args;
  va_start(args, format);
  vsnprintf(formattedText, sizeof(formattedText), format, args);
  _print(formattedText);
  va_end(args);
}

void hal_printfSuccess(char* format, ...) {
  char formattedText[512];

  va_list args;
  va_start(args, format);
  vsnprintf(formattedText, sizeof(formattedText), format, args);
  _print(formattedText);
  va_end(args);
}

void hal_printfInfo(char* format, ...) {
  char formattedText[512];

  va_list args;
  va_start(args, format);
  vsnprintf(formattedText, sizeof(formattedText), format, args);
  _print(formattedText);
  va_end(args);
}

void hal_strcpy_s(char* dst, int maxSize, const char* src) {
  int srcLen = strlen(src);
  int len = srcLen > maxSize - 1 ? maxSize - 1 : srcLen;

  memcpy(dst, src, len);
  dst[len + 1] = '\0';
}







#include "hal_misc.h"
#include <stdint.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>

uint32_t hal_clock() {
  return clock();
}

static void _printColored(const char* text, uint16_t colorAttributes) {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
  WORD saved_attributes;

  GetConsoleScreenBufferInfo(hConsole, &consoleInfo); // Save current font color
  saved_attributes = consoleInfo.wAttributes;
  SetConsoleTextAttribute(hConsole, colorAttributes); // Change font color
  printf(text);
  SetConsoleTextAttribute(hConsole, saved_attributes); // Restore original font color
  printf("\n\r");
}

void hal_printf(char* format, ...) {
  char formattedText[512];

  va_list args;
  va_start(args, format);
  vsnprintf_s(formattedText, sizeof(formattedText), sizeof(formattedText), format, args);
  _printColored(formattedText, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
  va_end(args);
}

void hal_printfError(const char* format, ...) {
  char formattedText[512];

  va_list args;
  va_start(args, format);
  vsnprintf_s(formattedText, sizeof(formattedText), sizeof(formattedText), format, args);
  _printColored(formattedText, FOREGROUND_RED | FOREGROUND_INTENSITY);
  va_end(args);
}

void hal_printfWarning(char* format, ...) {
  char formattedText[512];

  va_list args;
  va_start(args, format);
  vsnprintf_s(formattedText, sizeof(formattedText), sizeof(formattedText), format, args);
  _printColored(formattedText, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_INTENSITY); // Yellow
  va_end(args);
}

void hal_printfSuccess(char* format, ...) {
  char formattedText[512];

  va_list args;
  va_start(args, format);
  vsnprintf_s(formattedText, sizeof(formattedText), sizeof(formattedText), format, args);
  _printColored(formattedText, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
  va_end(args);
}

void hal_printfInfo(char* format, ...) {
  char formattedText[512];

  va_list args;
  va_start(args, format);
  vsnprintf_s(formattedText, sizeof(formattedText), sizeof(formattedText), format, args);
  _printColored(formattedText, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
  va_end(args);
}

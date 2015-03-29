#include <stdint.h>
#include "../hal/hal_filesystem.h"
#include "../hal/hal_display.h"
#include "menu.h"

void getFileNameFromCursorPos(char* srcPath, char* dstFilePath, int cursorPos) {
  FO_FIND_DATA findData;

  strcpy_s(dstFilePath, 256, srcPath);
  strcat_s(dstFilePath, 256, "\\*");

  BOOL endOfDirectory = !fo_findInit(dstFilePath, &findData);
  static const uint32_t X_OFFSET = 35;
  static const uint32_t Y_OFFSET = 40;
  int itemCount = 0;

  while (!endOfDirectory) {
    if (findData.fileName[0] != '.')
      if (findData.fileName[1] != '.')
        if (itemCount++ == cursorPos) {
          dstFilePath[strlen(dstFilePath) - 1] = '\0'; // remove trailing start
          strcat_s(dstFilePath, 256, findData.fileName);
          break;
        }

        if (!fo_findNext(&findData))
          endOfDirectory = TRUE;
  }

  fo_findFree();
}

int drawTracks(char* path) {
  FO_FIND_DATA findData;

  static const uint32_t X_OFFSET = 35;
  static const uint32_t Y_OFFSET = 40;
  int itemCount = 0;
  BOOL endOfDirectory = !fo_findInit(path, &findData);

  while (!endOfDirectory) {
    if (findData.fileName[0] != '.')
      if (findData.fileName[1] != '.') {
        display_drawText(X_OFFSET, Y_OFFSET + 18 * itemCount++, findData.fileName,
          0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
      }

      if (!fo_findNext(&findData))
        endOfDirectory = TRUE;
  }
  fo_findFree();

  return 0;
}

void drawCursor(uint32_t cursorPos) {
  const uint32_t X_OFFSET = 25;
  const uint32_t Y_OFFSET = 45;

  display_drawRect(X_OFFSET, Y_OFFSET + 18 * cursorPos, 5, 5, 255, 255, 255);
  const uint32_t LINE_OFFSET = 18;

  uint32_t x[3] = {
    X_OFFSET,      // links oben
    X_OFFSET + 50, // rechts mitte
    X_OFFSET };    // links unten

  uint32_t y[3] = { Y_OFFSET + 18 * cursorPos, // links oben
    Y_OFFSET + 18 * cursorPos + 25,            // rechts mitte
    Y_OFFSET + 18 * cursorPos + 50 };          // links unten
}

void drawMenu(char* path, int16_t cursorPos) {
  static const uint32_t X_OFFSET = 65;
  static const uint32_t Y_OFFSET = 240 - 18;
  static char searchPath[256];

  display_clear(0x00, 0x00, 0x00);
  display_drawText(X_OFFSET - 30, 0, "Use the game pad to select a song", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  display_drawText(X_OFFSET + 10, 18, "Press A button to start", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  strcpy_s(searchPath, sizeof(searchPath), path);
  strcat_s(searchPath, sizeof(searchPath), "\\*");
  drawTracks(searchPath);
  drawCursor(cursorPos);

  display_redraw();
}
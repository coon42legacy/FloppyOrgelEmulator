#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "hal_gui.h"
#include "floppyorgel_system.h"

void debugPrintln(char* text) {
  printf("%s\n\r", text);
}

void drawMenu() {
  static const uint32_t X_OFFSET = 65;
  static const uint32_t Y_OFFSET = 240 - 18;

  gui_clear(0x00, 0x00, 0x00);
  gui_drawText(X_OFFSET - 30, 0, "Use the game pad to select a song", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  gui_drawText(X_OFFSET + 10, 18, "Press A button to start", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  gui_redraw();
  Sleep(1000);
}

void system_main() {
  debugPrintln("\n\r");
  debugPrintln("################################");
  debugPrintln("Floppy Orgel v3.0 initialisiert.");
  debugPrintln("################################"); 

  while (1) {
    drawMenu();
  }
}

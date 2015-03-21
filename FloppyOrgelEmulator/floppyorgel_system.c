#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "hal_gui.h"
#include "floppyorgel_system.h"

void debugPrintln(char* text) {
  printf("%s\n\r", text);
}

void drawMenu() {
  gui_clear(0xFF, 0x00, 0x00);
  printf("Red\n\r");
  Sleep(1000);
  gui_clear(0x00, 0xFF, 0x00);
  printf("Green\n\r");
  Sleep(1000);
  gui_clear(0x00, 0x00, 0xFF);
  printf("Blue\n\r");
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

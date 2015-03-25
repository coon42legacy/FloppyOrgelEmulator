#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "hal_gui.h"
#include "hal_gamepad.h"
#include "floppyorgel_system.h"

void debugPrintln(char* text) {
  printf("%s\n\r", text);
}

void drawCursor(uint32_t cursorPos) {
  const uint32_t X_OFFSET = 25;
  const uint32_t Y_OFFSET = 45;

  gui_drawRect(X_OFFSET, Y_OFFSET + 18 * cursorPos, 5, 5, 255, 255, 255);
  const uint32_t LINE_OFFSET = 18;

  uint32_t x[3] = { 
    X_OFFSET,      // links oben
    X_OFFSET + 50, // rechts mitte
    X_OFFSET };    // links unten

  uint32_t y[3] = { Y_OFFSET + 18 * cursorPos, // links oben
    Y_OFFSET + 18 * cursorPos + 25,            // rechts mitte
    Y_OFFSET + 18 * cursorPos + 50 };          // links unten
}

void drawMenu(int cursorPos) {
  static const uint32_t X_OFFSET = 65;
  static const uint32_t Y_OFFSET = 240 - 18;

  gui_clear(0x00, 0x00, 0x00);
  gui_drawText(X_OFFSET - 30, 0, "Use the game pad to select a song", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  gui_drawText(X_OFFSET + 10, 18, "Press A button to start", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  drawTracks("_sdcard\\*");
  drawCursor(cursorPos);

  gui_redraw();
}

void debugPrintNesGamePadState() {
  union NesGamePadStates_t state = getNesGamepadState();
  if (state.code == 0xFF)
    printf("Game pad is not plugged in\n\r");
  else {
    printf("A: ");      if (state.states.A)      printf(" ON"); else printf("OFF"); printf(" | ");
    printf("B: ");      if (state.states.B)      printf(" ON"); else printf("OFF"); printf(" | ");
    printf("UP: ");     if (state.states.North)  printf(" ON"); else printf("OFF"); printf(" | ");
    printf("DOWN: ");   if (state.states.South)  printf(" ON"); else printf("OFF"); printf(" | ");
    printf("LEFT: ");   if (state.states.West)   printf(" ON"); else printf("OFF"); printf(" | ");
    printf("RIGHT: ");  if (state.states.East)   printf(" ON"); else printf("OFF"); printf(" | ");
    printf("START: ");  if (state.states.Start)  printf(" ON"); else printf("OFF"); printf(" | ");
    printf("SELECT: "); if (state.states.Select) printf(" ON"); else printf("OFF"); printf(" | ");
  }
  printf("\n\r");
}

void system_main() {
  debugPrintln("\n\r");
  debugPrintln("################################");
  debugPrintln("Floppy Orgel v3.0 initialisiert.");
  debugPrintln("################################"); 

  int cursorPos = 0;

  while (1) {
    //debugPrintNesGamePadState();

    union NesGamePadStates_t state = getNesGamepadState();
    if (state.states.South)
      cursorPos = cursorPos < 10 ? cursorPos + 1 : cursorPos;
    else if (state.states.North)
      cursorPos = cursorPos > 0 ? cursorPos - 1 : cursorPos;

    drawMenu(cursorPos);
    Sleep(100);
  }
}

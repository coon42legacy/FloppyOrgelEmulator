#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "hal_gui.h"
#include "hal_gamepad.h"
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

  while (1) {
    debugPrintNesGamePadState();
    drawMenu();
    Sleep(10);
  }
}

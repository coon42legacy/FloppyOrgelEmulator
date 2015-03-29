#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "../hal/hal_display.h"
#include "../hal/hal_gamepad.h"
#include "../hal/hal_filesystem.h"
#include "floppyorgel_system.h"
#include "menu.h"
#include "embedded-midilib/midiplayer.h"
#include "embedded-midilib/hal_midiplayer_win32.h"

#define MIDI_PATH "_sdcard"
static MIDI_PLAYER mpl;

void debugPrintln(char* text) {
  printf("%s\n\r", text);
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
  hal_midiplayer_init(&mpl);
  
  debugPrintln("\n\r");
  debugPrintln("################################");
  debugPrintln("Floppy Orgel v3.0 initialisiert.");
  debugPrintln("################################"); 

  BOOL playingMusic = FALSE;
  int cursorPos = 0;
  while (1) {
    union NesGamePadStates_t state = getNesGamepadState();
    if (state.states.South)
      cursorPos = cursorPos < 10 ? cursorPos + 1 : cursorPos;
    else if (state.states.North)
      cursorPos = cursorPos > 0 ? cursorPos - 1 : cursorPos;
    else if (state.states.A) {
      char filePath[256];
      getFileNameFromCursorPos(MIDI_PATH, filePath, cursorPos);
      playMidiFile(&mpl, filePath);
      playingMusic = TRUE;
    }
     
    if (!midiPlayerTick(&mpl)) {
      if (playingMusic) {
        playingMusic = FALSE;
        hal_printfSuccess("Playback finished!");
      }
      drawMenu(MIDI_PATH, cursorPos);
    }
  }

  hal_midiplayer_free(&mpl);
}

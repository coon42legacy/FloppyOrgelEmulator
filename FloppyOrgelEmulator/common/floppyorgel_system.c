#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "../hal/hal_display.h"
#include "../hal/hal_inputdevice.h"
#include "../hal/hal_filesystem.h"
#include "floppyorgel_system.h"
#include "menu.h"
#include "embedded-midilib/midiplayer.h"
#include "embedded-midilib/hal_midiplayer_win32.h"

#define MIDI_PATH "_sdcard"
//#define MIDI_PATH "C:\\Users\\Stephan\\Desktop\\tmp\\Command & Conquer"

static MIDI_PLAYER mpl;

void debugPrintln(char* text) {
  printf("%s\n\r", text);
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
    InputDeviceStates_t state = getInputDeviceState();
    if (state.South)
      cursorPos = cursorPos < 10 ? cursorPos + 1 : cursorPos;
    else if (state.North)
      cursorPos = cursorPos > 0 ? cursorPos - 1 : cursorPos;
    else if (state.Action) {
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

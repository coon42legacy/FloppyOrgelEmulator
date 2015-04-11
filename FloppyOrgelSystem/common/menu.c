#include <stdint.h>
#include "../hal/hal_inputdevice.h"
#include "../hal/hal_filesystem.h"
#include "../hal/hal_display.h"
// #include "embedded-midilib/midiplayer.h"
// #include "embedded-midilib/hal_midiplayer_win32.h"
#include "canvas/canvas.h"
#include "AsciiLib/AsciiLib.h"
#include "config.h"
#include "menu.h"

// Stack based FSM
#define FSM_STACK_SIZE 16
void* stack[FSM_STACK_SIZE];
int _stackSize;
//static MIDI_PLAYER mpl;
static char filePathOfSongToPlay[256];

// States
void fsmStateMainMenu() {
  InputDeviceStates_t buttonPressed = getInputDeviceState();

  static const uint32_t X_OFFSET = 65;
  //static const uint32_t Y_OFFSET = 240 - 18;
  static uint8_t cursorPos = 0;

  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(X_OFFSET - 30, 0, "Use the game pad to navigate", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(X_OFFSET + 10, 18, "Press A button to select", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  canvas_drawText(35, 40 + 18 * 0, "Button Test", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(35, 40 + 18 * 1, "Play MIDI File", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(35, 40 + 18 * 2, "Live Mode", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(35, 40 + 18 * 3, "Floppy Test", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  drawCursor(cursorPos);
  display_redraw();

  if (buttonPressed.South)
    cursorPos++;
  else if (buttonPressed.North)
    cursorPos--;

  if (buttonPressed.Action) {
    switch (cursorPos) {
      case 0:
        fsmPush(fsmStateButtonTest);
        break;
      case 1:
        fsmPush(fsmStatePlaylist);
        break;
    }        
  }
}

void fsmStateButtonTest() {
  InputDeviceStates_t buttonPressed = getInputDeviceState();
  static const uint32_t X_OFFSET = 65;
  // static const uint32_t Y_OFFSET = 240 - 18;

  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(X_OFFSET - 30, 0, "Button Test", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(X_OFFSET + 10, 18, "Comming soon!", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  display_redraw();

  if (buttonPressed.Back)
    fsmPop();
}

void fsmStatePlaylist() {
  InputDeviceStates_t buttonPressed = getInputDeviceState();
  static uint8_t cursorPos = 0;

  if (buttonPressed.South)
    cursorPos++;
  else if (buttonPressed.North)
    cursorPos--;
  else if (buttonPressed.Action) {
    getFileNameFromCursorPos(MIDI_PATH, filePathOfSongToPlay, cursorPos);
    fsmPush(fsmStartPlayBack);
  }
  else if (buttonPressed.Back)
    fsmPop();

  drawMenu(MIDI_PATH, cursorPos);
}

void fsmStartPlayBack() {
/*
  hal_midiplayer_init(&mpl);
  playMidiFile(&mpl, filePathOfSongToPlay);
  static const uint32_t X_OFFSET = 65;
  static const uint32_t Y_OFFSET = 240 - 18;

  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(X_OFFSET - 30, 0, "Now Playing!", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(X_OFFSET + 10, 18, "Press B to stop playback.", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  display_redraw();

  fsmPop();
  fsmPush(fsmStatePlaying);
  */
}

void fsmStatePlaying() {
	/*
  InputDeviceStates_t buttonPressed = getInputDeviceState();

  if (buttonPressed.Back) {
    fsmPop();
    fsmPush(fsmStatePlaybackAborted);
  }

  if (!midiPlayerTick(&mpl)) {
    fsmPop();
    fsmPush(fsmStatePlaybackFinished);
  }
  */
}

void fsmStatePlaybackFinished() {
  // hal_printfSuccess("Playback finished!");
  // hal_midiplayer_free(&mpl);

  fsmPop();
  fsmPush(fsmStatePlaylist);
}

void fsmStatePlaybackAborted() {
  // hal_printfSuccess("Playback aborted by user.");
  // hal_midiplayer_free(&mpl);

  fsmPop();
  fsmPush(fsmStatePlaylist);
}

// FSM
void fsmInit() {
  _stackSize = 0;
}

bool fsmPush(void* state) {
  if (_stackSize < FSM_STACK_SIZE) {
    stack[_stackSize++] = state;
    return true;
  }
  else
    return false;
}

bool fsmPop() {
  if (_stackSize > 0) {
    _stackSize--;
    return true;
  }
  else
    return false;
}

void* fsmGetCurrentState() {
  if (_stackSize > 0)
    return stack[_stackSize - 1];
  else
    return NULL;
}

void fsmTick() {
  void(*stateFunc)() = fsmGetCurrentState();

  if (stateFunc != NULL)
    stateFunc();
}
//

void getFileNameFromCursorPos(char* srcPath, char* dstFilePath, int cursorPos) {
  FO_FIND_DATA findData;

  strcpy(dstFilePath, srcPath);
  strcat(dstFilePath, "\\*");

  bool endOfDirectory = !fo_findInit(dstFilePath, &findData);
  // static const uint32_t X_OFFSET = 35;
  // static const uint32_t Y_OFFSET = 40;
  int itemCount = 0;

  while (!endOfDirectory) {
    if (findData.fileName[0] != '.')
      if (findData.fileName[1] != '.')
        if (itemCount++ == cursorPos) {
          dstFilePath[strlen(dstFilePath) - 1] = '\0'; // remove trailing start
          strcat(dstFilePath, findData.fileName);
          break;
        }

        if (!fo_findNext(&findData))
          endOfDirectory = true;
  }

  fo_findFree();
}

int drawTracks(char* path) {
  FO_FIND_DATA findData;

  static const uint32_t X_OFFSET = 35;
  static const uint32_t Y_OFFSET = 40;
  int itemCount = 0;
  bool endOfDirectory = !fo_findInit(path, &findData);

  while (!endOfDirectory) {
    if (findData.fileName[0] != '.')
      if (findData.fileName[1] != '.') {
        canvas_drawText(X_OFFSET, Y_OFFSET + 18 * itemCount++, findData.fileName,
          0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
      }

      if (!fo_findNext(&findData))
        endOfDirectory = true;
  }
  fo_findFree();

  return 0;
}

void drawCursor(uint32_t cursorPos) {
  const uint32_t X_OFFSET = 25;
  const uint32_t Y_OFFSET = 45;

  canvas_drawRect(X_OFFSET, Y_OFFSET + 18 * cursorPos, 5, 5, 255, 255, 255);
}

void drawMenu(char* path, int16_t cursorPos) {
  static const uint32_t X_OFFSET = 65;
  // static const uint32_t Y_OFFSET = 240 - 18;
  static char searchPath[256];

  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(X_OFFSET - 30, 0, "Use the game pad to select a song", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(X_OFFSET + 10, 18, "Press A button to start", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  strcpy(searchPath, path);
  strcat(searchPath, "\\*");
  drawTracks(searchPath);
  drawCursor(cursorPos);

  display_redraw();
}

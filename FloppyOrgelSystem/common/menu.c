#include <stdint.h>
#include "../hal/hal_inputdevice.h"
#include "../hal/hal_filesystem.h"
#include "../hal/hal_display.h"
#include "../hal/hal_misc.h"
#include "../hal/hal_mididevice.h"
#include "embedded-midilib/midiplayer.h"
#include "canvas/canvas.h"
#include "AsciiLib/AsciiLib.h"
#include "config.h"
#include "menu.h"

// Stack based FSM
#define FSM_STACK_SIZE 16
void* stack[FSM_STACK_SIZE];
int _stackSize;
static MIDI_PLAYER mpl;
static char filePathOfSongToPlay[256];

// Midi Event handlers
char noteName[64]; // TOOD: refactor to const string array

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

void HexList(uint8_t *pData, int32_t iNumBytes) {
  for (int32_t i = 0; i < iNumBytes; i++)
    printf("%.2x ", pData[i]);
}

void printTrackPrefix(uint32_t track, uint32_t tick, char* pEventName)  {
  printf("[Track: %d] %06d %s ", track, tick, pEventName);
}

static void onNoteOff(int32_t track, int32_t tick, int32_t channel, int32_t note) {
  muGetNameFromNote(noteName, note);
  hal_midiDeviceMessage(msgNoteOff, channel, note, 0);
  printTrackPrefix(track, tick, "Note Off");
  printf("(%d) %s", channel, noteName);
  printf("\r\n");
}

static void onNoteOn(int32_t track, int32_t tick, int32_t channel, int32_t note, int32_t velocity) {
  muGetNameFromNote(noteName, note);
  hal_midiDeviceMessage(msgNoteOn, channel, note, velocity);
  printTrackPrefix(track, tick, "Note On");
  printf("(%d) %s [%d] %d", channel, noteName, note, velocity);
  printf("\r\n");
}

static void onNoteKeyPressure(int32_t track, int32_t tick, int32_t channel, int32_t note, int32_t pressure) {
  muGetNameFromNote(noteName, note);
  hal_midiDeviceMessage(msgNoteKeyPressure, channel, note, pressure);
  printTrackPrefix(track, tick, "Note Key Pressure");
  printf("(%d) %s %d", channel, noteName, pressure);
  printf("\r\n");
}

static void onSetParameter(int32_t track, int32_t tick, int32_t channel, int32_t control, int32_t parameter) {
  muGetControlName(noteName, control);
  printTrackPrefix(track, tick, "Set Parameter");
  hal_midiDeviceMessage(msgSetParameter, channel, control, parameter);
  printf("(%d) %s -> %d", channel, noteName, parameter);
  printf("\r\n");
}

static void onSetProgram(int32_t track, int32_t tick, int32_t channel, int32_t program) {
  muGetInstrumentName(noteName, program);
  hal_midiDeviceMessage(msgSetProgram, channel, program, 0);
  printTrackPrefix(track, tick, "Set Program");
  printf("(%d) %s", channel, noteName);
  printf("\r\n");
}

static void onChangePressure(int32_t track, int32_t tick, int32_t channel, int32_t pressure) {
  muGetControlName(noteName, pressure);
  hal_midiDeviceMessage(msgChangePressure, channel, pressure, 0);
  printTrackPrefix(track, tick, "Change Pressure");
  printf("(%d) %s", channel, noteName);
  printf("\r\n");
}

static void onSetPitchWheel(int32_t track, int32_t tick, int32_t channel, int16_t pitch) {
  hal_midiDeviceMessage(msgSetPitchWheel, channel, pitch << 1, pitch >> 7);
  printTrackPrefix(track, tick, "Set Pitch Wheel");
  printf("(%d) %d", channel, pitch);
  printf("\r\n");
}

static void onMetaMIDIPort(int32_t track, int32_t tick, int32_t midiPort) {
  printTrackPrefix(track, tick, "Meta event ----");
  printf("MIDI Port = %d", midiPort);
  printf("\r\n");
}

static void onMetaSequenceNumber(int32_t track, int32_t tick, int32_t sequenceNumber) {
  printTrackPrefix(track, tick, "Meta event ----");
  printf("Sequence Number = %d", sequenceNumber);
  printf("\r\n");
}

static void _onTextEvents(int32_t track, int32_t tick, const char* textType, const char* pText) {
  printTrackPrefix(track, tick, "Meta event ----");
  hal_printfInfo("%s = %s", textType, pText);
}

static void onMetaTextEvent(int32_t track, int32_t tick, char* pText) {
  _onTextEvents(track, tick, "Text", pText);
}

static void onMetaCopyright(int32_t track, int32_t tick, char* pText) {
  _onTextEvents(track, tick, "Copyright ", pText);
}

static void onMetaTrackName(int32_t track, int32_t tick, char *pText) {
  _onTextEvents(track, tick, "Track name", pText);
}

static void onMetaInstrument(int32_t track, int32_t tick, char *pText) {
  _onTextEvents(track, tick, "Instrument", pText);
}

static void onMetaLyric(int32_t track, int32_t tick, char *pText) {
  _onTextEvents(track, tick, "Lyric", pText);
}

static void onMetaMarker(int32_t track, int32_t tick, char *pText) {
  _onTextEvents(track, tick, "Marker", pText);
}

static void onMetaCuePoint(int32_t track, int32_t tick, char *pText) {
  _onTextEvents(track, tick, "Cue point", pText);
}

static void onMetaEndSequence(int32_t track, int32_t tick) {
  printTrackPrefix(track, tick, "Meta event ----");
  printf("End Sequence");
  printf("\r\n");
}

static void onMetaSetTempo(int32_t track, int32_t tick, int32_t bpm) {
  printTrackPrefix(track, tick, "Meta event ----");
  hal_printfWarning("Tempo = %d bpm", bpm);
}

static void onMetaSMPTEOffset(int32_t track, int32_t tick, uint32_t hours, uint32_t minutes, uint32_t seconds, uint32_t frames, uint32_t subframes) {
  printTrackPrefix(track, tick, "Meta event ----");
  printf("SMPTE offset = %d:%d:%d.%d %d", hours, minutes, seconds, frames, subframes);
  printf("\r\n");
}

static void onMetaTimeSig(int32_t track, int32_t tick, int32_t nom, int32_t denom, int32_t metronome, int32_t thirtyseconds) {
  printTrackPrefix(track, tick, "Meta event ----");
  printf("Time sig = %d/%d", nom, denom);
  printf("\r\n");
}

static void onMetaKeySig(int32_t track, int32_t tick, uint32_t key, uint32_t scale) {
  printTrackPrefix(track, tick, "Meta event ----");
  if (muGetKeySigName(noteName, key)) {
    printf("Key sig = %s", noteName);
    printf("\r\n");
  }
}

static void onMetaSequencerSpecific(int32_t track, int32_t tick, void* pData, uint32_t size) {
  printTrackPrefix(track, tick, "Meta event ----");
  printf("Sequencer specific = ");
  HexList(pData, size);
  printf("\r\n");
}

static void onMetaSysEx(int32_t track, int32_t tick, void* pData, uint32_t size) {
  printTrackPrefix(track, tick, "Meta event ----");
  printf("SysEx = ");
  HexList(pData, size);
  printf("\r\n");
}

void fsmStartPlayBack() {
  hal_midiDeviceInit();
  midiplayer_init(&mpl, onNoteOff, onNoteOn, onNoteKeyPressure, onSetParameter, onSetProgram, onChangePressure,
    onSetPitchWheel, onMetaMIDIPort, onMetaSequenceNumber, onMetaTextEvent, onMetaCopyright, onMetaTrackName, 
    onMetaInstrument, onMetaLyric, onMetaMarker, onMetaCuePoint, onMetaEndSequence, onMetaSetTempo, 
    onMetaSMPTEOffset, onMetaTimeSig, onMetaKeySig, onMetaSequencerSpecific, onMetaSysEx);

  playMidiFile(&mpl, filePathOfSongToPlay);
  static const uint32_t X_OFFSET = 65;
  static const uint32_t Y_OFFSET = 240 - 18;

  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(X_OFFSET - 30, 0, "Now Playing!", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(X_OFFSET + 10, 18, "Press B to stop playback.", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  display_redraw();

  fsmPop();
  fsmPush(fsmStatePlaying);
}

void fsmStatePlaying() {
  InputDeviceStates_t buttonPressed = getInputDeviceState();

  if (buttonPressed.Back) {
    fsmPop();
    fsmPush(fsmStatePlaybackAborted);
  }

  if (!midiPlayerTick(&mpl)) {
    fsmPop();
    fsmPush(fsmStatePlaybackFinished);
  }
}

void fsmStatePlaybackFinished() {
  hal_printfSuccess("Playback finished!");
  hal_midiDeviceFree();

  fsmPop();
  fsmPush(fsmStatePlaylist);
}

void fsmStatePlaybackAborted() {
  hal_printfSuccess("Playback aborted by user.");
  hal_midiDeviceFree();

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

  bool endOfDirectory = !hal_findInit(dstFilePath, &findData);
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

        if (!hal_findNext(&findData))
          endOfDirectory = true;
  }

  hal_findFree();
}

int drawTracks(char* path) {
  FO_FIND_DATA findData;

  static const uint32_t X_OFFSET = 35;
  static const uint32_t Y_OFFSET = 40;
  int itemCount = 0;
  bool endOfDirectory = !hal_findInit(path, &findData);

  while (!endOfDirectory) {
    if (findData.fileName[0] != '.')
      if (findData.fileName[1] != '.') {
        canvas_drawText(X_OFFSET, Y_OFFSET + 18 * itemCount++, findData.fileName,
          0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
      }

      if (!hal_findNext(&findData))
        endOfDirectory = true;
  }
  hal_findFree();

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

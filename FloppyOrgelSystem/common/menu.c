#include <stdint.h>
#include "../hal/hal_inputdevice.h"
#include "../hal/hal_filesystem.h"
#include "../hal/hal_display.h"
#include "../hal/hal_misc.h"
#include "../hal/hal_mididevice.h"
#include "embedded-midilib/midiutil.h"
#include "embedded-midilib/midiplayer.h"
#include "canvas/canvas.h"
#include "AsciiLib/AsciiLib.h"
#include "LockFreeFIFO.h"
#include "StackBasedFsm.h"
#include "config.h"
#include "menu.h"

static LockFreeFIFO_t fifoDebugPort;
static MIDI_PLAYER mpl;
static char filePathOfSongToPlay[256];

// Midi Event handlers

// helpers
void HexList(uint8_t *pData, int32_t iNumBytes) {
  for (int32_t i = 0; i < iNumBytes; i++)
    printf("%.2x ", pData[i]);
}

void printTrackPrefix(uint32_t track, uint32_t tick, char* pEventName)  {
  printf("[Track: %d] %06d %s ", track, tick, pEventName);
}

// MIDI callbacks

static void onNoteOff(int32_t track, int32_t tick, int32_t channel, int32_t note) {
  hal_midiDeviceMessage(msgNoteOff, channel, note, 0);
  hal_midiDeviceNoteOff(channel, note);
  printTrackPrefix(track, tick, "Note Off");
  printf("(%d) %s", channel, muGetNameFromNote(note));
  printf("\r\n");
}

static void onNoteOn(int32_t track, int32_t tick, int32_t channel, int32_t note, int32_t velocity) {
  hal_midiDeviceMessage(msgNoteOn, channel, note, velocity);
  hal_midiDeviceNoteOn(channel, note, velocity);
  printTrackPrefix(track, tick, "Note On");
  printf("(%d) %s [%d] %d", channel, muGetNameFromNote(note), note, velocity);
  printf("\r\n");
}

static void onNoteKeyPressure(int32_t track, int32_t tick, int32_t channel, int32_t note, int32_t pressure) {
  hal_midiDeviceMessage(msgNoteKeyPressure, channel, note, pressure);
  printTrackPrefix(track, tick, "Note Key Pressure");
  printf("(%d) %s %d", channel, muGetNameFromNote(note), pressure);
  printf("\r\n");
}

static void onSetParameter(int32_t track, int32_t tick, int32_t channel, int32_t control, int32_t parameter) {
  printTrackPrefix(track, tick, "Set Parameter");
  hal_midiDeviceMessage(msgSetParameter, channel, control, parameter);
  printf("(%d) %s -> %d", channel, muGetControlName(control), parameter);
  printf("\r\n");
}

static void onSetProgram(int32_t track, int32_t tick, int32_t channel, int32_t program) {
  hal_midiDeviceMessage(msgSetProgram, channel, program, 0);
  printTrackPrefix(track, tick, "Set Program");
  printf("(%d) %s", channel, muGetInstrumentName(program));
  printf("\r\n");
}

static void onChangePressure(int32_t track, int32_t tick, int32_t channel, int32_t pressure) {
  hal_midiDeviceMessage(msgChangePressure, channel, pressure, 0);
  printTrackPrefix(track, tick, "Change Pressure");
  printf("(%d) %s", channel, muGetControlName(pressure));
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
  // TODO: fix
  static char noteName[256];
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

void stopAllDrives() {  
  hal_printfInfo("Stopping all drives...");
  for (int i = 0; i < 16; i++) {    
    onNoteOff(0, 0, i, 0);
  }
}

void getFileNameFromCursorPos(char* srcPath, char* dstFilePath, int cursorPos) {
  static FO_FIND_DATA findData;

  strcpy(dstFilePath, srcPath);
  strcat(dstFilePath, "/*");

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
  static FO_FIND_DATA findData;

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
  strcat(searchPath, "/*");
  drawTracks(searchPath);
  drawCursor(cursorPos);

  display_redraw();
}

// States
void fsmStateMainMenu(StackBasedFsm_t* fsm) {
  static bool firstRun = true;
  if (firstRun) {
    hal_rs485init(&fifoDebugPort);
    firstRun = false;
  }
  
  InputDeviceStates_t buttonPressed = getInputDeviceState();
  static const uint32_t X_OFFSET = 65;
  //static const uint32_t Y_OFFSET = 240 - 18;
  static uint8_t cursorPos = 2;

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
        fsmPush(fsm, fsmStateButtonTest);
        break;
      case 1:        
        fsmPush(fsm, fsmStatePlaylist);
        break;
      case 2:
        fsmPush(fsm, fsmStateLiveMode);
        break;
    }        
  }
}

void fsmStateButtonTest(StackBasedFsm_t* fsm) {
  InputDeviceStates_t buttonPressed = getInputDeviceState();
  static const uint32_t X_OFFSET = 65;
  // static const uint32_t Y_OFFSET = 240 - 18;

  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(X_OFFSET - 30, 0, "Button Test", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(X_OFFSET + 10, 18, "Comming soon!", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  display_redraw();

  if (buttonPressed.Back)
    fsmPop(fsm);
}

void fsmStatePlaylist(StackBasedFsm_t* fsm) {
  InputDeviceStates_t buttonPressed = getInputDeviceState();
  static uint8_t cursorPos = 0;

  if (buttonPressed.South)
    cursorPos++;
  else if (buttonPressed.North)
    cursorPos--;
  else if (buttonPressed.Action) {
    getFileNameFromCursorPos(MIDI_PATH, filePathOfSongToPlay, cursorPos);
    printf("Playing: %s\r\n", filePathOfSongToPlay);
    fsmPush(fsm, fsmStartPlayBack);
  }
  else if (buttonPressed.Back)
    fsmPop(fsm);

  drawMenu(MIDI_PATH, cursorPos);
}

void fsmStateLiveMode(StackBasedFsm_t* fsm) {
  static const uint32_t X_OFFSET = 100;
  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(X_OFFSET - 30, 0, "--- Live mode ---", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(0, 18, "Now receiving MIDI-Data on debug port...", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  display_redraw();

  fsmPush(fsm, fsmStateLiveReceiving);
}

void fsmStateLiveReceiving(StackBasedFsm_t* fsm) {
  InputDeviceStates_t buttonPressed = getInputDeviceState();

  while (getRingBufferDistance(&fifoDebugPort) < RING_BUFFER_SIZE) {
    hal_rs485Send(readFromRingBuffer(&fifoDebugPort));
    // display_clear(0, 255, 0);
  }
  
  if (buttonPressed.Back) {
    fsmPop(fsm);
    fsmPop(fsm);
  }
}

void fsmStartPlayBack(StackBasedFsm_t* fsm) {
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

  fsmPop(fsm);
  fsmPush(fsm, fsmStatePlaying);
}

void fsmStatePlaying(StackBasedFsm_t* fsm) {
  InputDeviceStates_t buttonPressed = getInputDeviceState();

  if (buttonPressed.Back) {
    fsmPop(fsm);
    fsmPush(fsm, fsmStatePlaybackAborted);
  }

  if (!midiPlayerTick(&mpl)) {
    fsmPop(fsm);
    fsmPush(fsm, fsmStatePlaybackFinished);
  }
}

void fsmStatePlaybackFinished(StackBasedFsm_t* fsm) {
  hal_printfSuccess("Playback finished!");
  stopAllDrives();
  hal_midiDeviceFree();

  fsmPop(fsm);
  fsmPush(fsm, fsmStatePlaylist);
}

void fsmStatePlaybackAborted(StackBasedFsm_t* fsm) {
  hal_printfSuccess("Playback aborted by user.");
  stopAllDrives();
  hal_midiDeviceFree();

  fsmPop(fsm);
  fsmPush(fsm, fsmStatePlaylist);
}

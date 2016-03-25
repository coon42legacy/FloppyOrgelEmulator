#include "../../StackBasedFsm.h"
#include "../../embedded-midilib/midiutil.h"
#include "../../embedded-midilib/midiplayer.h"
#include "../../../hal/hal_mididevice.h"
#include "../../../hal/hal_misc.h"

#include "startPlayBack.h"

// TODO: put into context
static char filePathOfSongToPlay[256];
static MIDI_PLAYER mpl;

// helpers
static void HexList(uint8_t* pData, int32_t iNumBytes) {
  for (int32_t i = 0; i < iNumBytes; i++)
    printf("%.2x ", pData[i]);
}

static void printTrackPrefix(uint32_t track, uint32_t tick, char* pEventName)  {
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
  hal_midiDeviceMessage(msgControlChange, channel, control, parameter);
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

static void stopAllDrives() {
  hal_printfInfo("Stopping all drives...");
  for (int i = 0; i < 16; i++) {
    onNoteOff(0, 0, i, 0);
  }
}

// ------------------------------------------------------------------------------------------------------------
// playing
// ------------------------------------------------------------------------------------------------------------

void playing(StackBasedFsm_t* pFsm, FsmState* state, void* pArgs) {
  // InputDeviceStates_t buttonPressed = getInputDeviceState();

  // if (buttonPressed.Back) {
  //   fsmPop(fsm);
  //   fsmPush(fsm, playbackAborted, NULL);
  // }
  // 
  // if (!midiPlayerTick(&mpl)) {
  //   fsmPop(fsm);
  //   fsmPush(fsm, playbackFinished, NULL);
  // }
}

// ------------------------------------------------------------------------------------------------------------
// startPlayBack
// ------------------------------------------------------------------------------------------------------------

void startPlayBack(StackBasedFsm_t* pFsm, FsmState* state, void* pParams) {
  char* pPath = pParams;

  hal_printfInfo("Start playback of file: %s", pPath);

//  hal_midiDeviceInit();
//  midiplayer_init(&mpl, onNoteOff, onNoteOn, onNoteKeyPressure, onSetParameter, onSetProgram, onChangePressure,
//    onSetPitchWheel, onMetaMIDIPort, onMetaSequenceNumber, onMetaTextEvent, onMetaCopyright, onMetaTrackName,
//    onMetaInstrument, onMetaLyric, onMetaMarker, onMetaCuePoint, onMetaEndSequence, onMetaSetTempo,
//    onMetaSMPTEOffset, onMetaTimeSig, onMetaKeySig, onMetaSequencerSpecific, onMetaSysEx);
//
//  playMidiFile(&mpl, filePathOfSongToPlay);
//  static const uint32_t X_OFFSET = 65;
//  static const uint32_t Y_OFFSET = 240 - 18;
//
//  canvas_clear(0x00, 0x00, 0x00);
//  canvas_drawText(CENTER, 0, "Now Playing!", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
//  canvas_drawText(CENTER, 18, "Press B to stop playback.", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
//  display_redraw();
//
//  fsmPop(fsm);
//  fsmPush(fsm, playing, NULL);
}

// ------------------------------------------------------------------------------------------------------------
// playbackFinished
// ------------------------------------------------------------------------------------------------------------

void playbackFinished(StackBasedFsm_t* pFsm, FsmState* state, void* pArgs) {
  //  hal_printfSuccess("Playback finished!");
  //  stopAllDrives();
  //  hal_midiDeviceFree();
  //
  //  fsmPop(fsm);
  //  fsmPush(fsm, playlist, NULL);
}

// ------------------------------------------------------------------------------------------------------------
// playbackAborted
// ------------------------------------------------------------------------------------------------------------

void playbackAborted(StackBasedFsm_t* pFsm, FsmState* state, void* pArgs) {
  //  hal_printfSuccess("Playback aborted by user.");
  //  stopAllDrives();
  //  hal_midiDeviceFree();
  //
  //  fsmPop(fsm);
  //  fsmPush(fsm, playlist, NULL);
}
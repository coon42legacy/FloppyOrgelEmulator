#include <Windows.h>
#include <stdint.h>
#include <mmsystem.h>
#include "hal_misc.h"
#include "hal_mididevice.h"
#pragma comment (lib, "winmm.lib")

static HMIDIOUT hMidiOut;

void hal_midiDeviceInit() {
  uint32_t result = midiOutOpen(&hMidiOut, MIDI_MAPPER, 0, 0, 0);
  if (result != MMSYSERR_NOERROR)
    hal_printfError("MIDI-Device does not work!");
}

void hal_midiDeviceFree() {
  midiOutClose(hMidiOut);
}

void hal_midiDeviceNoteOff(int32_t channel, int32_t note) {
  // Not used on win32
}

void hal_midiDeviceNoteOn(int32_t channel, int32_t note, int32_t velocity) {
  // Not used on win32
}

uint32_t hal_midiDeviceMessage(int32_t iStatus, int32_t iChannel, int32_t iData1, int32_t iData2) {
  uint32_t dwMessage = iStatus | iChannel - 1 | (iData1 << 8) | (iData2 << 16);
  return midiOutShortMsg(hMidiOut, dwMessage);
}

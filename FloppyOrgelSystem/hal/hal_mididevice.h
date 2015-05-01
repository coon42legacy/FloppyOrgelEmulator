#ifndef __HAL_MIDIDEVICE_H
#define __HAL_MIDIDEVICE_H

void hal_midiDeviceInit();
void hal_midiDeviceFree();
void hal_midiDeviceNoteOff(int32_t channel, int32_t note);
void hal_midiDeviceNoteOn(int32_t channel, int32_t note, int32_t velocity);
uint32_t hal_midiDeviceMessage(int32_t iStatus, int32_t iChannel, int32_t iData1, int32_t iData2);

#endif // __HAL_DISPLAY_H
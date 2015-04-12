#ifndef __HAL_MIDIDEVICE_H
#define __HAL_MIDIDEVICE_H

void hal_midiDeviceInit();
void hal_midiDeviceFree();
uint32_t hal_midiDeviceMessage(int32_t iStatus, int32_t iChannel, int32_t iData1, int32_t iData2);

#endif // __HAL_DISPLAY_H
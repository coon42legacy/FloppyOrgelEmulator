#ifndef __HAL_INPUTDEVICE_H
#define __HAL_INPUTDEVICE_H

#include <stdint.h>

typedef struct {
  uint16_t
  East   : 1,
  West   : 1,
  South  : 1,
  North  : 1,
  Start  : 1,
  Select : 1,
  Back   : 1,
  Action : 1,
  Connected : 1;
} InputDeviceStates_t;

void setupInputDevice();
InputDeviceStates_t getInputDeviceState();
void debugPrintInputDeviceState();

#endif
#include "hal_inputdevice.h"
#include <stdio.h>
#include <Windows.h>

extern HWND g_hEmuWnd;

#define KEY_ACTION VK_RETURN
#define KEY_BACK   VK_BACK
#define KEY_NORTH  'W'
#define KEY_WEST   'A'
#define KEY_SOUTH  'S'
#define KEY_EAST   'D'
#define KEY_SIMULATE_DISCONNECTED VK_TAB

// Optional
#define KEY_START VK_SPACE
#define KEY_SELECT 'X'

void setupInputDevice() {

}

uint8_t getButtonStateEmu(int virtualKeyCode) {  
  if (GetForegroundWindow() == g_hEmuWnd)
    return (GetAsyncKeyState(virtualKeyCode) & 32768) >> 15;
  else
    return 0;
}

InputDeviceStates_t getInputDeviceState() {
  InputDeviceStates_t states;

  states.Connected = !getButtonStateEmu(KEY_SIMULATE_DISCONNECTED);
  states.Action = getButtonStateEmu(KEY_ACTION);
  states.Back   = getButtonStateEmu(KEY_BACK);
  states.North  = getButtonStateEmu(KEY_NORTH) || getButtonStateEmu(VK_UP);
  states.West   = getButtonStateEmu(KEY_WEST)  || getButtonStateEmu(VK_LEFT);
  states.South  = getButtonStateEmu(KEY_SOUTH) || getButtonStateEmu(VK_DOWN);
  states.East   = getButtonStateEmu(KEY_EAST)  || getButtonStateEmu(VK_RIGHT);
    
  return states;
}

void debugPrintInputDeviceState() {
  InputDeviceStates_t state = getInputDeviceState();
  if (!state.Connected)
    printf("Game pad is not plugged in");
  else {
    printf("Action: "); if (state.Action) printf(" ON"); else printf("OFF"); printf(" | ");
    printf("Back: ");   if (state.Back)   printf(" ON"); else printf("OFF"); printf(" | ");
    printf("UP: ");     if (state.North)  printf(" ON"); else printf("OFF"); printf(" | ");
    printf("DOWN: ");   if (state.South)  printf(" ON"); else printf("OFF"); printf(" | ");
    printf("LEFT: ");   if (state.West)   printf(" ON"); else printf("OFF"); printf(" | ");
    printf("RIGHT: ");  if (state.East)   printf(" ON"); else printf("OFF"); printf(" | ");
  }
  printf("\n\r");
}

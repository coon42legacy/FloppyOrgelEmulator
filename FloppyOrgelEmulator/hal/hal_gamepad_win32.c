#include "hal_gamepad.h"
#include <Windows.h>

extern HWND g_hEmuWnd;

#define KEY_ACTION VK_RETURN
#define KEY_BACK   VK_BACK
#define KEY_NORTH  'W'
#define KEY_WEST   'A'
#define KEY_SOUTH  'S'
#define KEY_EAST   'D'

// Optional
#define KEY_START VK_SPACE
#define KEY_SELECT 'X'

void setupNesGamePad() {

}

uint8_t getButtonStateEmu(int virtualKeyCode) {  
  if (GetForegroundWindow() == g_hEmuWnd)
    return (GetAsyncKeyState(virtualKeyCode) & 32768) >> 15;
  else
    return 0;
}

union NesGamePadStates_t getNesGamepadState() {
  union NesGamePadStates_t states;
      
  states.states.A      = getButtonStateEmu(KEY_ACTION);
  states.states.B      = getButtonStateEmu(KEY_BACK);
  states.states.North  = getButtonStateEmu(KEY_NORTH) || getButtonStateEmu(VK_UP);
  states.states.West   = getButtonStateEmu(KEY_WEST)  || getButtonStateEmu(VK_LEFT);
  states.states.South  = getButtonStateEmu(KEY_SOUTH) || getButtonStateEmu(VK_DOWN);
  states.states.East   = getButtonStateEmu(KEY_EAST)  || getButtonStateEmu(VK_RIGHT);
  states.states.Start  = getButtonStateEmu(KEY_ACTION);
  states.states.Start  = getButtonStateEmu(KEY_START);
  states.states.Select = getButtonStateEmu(KEY_SELECT);
  
  return states;
}

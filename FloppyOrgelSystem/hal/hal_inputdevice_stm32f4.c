#include <stdio.h>
#include "hal_inputdevice.h"
#include "NesGamePad/NesGamePad.h"

void setupInputDevice() {

}

InputDeviceStates_t getInputDeviceState() {
  InputDeviceStates_t states;
  union NesGamePadStates_t nesStates = getNesGamepadState();

  states.Action = nesStates.states.A;
  states.Back = nesStates.states.B;
  states.East = nesStates.states.East;
  states.West = nesStates.states.West;
  states.North = nesStates.states.North;
  states.South = nesStates.states.South;
  states.Connected = nesStates.code != 0xFF;

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

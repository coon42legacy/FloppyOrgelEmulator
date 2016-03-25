#include "../StackBasedFsm.h"
#include "../SlotBasedMenu.h"
#include "../canvas/canvas.h"
#include "../../hal/hal_display.h"
#include "../../hal/hal_inputdevice.h"
#include "../../hal/hal_mididevice.h"
#include "../LockFreeFIFO.h"
#include "liveMode.h"

void liveReceiving(StackBasedFsm_t* pFsm, FsmState* state, void* pArgs) {
 // InputDeviceStates_t buttonPressed = getInputDeviceState();

  // while (getRingBufferDistance(&fifoDebugPort) < RING_BUFFER_SIZE) { // TODO: provide fifoDebugPort as parameter here!
  //   hal_rs485Send(readFromRingBuffer(&fifoDebugPort));
  //   // display_clear(0, 255, 0);
  // }

  // if (buttonPressed.Back) {
  //   fsmPop(fsm);
  //   fsmPop(fsm);
  // }
}

void liveMode(StackBasedFsm_t* pFsm, FsmState* state) {
//  canvas_clear(0x00, 0x00, 0x00);
//  canvas_drawText(CENTER, 0, "--- Live mode ---", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
//  canvas_drawText(CENTER, 18, "Now receiving MIDI-Data on debug port...", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
//  display_redraw();
//
//  fsmPush(fsm, liveReceiving, NULL);
}

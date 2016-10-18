#include "../StackBasedFsm.h"
#include "../SlotBasedMenu.h"
#include "../canvas/canvas.h"
#include "../../hal/hal_display.h"
#include "../../hal/hal_inputdevice.h"
#include "../../hal/hal_mididevice.h"
#include "../../hal/hal_misc.h"
#include "../LockFreeFifo.h"
#include "liveMode.h"

static struct {
  LockFreeFIFO_t* pFifoDebugPort;

} context;

static void onEnter(StackBasedFsm_t* pFsm, void* pParams) {
  hal_printf("liveMode::onEnter()");

  context.pFifoDebugPort = (LockFreeFIFO_t*)pParams;

  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 0, "--- Live mode ---", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 18, "Now receiving MIDI-Data on debug port...", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  display_redraw();
}

static void onBackPress(StackBasedFsm_t* pFsm) {
  hal_printf("example::onBackPress()");

  leaveState(pFsm);
}

static void onLeaveState(StackBasedFsm_t* pFsm) {
  hal_printf("example::onLeaveState()");

  // TODO: stop all floppies
}

static void onTick(StackBasedFsm_t* pFsm) {
  // while (getRingBufferDistance(context.pFifoDebugPort) < RING_BUFFER_SIZE)

  while (ringBufferDataAvailable(context.pFifoDebugPort))
    hal_rs485Send(readFromRingBuffer(context.pFifoDebugPort));
}

// Always implement this as last function of your state file:

void liveMode(StackBasedFsm_t* pFsm, FsmState* pState) {
  pState->onEnterState = onEnter;

  // Button press callbacks:
  pState->onBackPress = onBackPress;

  // State callbacks:
  pState->onLeaveState = onLeaveState;
  pState->onTick = onTick;
}
































// void liveReceiving(StackBasedFsm_t* pFsm, FsmState* state, void* pArgs) {
//   InputDeviceStates_t buttonPressed = getInputDeviceState();
//
//   while (getRingBufferDistance(&fifoDebugPort) < RING_BUFFER_SIZE) { // TODO: provide fifoDebugPort as parameter here!
//     hal_rs485Send(readFromRingBuffer(&fifoDebugPort));
//     // display_clear(0, 255, 0);
//   }
//
//   if (buttonPressed.Back) {
//     fsmPop(fsm);
//     fsmPop(fsm);
//   }
// }

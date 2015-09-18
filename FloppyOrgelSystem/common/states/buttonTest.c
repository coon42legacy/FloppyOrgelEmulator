#include "../StackBasedFsm.h"
#include "../canvas/canvas.h"
#include "../../hal/hal_display.h"
#include "../../hal/hal_inputdevice.h"

FsmState buttonTest(StackBasedFsm_t* fsm) {
  InputDeviceStates_t buttonPressed = getInputDeviceState();
  
  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 0, "Button Test", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 18, "Coming soon!", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  display_redraw();

  if (buttonPressed.Back)
    fsmPop(fsm);
}
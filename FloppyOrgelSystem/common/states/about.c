#include "../StackBasedFsm.h"
#include "../SlotBasedMenu.h"
#include "../canvas/canvas.h"
#include "../../hal/hal_display.h"
#include "../../hal/hal_inputdevice.h"
#include "about.h"

FsmState about(StackBasedFsm_t* fsm) {
  canvas_clear(0, 0, 0);
  canvas_drawText(CENTER, CENTER, "Version: " VERSION, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  if (getInputDeviceState().Back)
    fsmPop(fsm);
  
  display_redraw();
}

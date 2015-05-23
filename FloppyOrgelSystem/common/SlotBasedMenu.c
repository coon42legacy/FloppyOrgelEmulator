#include <stdint.h>
#include "StackBasedFsm.h"
#include "SlotBasedMenu.h"
#include "../hal/hal_inputdevice.h"

void menuTick(SlotBasedMenu_t* sbm, StackBasedFsm_t* fsm) {
  InputDeviceStates_t buttonPressed = getInputDeviceState();

  if (buttonPressed.South && sbm->cursorPos + 1 < sbm->numSlots)
    sbm->cursorPos++;
  else if (buttonPressed.North && sbm->cursorPos > 0)
    sbm->cursorPos--;
  else if (buttonPressed.Action)
    fsmPush(fsm, sbm->slot[sbm->cursorPos].pNextFsmStateFunc);
  else if (buttonPressed.Back)
    fsmPop(fsm);
}

void menuAddSlot(SlotBasedMenu_t* sbm, char* label, FsmStateFunc pTransitionStateFunc) {
  MenuSlot_t* slot = &sbm->slot[sbm->numSlots];

  if (sbm->numSlots >= MENU_MAX_SLOTS)
    return;

  slot->pLabel = label;
  slot->pNextFsmStateFunc = pTransitionStateFunc;
  sbm->numSlots++;
}

void menuDraw(SlotBasedMenu_t* sbm) {
  for (int i = 0; i < sbm->numSlots; i++) 
    canvas_drawText(35, 40 + 18 * i, sbm->slot[i].pLabel, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  drawCursor(sbm->cursorPos);
}
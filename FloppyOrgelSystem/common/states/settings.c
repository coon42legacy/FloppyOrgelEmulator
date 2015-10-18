#include "../StackBasedFsm.h"
#include "../SlotBasedMenu.h"
#include "../canvas/canvas.h"
#include "../../hal/hal_display.h"
#include "settings.h"

void onSettingsMenuAction(StackBasedFsm_t* fsm) {
  
}

void onSettingsMenuBack(StackBasedFsm_t* fsm) {
  fsmPop(fsm);
}

FsmState settings(StackBasedFsm_t* pFsm) {
  static SlotBasedMenu_t menu;

  canvas_clear(0, 0, 0);
  canvas_drawText(CENTER, 0, "--- Settings ---", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  static bool firstRun = true;
  if (firstRun) {
    settingsMenuInit(&menu, pFsm, 25, 45, onSettingsMenuAction, onSettingsMenuBack);
    // menuAddSlot(&menu, "BG Red: [0x00]", NULL);
    // menuAddSlot(&menu, "BG Green: [0x00]", NULL);
    // menuAddSlot(&menu, "BG Blue: [0x00]", NULL);

    firstRun = false;
  }

  menuTick(&menu, pFsm);
  menuDraw(&menu);
  display_redraw();
}

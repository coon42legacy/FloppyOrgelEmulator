#include "../StackBasedFsm.h"
#include "../SlotBasedMenu.h"
#include "../canvas/canvas.h"
#include "../../hal/hal_display.h"
#include "floppyTest.h"

FsmState floppyTest(StackBasedFsm_t* pFsm) {
  static SlotBasedMenu_t menu;
  static bool firstRun = true;
  if (firstRun) {
    userMenuInit(&menu, pFsm, 0, 0, 0, 0);
    firstRun = false;
  }
  menuTick(&menu, pFsm);

  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 0, "--- Floppy Test ---", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 18, "Select drive and Frequency. Press 'A' Button to play.", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  display_redraw();
}

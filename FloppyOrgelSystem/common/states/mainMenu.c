#include "../StackBasedFsm.h"
#include "../SlotBasedMenu.h"
#include "../canvas/canvas.h"
#include "../../hal/hal_mididevice.h"
#include "../../hal/hal_display.h"

#include "../states/buttonTest.h"
#include "../states/playlist.h"
#include "../states/liveMode.h"
#include "../states/settings.h"
#include "../states/about.h"

#include "mainMenu.h"

static LockFreeFIFO_t fifoDebugPort;

static void onUserMenuAction(StackBasedFsm_t* fsm, FsmStateFunc curState, FsmStateFunc nextState) {
  fsmPush(fsm, nextState);
}

static void onUserMenuBack(StackBasedFsm_t* fsm, FsmStateFunc curState) {
  fsmPop(fsm);
}

FsmState mainMenu(StackBasedFsm_t* fsm) {
  static SlotBasedMenu_t menu;

  static bool firstRun = true;
  if (firstRun) {
    hal_rs485init(&fifoDebugPort);

    userMenuInit(&menu, 3, 45, onUserMenuAction, onUserMenuBack);
    menuAddSlot(&menu, "Button Test", buttonTest);
    menuAddSlot(&menu, "Play MIDI File", playlist);
    menuAddSlot(&menu, "Live Mode", liveMode);
    menuAddSlot(&menu, "Settings", settings);
    menuAddSlot(&menu, "About", about);
      
    firstRun = false;
  }
  
  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 0, "Use the game pad to navigate", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 18, "Press A button to select", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  menuTick(&menu, fsm);
  menuDraw(&menu);
  display_redraw();
}
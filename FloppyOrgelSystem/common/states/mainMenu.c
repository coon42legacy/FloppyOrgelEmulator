#include "../StackBasedFsm.h"
#include "../SlotBasedMenu.h"
#include "../canvas/canvas.h"
#include "../../hal/hal_mididevice.h"
#include "../../hal/hal_display.h"
#include "../../hal/hal_misc.h"

#include "../states/buttonTest.h"
#include "../states/playlist.h"
#include "../states/liveMode.h"
#include "../states/settings.h"
#include "../states/about.h"

#include "mainMenu.h"

static struct {
  LockFreeFIFO_t fifoDebugPort;
  SlotBasedMenu_t menu;
} context;


static void draw() {
  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 0, "Use the game pad to navigate", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 18, "Press A button to select", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  menuDraw(&context.menu);
  display_redraw();
}

static void onAction() {
  hal_printf("mainMenu::onAction()");

  userMenuTransitToSelectedSlot(&context.menu);
}

static void onBack() {
  hal_printf("mainMenu::onBack()");

  userMenuTransitBack(&context.menu);
}

static void onEnter(void* pArgs) {
  StackBasedFsm_t* pFsm = pArgs;
  hal_printf("mainMenu::onEnter()");

  hal_rs485init(&context.fifoDebugPort); // TODO: move to live mode state?
  userMenuInit(&context.menu, pFsm, 3, 45);
  menuAddSlot(&context.menu, "Button Test", buttonTest);
  menuAddSlot(&context.menu, "Play MIDI File", playlist);
  menuAddSlot(&context.menu, "Live Mode", liveMode);
  menuAddSlot(&context.menu, "Settings", settings);
  menuAddSlot(&context.menu, "About", about);

  draw();
}

static void onReenter() {
  hal_printf("mainMenu::onReenter()");

  draw();
}

static void onLeaveState() {
  hal_printf("mainMenu::onLeaveState()");
}

static void onTick() {

}

static void onDirection(bool south, bool north, bool west, bool east) {
  // hal_printf("mainMenu::onDirection()");

  if (south)
    menuMoveCursorDown(&context.menu);

  if (north)
    menuMoveCursorUp(&context.menu);
  
  draw();
}

void mainMenu(FsmState* state, void* pArgs) {
  state->onAction = onAction;
  state->onBack = onBack;
  state->onDirection = onDirection;
  state->onEnterState = onEnter;
  state->onReenterState = onReenter;
  state->onLeaveState = onLeaveState;
  state->onTick = onTick;
 
  state->onEnterState(pArgs);
}

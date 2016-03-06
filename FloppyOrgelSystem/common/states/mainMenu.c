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

static void onAction(StackBasedFsm_t* pFsm) {
  hal_printf("mainMenu::onAction()");

  userMenuTransitToSelectedSlot(&context.menu, context.menu.pFsm);
}

static void onBackPress(StackBasedFsm_t* pFsm) {
  hal_printf("mainMenu::onBackPress()");

  userMenuTransitBack(&context.menu);
}

static void onBackRelease(StackBasedFsm_t* pFsm) {
  hal_printf("mainMenu::onBackRelease()");
}

static void onEnter(StackBasedFsm_t* pFsm, void* pParams) {
  hal_printf("mainMenu::onEnter()");

  hal_rs485init(&context.fifoDebugPort); // TODO: move to live mode state?
  menuInit(&context.menu, pFsm, 3, 45);
  menuAddSlot(&context.menu, "Button Test", buttonTest);
  menuAddSlot(&context.menu, "Play MIDI File", playlist);
  menuAddSlot(&context.menu, "Live Mode", liveMode);
  menuAddSlot(&context.menu, "Settings", settings);
  menuAddSlot(&context.menu, "About", about);

  draw();
}

static void onReenter(StackBasedFsm_t* pFsm) {
  hal_printf("mainMenu::onReenter()");

  draw();
}

static void onLeaveState(StackBasedFsm_t* pFsm) {
  hal_printf("mainMenu::onLeaveState()");
}

static void onTick(StackBasedFsm_t* pFsm) {

}

static void onDirection(StackBasedFsm_t* pFsm, bool south, bool north, bool west, bool east) {
  // hal_printf("mainMenu::onDirection()");

  if (south)
    menuMoveCursorDown(&context.menu);

  if (north)
    menuMoveCursorUp(&context.menu);
  
  draw();
}

void mainMenu(StackBasedFsm_t* pFsm, FsmState* pState, void* pParams) {
  pState->onActionPress = onAction;
  pState->onBackPress = onBackPress;
  pState->onBackRelease = onBackRelease;
  pState->onDirection = onDirection;
  pState->onEnterState = onEnter;
  pState->onReenterState = onReenter;
  pState->onLeaveState = onLeaveState;
  pState->onTick = onTick;

  pState->onEnterState(pFsm, pParams);
}

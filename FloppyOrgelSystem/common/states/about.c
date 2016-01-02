#include "../StackBasedFsm.h"
#include "../SlotBasedMenu.h"
#include "../canvas/canvas.h"
#include "../../hal/hal_display.h"
#include "../../hal/hal_inputdevice.h"
#include "../../hal/hal_misc.h"
#include "about.h"

static struct {
  // put your local variables here!
  int some_local_variable;

} context;

static void draw() {
  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(CENTER, CENTER, "Version: " VERSION, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  display_redraw();
}

static void onAction(StackBasedFsm_t* pFsm) {
  hal_printf("about::onAction()");
}

static void onBack(StackBasedFsm_t* pFsm) {
  hal_printf("about::onBack()");

  fsmPop(pFsm);
}

static void onEnter(StackBasedFsm_t* pFsm, void* pParams) {
  hal_printf("about::onEnter()");

  draw();
}

static void onReenter(StackBasedFsm_t* pFsm) {
  hal_printf("about::onReenter()");

  draw();
}

static void onLeaveState(StackBasedFsm_t* pFsm) {
  hal_printf("about::onLeaveState()");
}

static void onTick(StackBasedFsm_t* pFsm) {
  
}

static void onDirection(StackBasedFsm_t* pFsm, bool south, bool north, bool west, bool east) {
  draw();

  // hal_printf("onDirection()");

  // TODO: redraw cursor area
}

void about(StackBasedFsm_t* pFsm, FsmState* pState, void* pParams) {
  pState->onAction = onAction;
  pState->onBack = onBack;
  pState->onDirection = onDirection;
  pState->onEnterState = onEnter;
  pState->onReenterState = onReenter;
  pState->onLeaveState = onLeaveState;
  pState->onTick = onTick;

  pState->onEnterState(pFsm, pParams);
}

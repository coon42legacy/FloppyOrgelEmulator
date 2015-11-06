#include "../StackBasedFsm.h"
#include "../SlotBasedMenu.h"
#include "../canvas/canvas.h"
#include "../../hal/hal_display.h"
#include "../../hal/hal_inputdevice.h"
#include "../../hal/hal_misc.h"
#include "about.h"

static struct {
  StackBasedFsm_t* pFsm;

} context;

static void draw() {
  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(CENTER, CENTER, "Version: " VERSION, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  display_redraw();
}

static void onAction() {
  hal_printf("about::onAction()");
}

static void onBack() {
  hal_printf("about::onBack()");

  fsmPop(context.pFsm);
}

static void onEnter(void* pArgs) {
  hal_printf("about::onEnter()");
  context.pFsm = pArgs;

  draw();
}

static void onReenter() {
  hal_printf("about::onReenter()");

  draw();
}

static void onLeaveState() {
  hal_printf("about::onLeaveState()");
}

static void onTick() {
  
}

static void onDirection(bool south, bool north, bool west, bool east) {
  draw();

  // hal_printf("onDirection()");

  // TODO: redraw cursor area
}

void about(FsmState* state, void* pArgs) {
  state->onAction = onAction;
  state->onBack = onBack;
  state->onDirection = onDirection;
  state->onEnterState = onEnter;
  state->onReenterState = onReenter;
  state->onLeaveState = onLeaveState;
  state->onTick = onTick;

  state->onEnterState(pArgs);
}

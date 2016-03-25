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

static void onActionPress(StackBasedFsm_t* pFsm) {
  hal_printf("about::onActionPress()");
}

static void onActionRelease(StackBasedFsm_t* pFsm) {
  hal_printf("about::onActionRelease()");
}

static void onBackPress(StackBasedFsm_t* pFsm) {
  hal_printf("about::onBackPress()");

  leaveState(pFsm);
}

static void onStartPress(StackBasedFsm_t* pFsm) {
  hal_printf("about::onStartPress()");
}

static void onStartRelease(StackBasedFsm_t* pFsm) {
  hal_printf("about::onStartRelease()");
}

static void onSelectPress(StackBasedFsm_t* pFsm) {
  hal_printf("about::onSelectPress()");
}

static void onSelectRelease(StackBasedFsm_t* pFsm) {
  hal_printf("about::onSelectRelease()");
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

static void onDirectionPress(StackBasedFsm_t* pFsm, bool south, bool north, bool west, bool east) {
  draw();

  // hal_printf("onDirection()");

  // TODO: redraw cursor area
}

void about(StackBasedFsm_t* pFsm, FsmState* state) {
  state->onActionPress   = onActionPress;
  state->onActionRelease = onActionRelease;
  state->onBackPress     = onBackPress;
  state->onStartPress    = onStartPress;
  state->onStartRelease  = onStartRelease;
  state->onSelectPress   = onSelectPress;
  state->onSelectRelease = onSelectRelease;
  state->onDirectionPress     = onDirectionPress;
  state->onEnterState    = onEnter;
  state->onReenterState  = onReenter;
  state->onLeaveState    = onLeaveState;
  state->onTick          = onTick;
}

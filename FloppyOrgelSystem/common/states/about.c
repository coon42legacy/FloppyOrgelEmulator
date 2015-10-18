#include "../StackBasedFsm.h"
#include "../SlotBasedMenu.h"
#include "../canvas/canvas.h"
#include "../../hal/hal_display.h"
#include "../../hal/hal_inputdevice.h"
#include "about.h"

static struct {
  // TODO: add variables here
  uint8_t dummy;

} context;

static void onAction() {
  printf("onAction!\n");
}

static void onBack() {
  printf("onBack!\n");
}

static void onEnter(void* pArgs) {
  
}

static void onLeaveState() {

}

static void onTick() {
  canvas_clear(0x00, 0x00, 0x00);

  canvas_drawText(CENTER, CENTER, "Version: " VERSION, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  display_redraw();
}

static void onDirection(bool south, bool north, bool west, bool east) {
  printf("onDirection()\n");

  // TODO: redraw cursor area
}

void about(FsmState* state, void* pArgs) {
  state->onAction = onAction;
  state->onBack = onBack;
  state->onDirection = onDirection;
  state->onEnterState = onEnter;
  state->onLeaveState = onLeaveState;
  state->onTick = onTick;

  state->onEnterState(pArgs);
}

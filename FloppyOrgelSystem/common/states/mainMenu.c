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

static struct {
  LockFreeFIFO_t fifoDebugPort;
  SlotBasedMenu_t menu;
} context;

static void onAction() {
  printf("onAction!\n");
}

static void onBack() {
  printf("onBack!\n");
}

static void onEnter(void* pArgs) {
  StackBasedFsm_t* pFsm = pArgs;

  hal_rs485init(&context.fifoDebugPort); // TODO: move to live mode state?
  userMenuInit(&context.menu, pFsm, 3, 45, onAction, onBack);
  menuAddSlot(&context.menu, "Button Test", buttonTest);
  menuAddSlot(&context.menu, "Play MIDI File", playlist);
  menuAddSlot(&context.menu, "Live Mode", liveMode);
  menuAddSlot(&context.menu, "Settings", settings);
  menuAddSlot(&context.menu, "About", about);

  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 0, "Use the game pad to navigate", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 18, "Press A button to select", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  menuTick(&context.menu);
  menuDraw(&context.menu);
  display_redraw();
}

static void onLeaveState() {

}

static void onTick() {
  
}

static void onDirection(bool south, bool north, bool west, bool east) {
  printf("onDirection()");

  if (south) {
    printf("south");
    menuMoveCursorDown(&context.menu);
  }

  if (north) {
    printf("north");
    menuMoveCursorUp(&context.menu);
  }

  if (west)
    printf("west");

  if (east)
    printf("east");

  printf("\n");

  canvas_clear(0x00, 0x00, 0x00);
  menuDraw(&context.menu);
  display_redraw();

  // TODO: redraw cursor area
}

void mainMenu(FsmState* state, void* pArgs) {
  state->onAction = onAction;
  state->onBack = onBack;
  state->onDirection = onDirection;
  state->onEnterState = onEnter;
  state->onLeaveState = onLeaveState;
  state->onTick = onTick;
 
  state->onEnterState(pArgs);
}

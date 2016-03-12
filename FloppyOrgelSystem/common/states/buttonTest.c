#include "../StackBasedFsm.h"
#include "../canvas/canvas.h"
#include "../../hal/hal_display.h"
#include "../../hal/hal_misc.h"
#include <string.h>

static struct {
  int numActionPresses;
  int numBackPresses;
  int numStartPresses;
  int numSelectPresses;
  int numLeftPresses;
  int numRightPresses;
  int numDownPresses;
  int numUpPresses;

  bool selectIsPressed;
  bool startIsPressed;
} context;

static void draw() {
  hal_printf("buttonTest::draw()");

  char numBuf[32];

  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 0  * 18, "Button Test",                     0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 1  * 18 , "Press Start + Select to leave",  0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  itoa(context.numLeftPresses, numBuf, 10);
  canvas_drawText(0,      3  * 18, "Left: ",                  0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(100,    3  * 18, numBuf,                    0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  itoa(context.numRightPresses, numBuf, 10);
  canvas_drawText(0,      4 * 18, "Right: ",                  0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(100,    4 * 18, numBuf,                     0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  itoa(context.numUpPresses, numBuf, 10);
  canvas_drawText(0,      5 * 18, "Up: ",                     0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(100,    5 * 18, numBuf,                     0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  itoa(context.numDownPresses, numBuf, 10);
  canvas_drawText(0,      6 * 18, "Down: ",                   0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(100,    6 * 18, numBuf,                     0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  itoa(context.numActionPresses, numBuf, 10);
  canvas_drawText(0,      7 * 18, "Action: ",                 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(100,    7 * 18, numBuf,                     0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  itoa(context.numBackPresses, numBuf, 10);
  canvas_drawText(0,      8 * 18, "Back: ",                   0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(100,    8 * 18, numBuf,                     0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  itoa(context.numStartPresses, numBuf, 10);
  canvas_drawText(0,      9 * 18, "Start: ",                  0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(100,    9 * 18, numBuf,                     0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  itoa(context.numSelectPresses, numBuf, 10);
  canvas_drawText(0,      10 * 18, "Select: ",                0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(100,    10 * 18, numBuf,                    0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  display_redraw();
}

static void onActionPress(StackBasedFsm_t* pFsm) {
  hal_printf("buttonTest::onActionPress()");

  context.numActionPresses++;
  draw();
}

static void onBackPress(StackBasedFsm_t* pFsm) {
  hal_printf("buttonTest::onBackPress()");

  context.numBackPresses++;
  draw();
}

static void onStartPress(StackBasedFsm_t* pFsm) {
  hal_printf("buttonTest::onStartPress()");

  context.startIsPressed = true;
  context.numStartPresses++;

  draw();

  if (context.startIsPressed && context.selectIsPressed)
    leaveState(pFsm);
}

static void onStartRelease(StackBasedFsm_t* pFsm) {
  hal_printf("example::onBackRelease()");

  context.startIsPressed = false;
}

static void onSelectPress(StackBasedFsm_t* pFsm) {
  hal_printf("buttonTest::onSelectPress()");

  context.selectIsPressed = true;
  context.numSelectPresses++;

  draw();

  if (context.startIsPressed && context.selectIsPressed)
    leaveState(pFsm); 
}

static void onSelectRelease(StackBasedFsm_t* pFsm) {
  hal_printf("example::onSelectRelease()");

  context.selectIsPressed = false;
}



static void onEnter(StackBasedFsm_t* pFsm, void* pParams) {
  hal_printf("buttonTest::onEnter()");
  memset(&context, 0, sizeof(context));

  draw();
}

static void onTick(StackBasedFsm_t* pFsm) {
  
}

static void onDirectionPress(StackBasedFsm_t* pFsm, bool south, bool north, bool west, bool east) {
  hal_printf("buttonTest::onDirection()");

  if (south)
    context.numDownPresses++;

  if (north)
    context.numUpPresses++;

  if (west)
    context.numLeftPresses++;

  if (east)
    context.numRightPresses++;

  draw();
}

void buttonTest(StackBasedFsm_t* pFsm, FsmState* pState, void* pArgs) {
  pState->onActionPress = onActionPress;
  pState->onBackPress = onBackPress;
  pState->onStartPress = onStartPress;
  pState->onStartRelease = onStartRelease;
  pState->onSelectPress = onSelectPress;
  pState->onSelectRelease = onSelectRelease;
  pState->onEnterState = onEnter;
  pState->onTick = onTick;
  pState->onDirectionPress = onDirectionPress;
}
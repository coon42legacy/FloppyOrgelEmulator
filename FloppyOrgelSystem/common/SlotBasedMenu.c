#include <stdint.h>
#include <string.h>
#include "StackBasedFsm.h"
#include "SlotBasedMenu.h"
#include "../hal/hal_inputdevice.h"
#include "../hal/hal_filesystem.h"
#include "../hal/hal_misc.h"
#include "canvas/canvas.h"

static void menuInit(SlotBasedMenu_t* pSbm, StackBasedFsm_t* pFsm, int16_t xPos, int16_t yPos) {
  pSbm->type = INVALID_MENU;
  pSbm->xPos = xPos;
  pSbm->yPos = yPos;
  pSbm->cursorPos = 0;
  pSbm->numSlots = 0;
  pSbm->pFsm = pFsm;

  memset(pSbm->slot, 0, sizeof(pSbm->slot));
}

void userMenuInit(SlotBasedMenu_t* pSbm, StackBasedFsm_t* pFsm, int16_t xPos, int16_t yPos) {
  menuInit(pSbm, pFsm, xPos, yPos);

  pSbm->type = USER_MENU;
}

void userMenuTransitToSelectedSlot(SlotBasedMenu_t* pMenu, void* pArgs) {
  fsmPush(pMenu->pFsm, pMenu->slot[pMenu->cursorPos].pNextStateTransitionFunc, pArgs);
}

void userMenuTransitBack(SlotBasedMenu_t* pMenu) {
  fsmPop(pMenu->pFsm);
}

static void menuDrawCursor(SlotBasedMenu_t* pSbm) {
  switch (pSbm->type) {
    case USER_MENU:
      canvas_drawImage(pSbm->xPos, pSbm->yPos + 18 * pSbm->cursorPos, _pCursorImg);
      break;

    // case BROWSE_MENU:
    //   canvas_drawImage(pSbm->xPos, pSbm->yPos + 18 * (pSbm->cursorPos % MENU_FILES_PER_PAGE), cursorImg);
    //   break;

    // case SETTINGS_MENU:
    //   canvas_drawImage(pSbm->xPos, pSbm->yPos + 18 * pSbm->cursorPos, cursorImg);
    //   break;
  }
}

void menuAddSlot(SlotBasedMenu_t* pSbm, const char* label, TransitionFunc pFunc) {
  if (pSbm->type != USER_MENU || pSbm->numSlots >= MENU_MAX_SLOTS)
    return;

  hal_strcpy_s(pSbm->slot[pSbm->numSlots].pLabel, MAX_MENU_ITEM_CHARS, label);
  pSbm->slot[pSbm->numSlots].pNextStateTransitionFunc = pFunc;
  pSbm->numSlots++;
}

void menuDraw(SlotBasedMenu_t* sbm) {
  for (int i = 0; i < sbm->numSlots; i++)
    canvas_drawText(sbm->xPos + 28, sbm->yPos - 5 + 18 * i, sbm->slot[i].pLabel, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  menuDrawCursor(sbm);
}

void menuMoveCursorUp(SlotBasedMenu_t* sbm) {
  if (sbm->cursorPos > 0)
    sbm->cursorPos--;
}

void menuMoveCursorDown(SlotBasedMenu_t* sbm) {
  if (sbm->cursorPos + 1 < sbm->numSlots)
    sbm->cursorPos++;
}

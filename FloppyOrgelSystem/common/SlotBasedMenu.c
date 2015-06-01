#include <stdint.h>
#include <string.h>
#include "StackBasedFsm.h"
#include "SlotBasedMenu.h"
#include "../hal/hal_inputdevice.h"
#include "../hal/hal_filesystem.h"
#include "canvas/canvas.h"

static void menuInit(SlotBasedMenu_t* sbm, MenuType_t type, int16_t xPos, int16_t yPos) {
  sbm->type = type;
  sbm->xPos = xPos;
  sbm->yPos = yPos;
  sbm->cursorPos = 0;
  sbm->numSlots = 0;
  memset(sbm->slot, 0, sizeof(sbm->slot));
}

void userMenuInit(SlotBasedMenu_t* sbm, int16_t xPos, int16_t yPos, UserMenuActionCallback onAction, UserMenuBackCallback onBack) {
  menuInit(sbm, USER_MENU, xPos, yPos);
  sbm->userMenu.onAction = onAction;
  sbm->userMenu.onBack = onBack;
}

void browseMenuInit(SlotBasedMenu_t* sbm, int16_t xPos, int16_t yPos, char* filePath, BrowseMenuActionCallback onAction, BrowseMenuBackCallback onBack) {
  menuInit(sbm, BROWSE_MENU, xPos, yPos);
  sbm->browseMenu.filePath = filePath;
  sbm->browseMenu.onAction = onAction;
  sbm->browseMenu.onBack = onBack;
}

static void menuDrawCursor(SlotBasedMenu_t* sbm) {
  canvas_drawRect(sbm->xPos, sbm->yPos + 18 * sbm->cursorPos, 5, 5, 255, 255, 255);
}

static void getFileNameFromCursorPos(char* srcPath, char* dstFilePath, int cursorPos) {
  static FO_FIND_DATA findData;

  strcpy(dstFilePath, srcPath);

  bool endOfDirectory = !hal_findInit(dstFilePath, &findData);
  int itemCount = 0;

  while (!endOfDirectory) {
    if (findData.fileName[0] != '.' && findData.fileName[1] != '.')
    if (itemCount++ == cursorPos) {
      strcat(dstFilePath, "/");
      strcat(dstFilePath, findData.fileName);
      break;
    }

    if (!hal_findNext(&findData))
      endOfDirectory = true;
  }

  hal_findFree();
}

void menuTick(SlotBasedMenu_t* sbm, StackBasedFsm_t* fsm) {
  InputDeviceStates_t buttonPressed = getInputDeviceState();

  if (buttonPressed.South && sbm->cursorPos + 1 < sbm->numSlots)
    sbm->cursorPos++;
  else if (buttonPressed.North && sbm->cursorPos > 0)
    sbm->cursorPos--;

  switch (sbm->type) {
    case USER_MENU: {
      FsmStateFunc curState = fsm->stack[fsm->stackSize_ - 1];

      if (buttonPressed.Action && sbm->userMenu.onAction)
        sbm->userMenu.onAction(fsm, curState, sbm->slot[sbm->cursorPos].pNextFsmStateFunc);
      else if (buttonPressed.Back && sbm->userMenu.onBack)
        sbm->userMenu.onBack(fsm, curState);
      break;
    }

    case BROWSE_MENU: {
      static char fileNameOnCursor[256];
      static FO_FIND_DATA findData;

      if (buttonPressed.Action) {
        getFileNameFromCursorPos(sbm->browseMenu.filePath, fileNameOnCursor, sbm->cursorPos);
        sbm->browseMenu.onAction(fsm, fileNameOnCursor);
      }
      else if (buttonPressed.Back)
        sbm->browseMenu.onBack(fsm);
      
      // Draw tracks
      sbm->numSlots = 0;
      bool endOfDirectory = !hal_findInit(sbm->browseMenu.filePath, &findData);

      while (!endOfDirectory) {
        if (findData.fileName[0] != '.')
        if (findData.fileName[1] != '.')
          canvas_drawText(sbm->xPos + 10, sbm->yPos - 5 + 18 * sbm->numSlots++, findData.fileName, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

        if (!hal_findNext(&findData))
          endOfDirectory = true;
      }
      hal_findFree();
      menuDrawCursor(sbm);
      //

      break;
    }
  }
}

void menuAddSlot(SlotBasedMenu_t* sbm, char* label, FsmStateFunc pTransitionStateFunc) {
  if (sbm->type != USER_MENU || sbm->numSlots >= MENU_MAX_SLOTS)
    return;

  sbm->slot[sbm->numSlots].pLabel = label;
  sbm->slot[sbm->numSlots].pNextFsmStateFunc = pTransitionStateFunc;
  sbm->numSlots++;
}

void menuDraw(SlotBasedMenu_t* sbm) {
  for (int i = 0; i < sbm->numSlots; i++) 
    canvas_drawText(sbm->xPos + 10, sbm->yPos - 5 + 18 * i, sbm->slot[i].pLabel, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  menuDrawCursor(sbm);
}

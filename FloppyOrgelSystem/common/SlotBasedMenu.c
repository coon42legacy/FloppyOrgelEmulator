#include <stdint.h>
#include <string.h>
#include "StackBasedFsm.h"
#include "SlotBasedMenu.h"
#include "../hal/hal_inputdevice.h"
#include "../hal/hal_filesystem.h"
#include "canvas/canvas.h"

static void menuInit(SlotBasedMenu_t* pSbm, StackBasedFsm_t* pFsm, MenuType_t type, int16_t xPos, int16_t yPos) {
  pSbm->type = type;
  pSbm->xPos = xPos;
  pSbm->yPos = yPos;
  pSbm->cursorPos = 0;
  pSbm->numSlots = 0;
  pSbm->pFsm = pFsm;
  memset(pSbm->slot, 0, sizeof(pSbm->slot));
}

void userMenuInit(SlotBasedMenu_t* pSbm, StackBasedFsm_t* pFsm, int16_t xPos, int16_t yPos) {
  menuInit(pSbm, pFsm, USER_MENU, xPos, yPos);
}

void userMenuTransitToSelectedSlot(SlotBasedMenu_t* pMenu) {
  fsmPush(pMenu->pFsm, pMenu->slot[pMenu->cursorPos].pNextStateTransitionFunc, pMenu->pFsm);
}

void userMenuTransitBack(SlotBasedMenu_t* pMenu) {
  fsmPop(pMenu->pFsm);
}

void settingsMenuInit(SlotBasedMenu_t* pSbm, StackBasedFsm_t* pFsm, int16_t xPos, int16_t yPos) {
  menuInit(pSbm, pFsm, SETTINGS_MENU, xPos, yPos);
}

void browseMenuInit(SlotBasedMenu_t* pSbm, StackBasedFsm_t* pFsm, int16_t xPos, int16_t yPos, char* filePath, BrowseNewPageCallback onNewPage) {
  menuInit(pSbm, pFsm, BROWSE_MENU, xPos, yPos);
  // pSbm->browseMenu.filePath = filePath;
  // pSbm->browseMenu.onNewPage = onNewPage;
  // pSbm->browseMenu.curPage = 1;
}

static void menuDrawCursor(SlotBasedMenu_t* pSbm) {
  switch (pSbm->type) {
  case USER_MENU:
    canvas_drawImage(pSbm->xPos, pSbm->yPos + 18 * pSbm->cursorPos, cursorImg);
    break;

  case BROWSE_MENU:
    canvas_drawImage(pSbm->xPos, pSbm->yPos + 18 * (pSbm->cursorPos % MENU_FILES_PER_PAGE), cursorImg);
    break;

  case SETTINGS_MENU:    
    canvas_drawImage(pSbm->xPos, pSbm->yPos + 18 * pSbm->cursorPos, cursorImg);
    break;
  }
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

uint32_t getNumFiles(char* filePath) {
  static FO_FIND_DATA findData;
  bool endOfDirectory = !hal_findInit(filePath, &findData);
  int numFiles = 0;

  while (!endOfDirectory) {
    if (findData.fileName[0] != '.')
    if (findData.fileName[1] != '.')
      numFiles++;

    if (!hal_findNext(&findData))
      endOfDirectory = true;
  }
  hal_findFree();

  return numFiles;
}

uint32_t getNumPages(uint32_t numFiles, uint32_t filesPerPage) {
  return numFiles % filesPerPage == 0 ? numFiles / filesPerPage : numFiles / filesPerPage + 1;
}

// TODO: obsolete! remove soon!
void menuTick(SlotBasedMenu_t* sbm) {
  switch (sbm->type) {
    case USER_MENU: {      
      // if (buttonPressed.Action && sbm->userMenu.onAction) {
      //   sbm->userMenu.onAction();
      //   fsmPush(sbm->pFsm, sbm->slot[sbm->cursorPos].pNextStateTransitionFunc, NULL);
      // }
      // else if (buttonPressed.Back && sbm->userMenu.onBack)
      //   sbm->userMenu.onBack();
      // break;
    }

    case SETTINGS_MENU: {
      // if (buttonPressed.Action && sbm->settingsMenu.onAction)
      //   sbm->settingsMenu.onAction();
      // else if (buttonPressed.Back && sbm->settingsMenu.onBack)
      //   sbm->settingsMenu.onBack();

      break;
    }

    case BROWSE_MENU: {
      static char fileNameOnCursor[256];

      // if (buttonPressed.Action) {
      //   getFileNameFromCursorPos(sbm->browseMenu.filePath, fileNameOnCursor, sbm->cursorPos);
      //   sbm->browseMenu.onAction(fileNameOnCursor);
      // }
      // else if (buttonPressed.Back)
      //   sbm->browseMenu.onBack();

      
    }
  }
}

void menuAddSlot(SlotBasedMenu_t* pSbm, char* label, TransitionFunc pFunc) {
  if (pSbm->type != USER_MENU || pSbm->numSlots >= MENU_MAX_SLOTS)
    return;

  pSbm->slot[pSbm->numSlots].pLabel = label;
  pSbm->slot[pSbm->numSlots].pNextStateTransitionFunc = pFunc;
  pSbm->numSlots++;
}

void menuAddSettingsSlot(SlotBasedMenu_t* sbm, char* label) {

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

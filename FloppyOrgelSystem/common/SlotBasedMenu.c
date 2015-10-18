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

void userMenuInit(SlotBasedMenu_t* pSbm, StackBasedFsm_t* pFsm, int16_t xPos, int16_t yPos, UserMenuActionCallback onAction, UserMenuBackCallback onBack) {
  menuInit(pSbm, pFsm, USER_MENU, xPos, yPos);
  pSbm->userMenu.onAction = onAction;
  pSbm->userMenu.onBack = onBack;
}

void settingsMenuInit(SlotBasedMenu_t* pSbm, StackBasedFsm_t* pFsm, int16_t xPos, int16_t yPos, SettingsMenuSaveCallback onSave, SettingsMenuCancelCallback onCancel) {
  menuInit(pSbm, pFsm, SETTINGS_MENU, xPos, yPos);
  pSbm->settingsMenu.onAction = onSave;
  pSbm->settingsMenu.onBack = onCancel;
}

void browseMenuInit(SlotBasedMenu_t* pSbm, StackBasedFsm_t* pFsm, int16_t xPos, int16_t yPos, char* filePath, BrowseMenuActionCallback onAction, BrowseMenuBackCallback onBack, BrowseNewPageCallback onNewPage) {
  menuInit(pSbm, pFsm, BROWSE_MENU, xPos, yPos);
  pSbm->browseMenu.filePath = filePath;
  pSbm->browseMenu.onAction = onAction;
  pSbm->browseMenu.onBack = onBack;
  pSbm->browseMenu.onNewPage = onNewPage;
  pSbm->browseMenu.curPage = 1;
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
      static FO_FIND_DATA findData;

      // if (buttonPressed.Action) {
      //   getFileNameFromCursorPos(sbm->browseMenu.filePath, fileNameOnCursor, sbm->cursorPos);
      //   sbm->browseMenu.onAction(fileNameOnCursor);
      // }
      // else if (buttonPressed.Back)
      //   sbm->browseMenu.onBack();

      int numPages = getNumPages(getNumFiles(sbm->browseMenu.filePath), MENU_FILES_PER_PAGE);
      int page = sbm->cursorPos / MENU_FILES_PER_PAGE + 1;
      
      // Draw tracks of current page
      sbm->numSlots = getNumFiles(sbm->browseMenu.filePath);
      bool endOfDirectory = !hal_findInit(sbm->browseMenu.filePath, &findData);
      int i = 0;
      int itemCount = 0;
      
      while (!endOfDirectory) {
        if (findData.fileName[0] != '.')
          if (findData.fileName[1] != '.')
            if (i++ >= (page - 1) * MENU_FILES_PER_PAGE)
              canvas_drawText(sbm->xPos + 27, sbm->yPos - 5 + 18 * itemCount++, findData.fileName, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

        if (!hal_findNext(&findData) || i >= (page - 1) * MENU_FILES_PER_PAGE + MENU_FILES_PER_PAGE)
          endOfDirectory = true;
      }
      hal_findFree();
      menuDrawCursor(sbm);
      
      sbm->browseMenu.onNewPage(page, numPages); // FIXME: implement correctly!
      //

      break;
    }
  }
}

void menuAddSlot(SlotBasedMenu_t* sbm, char* label, TransitionFunc pFunc) {
  if (sbm->type != USER_MENU || sbm->numSlots >= MENU_MAX_SLOTS)
    return;

  sbm->slot[sbm->numSlots].pLabel = label;
  sbm->slot[sbm->numSlots].pNextStateTransitionFunc = pFunc;
  sbm->numSlots++;
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

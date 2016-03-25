#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../../StackBasedFsm.h"
#include "../../SlotBasedMenu.h"
#include "../../canvas/canvas.h"
#include "../../../hal/hal_inputdevice.h"
#include "../../../hal/hal_mididevice.h"
#include "../../../hal/hal_display.h"
#include "../../../hal/hal_misc.h"
#include "../../../hal/hal_filesystem.h"
#include "../../embedded-midilib/midiutil.h"
#include "../../embedded-midilib/midiplayer.h"
#include "startPlayBack.h"
#include "playlist.h"

// ------------------------------------------------------------------------------------------------------------
// playlist
// ------------------------------------------------------------------------------------------------------------

static struct {  
  SlotBasedMenu_t menu;
  char filePath[256];
  FO_FIND_DATA findData;
} context;

static void onBrowseNewPage(int currentPage, int totalPages) {
  hal_printf("playlist::onBrowseNewPage()");

  // Set tracks of current page
  bool endOfDirectory = !hal_findInit(context.filePath, &context.findData);
  int curFileIndex = 0;
  
  context.menu.numSlots = 0; // Clear all slots.

  while (!endOfDirectory) {
    if (context.findData.fileName[0] != '.' && context.findData.fileName[1] != '.') {
      if (curFileIndex >= (currentPage - 1) * MENU_FILES_PER_PAGE) {        
        menuAddSlot(&context.menu, context.findData.fileName, startPlayBack);

        curFileIndex++;
      }
    }

    endOfDirectory = !hal_findNext(&context.findData) || curFileIndex >= (currentPage - 1) * MENU_FILES_PER_PAGE + MENU_FILES_PER_PAGE;
  }
  hal_findFree();

  // Update page number
  char pageText[32];
  sprintf(pageText, "%2d / %2d", currentPage, totalPages); // TODO: hal_sprintf?

  canvas_drawText(255, 220, pageText, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
}

static void getFilePathFromCursorPos(char* dstFilePath, int cursorPos) {
  static FO_FIND_DATA findData;

  strcpy(dstFilePath, MIDI_PATH);
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

static uint32_t getNumFiles(char* filePath) {
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

static uint32_t getNumPages(uint32_t numFiles, uint32_t filesPerPage) {
  bool isPageCountEven = numFiles % filesPerPage == 0;
  int pageCount = isPageCountEven ? numFiles / filesPerPage : numFiles / filesPerPage + 1;

  return pageCount;
}

static void draw() {
  canvas_clear(0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 0, "Use the game pad to select a song", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
  canvas_drawText(CENTER, 18, "Press A button to start", 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);

  int numFilesTotal = getNumFiles(context.filePath);
  int numPages = getNumPages(numFilesTotal, MENU_FILES_PER_PAGE);
  int curPage = numPages > 0 ? context.menu.cursorPos / MENU_FILES_PER_PAGE + 1 : 0;
  int numFilesOfCurrentPage = curPage * MENU_FILES_PER_PAGE < numFilesTotal ? MENU_FILES_PER_PAGE : MENU_FILES_PER_PAGE - (curPage * MENU_FILES_PER_PAGE - numFilesTotal); // TODO: simplify

  if (numFilesTotal > 0)
    onBrowseNewPage(curPage, numPages); // FIXME: implement correctly!
  else {
    canvas_drawText(CENTER, context.menu.yPos - 5 + 3 * 18, "No tracks available!", 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00);
    canvas_drawText(CENTER, context.menu.yPos - 5 + 4 * 18, "SD-Card missing?", 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00);
  }

  menuDraw(&context.menu);
  display_redraw();
}

static void onAction(StackBasedFsm_t* pFsm) {
  hal_printf("playlist::onAction()");

  char path[256];
  getFilePathFromCursorPos(path, context.menu.cursorPos);
  userMenuTransitToSelectedSlot(&context.menu, path);
}

static void onBack(StackBasedFsm_t* pFsm) {
  hal_printf("playlist::onBack()");

  leaveState(pFsm);
}

static void onEnter(StackBasedFsm_t* pFsm, void* pParams) {
  hal_printf("playlist::onEnter()");  

  userMenuInit(&context.menu, pFsm, 3, 50);
  strcpy(context.filePath, MIDI_PATH);

  draw();
}

static void onReenter(StackBasedFsm_t* pFsm) {
  hal_printf("playlist::onReenter()");

  draw();
}

static void onLeaveState(StackBasedFsm_t* pFsm) {
  hal_printf("playlist::onLeaveState()");
}

static void onTick(StackBasedFsm_t* pFsm) {
  // hal_printf("playlist::onTick()");
}

static void onDirectionPress(StackBasedFsm_t* pFsm, bool south, bool north, bool west, bool east) {
  hal_printf("playlist::onDirection()");

  if (south)
    menuMoveCursorDown(&context.menu);

  if (north)
    menuMoveCursorUp(&context.menu);

  draw();
}

void playlist(StackBasedFsm_t* pFsm, FsmState* pState, void* pParams) {
  pState->onActionPress = onAction;
  pState->onBackPress = onBack;
  pState->onDirectionPress = onDirectionPress;
  pState->onEnterState = onEnter;
  pState->onReenterState = onReenter;
  pState->onLeaveState = onLeaveState;
  pState->onTick = onTick;
}

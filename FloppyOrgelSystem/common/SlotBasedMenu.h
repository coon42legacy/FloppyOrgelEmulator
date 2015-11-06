#ifndef __SLOT_BASED_FSM_H
#define __SLOT_BASED_FSM_H

#include <stdint.h>

#define MENU_MAX_SLOTS 12
#define MENU_FILES_PER_PAGE 10

typedef enum {
  USER_MENU,
  SETTINGS_MENU,
  BROWSE_MENU
} MenuType_t;

typedef struct {
  char* pLabel;
  TransitionFunc pNextStateTransitionFunc;
} MenuSlot_t;

typedef void (*BrowseNewPageCallback)(int currentPage, int totalPages);

// settings menu callbacks
typedef void(*SettingsMenuSaveCallback)();
typedef void(*SettingsMenuCancelCallback)();


typedef struct {
  MenuType_t type;
   
  // struct {
  //   int curPage;
  //   BrowseNewPageCallback onNewPage;
  // } browseMenu;

  uint16_t xPos;
  uint16_t yPos;
  uint8_t numSlots;
  uint8_t cursorPos;
  StackBasedFsm_t* pFsm;
  MenuSlot_t slot[MENU_MAX_SLOTS]; 
} SlotBasedMenu_t;

// general menu functions
void menuTick(SlotBasedMenu_t* sbm);
void menuAddSlot(SlotBasedMenu_t* pSbm, char* label, TransitionFunc pFunc);
void menuDraw(SlotBasedMenu_t* sbm);
void menuMoveCursorUp(SlotBasedMenu_t* sbm);
void menuMoveCursorDown(SlotBasedMenu_t* sbm);

// user menu specific functions
void userMenuInit(SlotBasedMenu_t* pSbm, StackBasedFsm_t* pFsm, int16_t xPos, int16_t yPos);
void userMenuTransitToSelectedSlot(SlotBasedMenu_t* pMenu);
void userMenuTransitBack(SlotBasedMenu_t* pMenu);

// settings menu specific functions
void settingsMenuInit(SlotBasedMenu_t* pSbm, StackBasedFsm_t* pFsm, int16_t xPos, int16_t yPos);

// browse menu specific functions
void browseMenuInit(SlotBasedMenu_t* pSbm, StackBasedFsm_t* pFsm, int16_t xPos, int16_t yPos, char* filePath, BrowseNewPageCallback onNewPage);

#endif // __SLOT_BASED_FSM_H

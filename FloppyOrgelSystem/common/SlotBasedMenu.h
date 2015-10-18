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

// user menu callbacks
typedef void (*UserMenuActionCallback)();
typedef void (*UserMenuBackCallback)();

// browse menu callbacks
typedef void (*BrowseMenuActionCallback)(char* filePath);
typedef void (*BrowseMenuBackCallback)(fsm);
typedef void (*BrowseNewPageCallback)(int currentPage, int totalPages);

// settings menu callbacks
typedef void(*SettingsMenuSaveCallback)();
typedef void(*SettingsMenuCancelCallback)();


typedef struct {
  MenuType_t type;

  union {
    struct {
      UserMenuActionCallback onAction;
      UserMenuBackCallback onBack;
    } userMenu;

    struct {
      SettingsMenuSaveCallback onAction;
      SettingsMenuCancelCallback onBack;
    } settingsMenu;

    struct {
      int curPage;
      char* filePath;
      BrowseMenuActionCallback onAction;
      BrowseMenuBackCallback onBack;
      BrowseNewPageCallback onNewPage;
    } browseMenu;
  };

  uint16_t xPos;
  uint16_t yPos;
  uint8_t numSlots;
  uint8_t cursorPos;
  StackBasedFsm_t* pFsm;
  MenuSlot_t slot[MENU_MAX_SLOTS]; 
} SlotBasedMenu_t;

// general menu functions
void menuTick(SlotBasedMenu_t* sbm);
void menuAddSlot(SlotBasedMenu_t* sbm, char* label, TransitionFunc pTransitionStateFunc);
void menuDraw(SlotBasedMenu_t* sbm);
void menuMoveCursorUp(SlotBasedMenu_t* sbm);
void menuMoveCursorDown(SlotBasedMenu_t* sbm);

// user menu specific functions
void userMenuInit(SlotBasedMenu_t* pSbm, StackBasedFsm_t* pFsm, int16_t xPos, int16_t yPos, UserMenuActionCallback onAction, UserMenuBackCallback onBack);

// browse menu specific functions
void browseMenuInit(SlotBasedMenu_t* pSbm, StackBasedFsm_t* pFsm, int16_t xPos, int16_t yPos, char* filePath, BrowseMenuActionCallback onAction, BrowseMenuBackCallback onBack, BrowseNewPageCallback onNewPage);

// settings menu specific functions
void settingsMenuInit(SlotBasedMenu_t* pSbm, StackBasedFsm_t* pFsm, int16_t xPos, int16_t yPos, SettingsMenuSaveCallback onSave, SettingsMenuCancelCallback onCancel);

#endif // __SLOT_BASED_FSM_H

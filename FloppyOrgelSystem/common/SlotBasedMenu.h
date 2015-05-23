#ifndef __SLOT_BASED_FSM_H
#define __SLOT_BASED_FSM_H

#define MENU_MAX_SLOTS 12

typedef struct {
  char* pLabel;
  FsmStateFunc pNextFsmStateFunc;
} MenuSlot_t;

typedef struct {
  uint8_t numSlots;
  uint8_t cursorPos;
  MenuSlot_t slot[MENU_MAX_SLOTS];
} SlotBasedMenu_t;

void menuTick(SlotBasedMenu_t* sbm, StackBasedFsm_t* fsm);
void menuAddSlot(SlotBasedMenu_t* sbm, char* label, FsmStateFunc pTransitionStateFunc);
void menuDraw(SlotBasedMenu_t* sbm);

#endif // __SLOT_BASED_FSM_H
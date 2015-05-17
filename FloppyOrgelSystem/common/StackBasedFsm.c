#include <stdbool.h>
#include <stdio.h>
#include "StackBasedFsm.h"

void fsmInit(StackBasedFsm_t* fsm) {
  fsm->stackSize_ = 0;
}

bool fsmPush(StackBasedFsm_t* fsm, void* state) {
  if (fsm->stackSize_ < FSM_STACK_SIZE) {
    fsm->stack[fsm->stackSize_++] = state;
    return true;
  }
  else
    return false;
}

bool fsmPop(StackBasedFsm_t* fsm) {
  if (fsm->stackSize_ > 0) {
    fsm->stackSize_--;
    return true;
  }
  else
    return false;
}

void* fsmGetCurrentState(StackBasedFsm_t* fsm) {
  if (fsm->stackSize_ > 0)
    return fsm->stack[fsm->stackSize_ - 1];
  else
    return NULL;
}

void fsmTick(StackBasedFsm_t* fsm) {
  void(*stateFunc)(StackBasedFsm_t* fsm) = fsmGetCurrentState(fsm);

  if (stateFunc != NULL)
    stateFunc(fsm);
}

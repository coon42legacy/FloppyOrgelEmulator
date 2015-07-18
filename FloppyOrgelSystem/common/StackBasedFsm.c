#include <stdbool.h>
#include <stdio.h>
#include "StackBasedFsm.h"

void fsmInit(StackBasedFsm_t* fsm) {
  fsm->stackSize_ = 0;
}

bool fsmPush(StackBasedFsm_t* fsm, FsmStateFunc stateFunc) {
  if (stateFunc != NULL && fsm->stackSize_ < FSM_STACK_SIZE) {
    fsm->stack[fsm->stackSize_++] = stateFunc;
    fsm->firstRunOfCurrentState = true;
    return true;
  }
  else
    return false;
}

bool fsmPop(StackBasedFsm_t* fsm) {
  if (fsm->stackSize_ > 1) {
    fsm->stackSize_--;
    fsm->firstRunOfCurrentState = true;
    return true;
  }
  else
    return false;
}

FsmState* fsmGetCurrentState(StackBasedFsm_t* fsm) {
  if (fsm->stackSize_ > 0)
    return fsm->stack[fsm->stackSize_ - 1];
  else
    return NULL;
}

void fsmTick(StackBasedFsm_t* fsm) {
  FsmStateFunc pStateFunc = fsmGetCurrentState(fsm);

  if (pStateFunc != NULL)
    pStateFunc(fsm);
}

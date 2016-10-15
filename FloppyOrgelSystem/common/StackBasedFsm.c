#include <stdbool.h>
#include <stdio.h>
#include "InputDevice.h"
#include "../hal/hal_misc.h"
#include "StackBasedFsm.h"

// CHECKME: decouple state handling from fsm?

static void initStateCallBacks(FsmState* pState) {
  pState->onActionPress    = NULL;
  pState->onActionRelease  = NULL;
  pState->onBackPress      = NULL;
  pState->onBackRelease    = NULL;
  pState->onStartPress     = NULL;
  pState->onStartRelease   = NULL;
  pState->onSelectPress    = NULL;
  pState->onSelectRelease  = NULL;
  pState->onDirectionPress = NULL;
  pState->onEnterState     = NULL;
  pState->onLeaveState     = NULL;
  pState->onReenterState   = NULL;
  pState->onTick           = NULL;
}

static bool isCallbackDefined(void* pCallBack, char* pCallBackName) {
  if (!pCallBack) {
    hal_printfError("Error on state transition: '%s' callback not set!", pCallBackName);
    return false;
  }

  return true;
}

static bool checkStateCallbacks(FsmState* pState) {
  bool callBacksOk = true;

  // TODO: make all callbacks optional and print warning instead?

  // callBacksOk &= isCallbackDefined(pState->onActionPress,   "onActionPress");
  // callBacksOk &= isCallbackDefined(pState->onActionRelease, "onActionRelease"); // TODO: make optional?
  // callBacksOk &= isCallbackDefined(pState->onBackPress,     "onBackPress");
  // callBacksOk &= isCallbackDefined(pState->onBackRelease,   "onBackRelease");
  // callBacksOk &= isCallbackDefined(pState->onStartPress,    "onStartPress");
  // callBacksOk &= isCallbackDefined(pState->onStartRelease,  "onStartRelease");
  // callBacksOk &= isCallbackDefined(pState->onSelectPress,   "onSelectPress");
  // callBacksOk &= isCallbackDefined(pState->onSelectRelease, "onSelectRelease");
  // callBacksOk &= isCallbackDefined(pState->onDirection,     "onDirection");
  callBacksOk &= isCallbackDefined(pState->onEnterState,    "onEnterState");
  // callBacksOk &= isCallbackDefined(pState->onLeaveState,    "onLeaveState");
  // callBacksOk &= isCallbackDefined(pState->onReenterState,  "onReenterState");
  // callBacksOk &= isCallbackDefined(pState->onTick,          "onTick");

  return callBacksOk;
}

void fsmInit(StackBasedFsm_t* pFsm) {
  pFsm->stackSize_ = 0;
}

bool fsmPush(StackBasedFsm_t* pFsm, TransitionFunc pStateFunc, void* pParams) {
  FsmState* pCurrentState = fsmGetCurrentState(pFsm);

  if (pFsm->stackSize_ < FSM_STACK_SIZE) {
    FsmState* pNextState = &pFsm->stack[pFsm->stackSize_];
    pFsm->stackSize_++;

    if (pCurrentState)
      pCurrentState->onLeaveState(pFsm);

    initStateCallBacks(pNextState);
    pStateFunc(pFsm, pNextState);

    if (checkStateCallbacks(pNextState))
      pNextState->onEnterState(pFsm, pParams);
    else {
      hal_printfWarning("Please define missing callbacks! Going back to previous state.");

      fsmPop(pFsm);
      return false;
    }

    return true;
  }
  else
    return false;
}

bool fsmPop(StackBasedFsm_t* pFsm) {
  FsmState* pState;

  if (pFsm->stackSize_ > 1) {
    pState = fsmGetCurrentState(pFsm);
    if (pState && pState->onLeaveState)
      pState->onLeaveState(pFsm);

    pFsm->stackSize_--;

    pState = fsmGetCurrentState(pFsm);
    if (pState && pState->onReenterState)
      pState->onReenterState(pFsm);

    return true;
  }
  else
    return false;
}

bool leaveState(StackBasedFsm_t* pFsm) {
  return fsmPop(pFsm);
}

FsmState* fsmGetCurrentState(StackBasedFsm_t* pFsm) {
  if (pFsm->stackSize_ > 0)
    return &pFsm->stack[pFsm->stackSize_ - 1];
  else
    return NULL;
}

void fsmTick(StackBasedFsm_t* pFsm) {
  FsmState* pState = fsmGetCurrentState(pFsm);

  processInputDevice(pFsm);

  if (pState)
    if (pState->onTick)
      pState->onTick(pFsm);
}

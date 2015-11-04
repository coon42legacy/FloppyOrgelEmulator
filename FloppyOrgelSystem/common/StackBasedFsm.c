#include <stdbool.h>
#include <stdio.h>
#include "StackBasedFsm.h"
#include "../hal/hal_inputdevice.h" // TODO: decouple somehow?
#include "../hal/hal_misc.h"

void fsmInit(StackBasedFsm_t* fsm) {
  fsm->stackSize_ = 0;
}

bool fsmPush(StackBasedFsm_t* fsm, TransitionFunc pFunc, void* pArgs) {
  FsmState* pState;

  if (fsm->stackSize_ < FSM_STACK_SIZE) {
    pState = fsmGetCurrentState(fsm);
    if (pState)
      pState->onLeaveState();

    pFunc(&fsm->stack[fsm->stackSize_++], pArgs);
    return true;
  }
  else
    return false;
}

bool fsmPop(StackBasedFsm_t* fsm) {
  FsmState* pState;

  if (fsm->stackSize_ > 1) {
    pState = fsmGetCurrentState(fsm);
    if (pState)
      pState->onLeaveState();
    
    fsm->stackSize_--;

    pState = fsmGetCurrentState(fsm);
    if (pState)
      pState->onReenterState();

    return true;
  }
  else
    return false;
}

FsmState* fsmGetCurrentState(StackBasedFsm_t* fsm) {
  if (fsm->stackSize_ > 0)
    return &fsm->stack[fsm->stackSize_ - 1];
  else
    return NULL;
}

void fsmTick(StackBasedFsm_t* fsm) {
  FsmState* pState = fsmGetCurrentState(fsm);
  InputDeviceStates_t buttonPressed = getInputDeviceState();

  if (buttonPressed.Action)
    if (pState->onAction)
      pState->onAction();

  if (buttonPressed.Back)
    if (pState->onBack)
      pState->onBack();

  if (buttonPressed.South || buttonPressed.North || buttonPressed.West || buttonPressed.East)
    if (pState->onDirection)
      pState->onDirection(buttonPressed.South, buttonPressed.North, buttonPressed.West, buttonPressed.East);

  if (pState)
    if (pState->onTick)
      pState->onTick(NULL);
    else
      hal_printf("onTick() callback not defined!");
}

#include <stdbool.h>
#include <stdio.h>
#include "StackBasedFsm.h"
#include "../hal/hal_inputdevice.h" // TODO: decouple somehow?
#include "../hal/hal_misc.h"

static void initState(FsmState* pState) {
  pState->onAction       = NULL;
  pState->onBack         = NULL;
  pState->onDirection    = NULL;
  pState->onEnterState   = NULL;
  pState->onLeaveState   = NULL;
  pState->onReenterState = NULL;
  pState->onTick         = NULL;
}

static void callBackMissingError(char* callBackName) {
  hal_printfError("Error on state transition: '%s' callback not set!");
}

static void checkState(FsmState* pState) {
  if (!pState->onAction) 
    callBackMissingError("onAction");

  if(!pState->onBack)
    callBackMissingError("onBack");

  if (!pState->onDirection)
    callBackMissingError("onDirection");

  if (!pState->onEnterState)
    callBackMissingError("onEnterState");

  if (!pState->onLeaveState)
    callBackMissingError("onLeaveState");
    
  if (!pState->onReenterState)
    callBackMissingError("onReenterState");

  if(!pState->onTick)
    callBackMissingError("onTick");
}

void fsmInit(StackBasedFsm_t* fsm) {
  fsm->stackSize_ = 0;
}

bool fsmPush(StackBasedFsm_t* fsm, TransitionFunc pFunc, void* pArgs) {
  FsmState* pCurrentState = fsmGetCurrentState(fsm);

  if (fsm->stackSize_ < FSM_STACK_SIZE) {
    FsmState* pNextState = &fsm->stack[fsm->stackSize_];
    fsm->stackSize_++;

    if (pCurrentState)
      pCurrentState->onLeaveState();
    
    initState(pNextState);            
    pFunc(pNextState, pArgs);



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

static bool anyDirectionIsPressed(InputDeviceStates_t state) {
  return state.South || state.North || state.West || state.East;
}

static bool isLastDirectionEqual(InputDeviceStates_t currentState, InputDeviceStates_t lastState) {
  uint16_t currentDirectionCode = currentState.South | currentState.North | currentState.West | 
    currentState.East;

  uint16_t lastDirectionCode = lastState.South | lastState.North | lastState.West | lastState.East;

  return currentDirectionCode == lastDirectionCode;
}

void fsmTick(StackBasedFsm_t* fsm) {
  FsmState* pState = fsmGetCurrentState(fsm);
  InputDeviceStates_t buttonPressed = getInputDeviceState();
  static InputDeviceStates_t lastState = { 0 };
  static uint32_t timeOnLastDirectionPress = 0;
  static uint32_t timeOnLastRepetition = 0;
  static bool isInRepetitionMode = false;

  if (buttonPressed.Action && !lastState.Action)
    if (pState->onAction)
      pState->onAction();

  if (buttonPressed.Back && !lastState.Back)
    if (pState->onBack)
      pState->onBack();

  // TODO: simplify
  // Cursor delay and repetition, when holding direction button
  if (anyDirectionIsPressed(buttonPressed)) {
    if (isLastDirectionEqual(buttonPressed, lastState)) {
      if (isInRepetitionMode) {
        if (hal_clock() > timeOnLastDirectionPress + CURSOR_DELAY_MS_BEFORE_REPEAT) {
          if (hal_clock() > timeOnLastRepetition + 1000 / CURSOR_SPEED_ITEMS_PER_SECOND) {
            if (pState->onDirection)
              pState->onDirection(buttonPressed.South, buttonPressed.North, buttonPressed.West, buttonPressed.East);

            // hal_printf("timeOnLastRepetition: %d, repeat on: %d", timeOnLastRepetition, timeOnLastRepetition + 1000 / CURSOR_SPEED_ITEMS_PER_SECOND);

            timeOnLastRepetition = hal_clock();
          }
        }
      }
      else {
        timeOnLastDirectionPress = hal_clock();
        timeOnLastRepetition = hal_clock();
        isInRepetitionMode = true;

        // hal_printf("In repetition mode");
        if (pState->onDirection)
          pState->onDirection(buttonPressed.South, buttonPressed.North, buttonPressed.West, buttonPressed.East);
      }
    }
  }
  else {
    if (isInRepetitionMode) {
      isInRepetitionMode = false;
      // hal_printf("End of repetition mode");
    }
  }    
  
  if (pState)
    if (pState->onTick)
      pState->onTick(NULL);
    else
      hal_printf("onTick() callback not defined!");

  lastState = buttonPressed;
}

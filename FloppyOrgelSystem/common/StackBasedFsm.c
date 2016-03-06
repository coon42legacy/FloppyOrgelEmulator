#include <stdbool.h>
#include <stdio.h>
#include "StackBasedFsm.h"
#include "../hal/hal_inputdevice.h" // TODO: decouple somehow?
#include "../hal/hal_misc.h"

static void initStateCallBacks(FsmState* pState) {
  pState->onActionPress   = NULL;
  pState->onActionRelease = NULL;
  pState->onBackPress          = NULL;
  pState->onDirection     = NULL;
  pState->onEnterState    = NULL;
  pState->onLeaveState    = NULL;
  pState->onReenterState  = NULL;
  pState->onTick          = NULL;
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

  callBacksOk &= isCallbackDefined(pState->onActionPress,   "onActionPress");
  // callBacksOk &= isCallbackDefined(pState->onActionRelease, "onActionRelease"); // TODO: make optional?
  callBacksOk &= isCallbackDefined(pState->onBackPress,     "onBackPress");
  // callBacksOk &= isCallbackDefined(pState->onBackRelease,   "onBackRelease");
  callBacksOk &= isCallbackDefined(pState->onDirection,     "onDirection");
  callBacksOk &= isCallbackDefined(pState->onEnterState,    "onEnterState");
  callBacksOk &= isCallbackDefined(pState->onLeaveState,    "onLeaveState");
  callBacksOk &= isCallbackDefined(pState->onReenterState,  "onReenterState");
  callBacksOk &= isCallbackDefined(pState->onTick,          "onTick");

  return callBacksOk;
}

void fsmInit(StackBasedFsm_t* pFsm) {
  pFsm->stackSize_ = 0;
}

bool fsmPush(StackBasedFsm_t* pFsm, TransitionFunc pFunc, void* pArgs) {
  FsmState* pCurrentState = fsmGetCurrentState(pFsm);

  if (pFsm->stackSize_ < FSM_STACK_SIZE) {
    FsmState* pNextState = &pFsm->stack[pFsm->stackSize_];
    pFsm->stackSize_++;

    if (pCurrentState)
      pCurrentState->onLeaveState(pFsm);
    
    initStateCallBacks(pNextState);
    pFunc(pFsm, pNextState, pArgs);

    if (!checkStateCallbacks(pNextState)) {
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

FsmState* fsmGetCurrentState(StackBasedFsm_t* pFsm) {
  if (pFsm->stackSize_ > 0)
    return &pFsm->stack[pFsm->stackSize_ - 1];
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

void processCursorButtons(StackBasedFsm_t* pFsm, FsmState* pState, InputDeviceStates_t* pButtonStates, 
    InputDeviceStates_t* pLastButtonStates, uint32_t* pTimeOnLastButtonPress) {

  static bool isInRepetitionMode = false;
  static uint32_t timeOnLastDirectionPress = 0;
  static uint32_t timeOnLastRepetition = 0;

  // TODO: simplify
  // Cursor delay and repetition, when holding direction button
  if (anyDirectionIsPressed(*pButtonStates)) {
    if (isLastDirectionEqual(*pButtonStates, *pLastButtonStates)) {
      if (isInRepetitionMode) {
        if (hal_clock() > timeOnLastDirectionPress + CURSOR_DELAY_MS_BEFORE_REPEAT) {
          if (hal_clock() > timeOnLastRepetition + 1000 / CURSOR_SPEED_ITEMS_PER_SECOND) {
            if (pState->onDirection)
              pState->onDirection(pFsm, pButtonStates->South, pButtonStates->North, pButtonStates->West, 
                  pButtonStates->East);

            // hal_printf("timeOnLastRepetition: %d, repeat on: %d", timeOnLastRepetition, timeOnLastRepetition + 1000 / CURSOR_SPEED_ITEMS_PER_SECOND);

            timeOnLastRepetition = hal_clock();
          }
        }
      }
      else {
        *pTimeOnLastButtonPress = hal_clock();
        timeOnLastDirectionPress = hal_clock();
        timeOnLastRepetition = hal_clock();
        isInRepetitionMode = true;

        // hal_printf("In repetition mode");
        if (pState->onDirection)
          pState->onDirection(pFsm, pButtonStates->South, pButtonStates->North, pButtonStates->West,
              pButtonStates->East);
      }
    }
  }
  else {
    if (isInRepetitionMode) {
      isInRepetitionMode = false;
      // hal_printf("End of repetition mode");
    }
  }
}

void processActionButtons(StackBasedFsm_t* pFsm, FsmState* pState, InputDeviceStates_t* pButtonStates,
    InputDeviceStates_t* pLastButtonStates, uint32_t* pTimeOnLastButtonPress) {

  // Force user to press button again after entering a menu
  // TODO: DRY!

  // Action button
  if (pButtonStates->Action && !pLastButtonStates->Action) {
    if (pState->onActionPress)
      pState->onActionPress(pFsm);

    *pTimeOnLastButtonPress = hal_clock();
  }

  if (!pButtonStates->Action && pLastButtonStates->Action) {
    if (pState->onActionRelease)
      pState->onActionRelease(pFsm);

    *pTimeOnLastButtonPress = hal_clock(); // CHECKME: Is this also needed on release for debouce?
  }

  // Back button
  if (pButtonStates->Back && !pLastButtonStates->Back) {
    if (pState->onBackPress)
      pState->onBackPress(pFsm);

    *pTimeOnLastButtonPress = hal_clock();
  }

  if (!pButtonStates->Back && pLastButtonStates->Back) {
    if (pState->onBackRelease)
      pState->onBackRelease(pFsm);

    *pTimeOnLastButtonPress = hal_clock(); // CHECKME: Is this also needed on release for debouce?
  }
}

bool isDebouncing(uint32_t timeOnLastButtonPress) {
  return hal_clock() > timeOnLastButtonPress + INPUT_DEVICE_DEBOUNCE_MS ? false : true;
}

void processInputDevice(StackBasedFsm_t* pFsm) {
  FsmState* pState = fsmGetCurrentState(pFsm);
  InputDeviceStates_t buttonStates = getInputDeviceState();
  static InputDeviceStates_t lastButtonStates = { 0 };
  static uint32_t timeOnLastButtonPress = 0;

  // TODO: do deboucing here!
  if (!isDebouncing(timeOnLastButtonPress)) { // CHECKME: it may be neccesarry to debounce each button individually
    processCursorButtons(pFsm, pState, &buttonStates, &lastButtonStates, &timeOnLastButtonPress);
    processActionButtons(pFsm, pState, &buttonStates, &lastButtonStates, &timeOnLastButtonPress);

    lastButtonStates = buttonStates;
  }
}

void fsmTick(StackBasedFsm_t* pFsm) {
  FsmState* pState = fsmGetCurrentState(pFsm);

  processInputDevice(pFsm);
  
  if (pState)
    if (pState->onTick)
      pState->onTick(pFsm);
}

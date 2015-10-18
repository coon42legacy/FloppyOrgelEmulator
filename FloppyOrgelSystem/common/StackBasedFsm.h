#ifndef __STACK_BASED_FSM_H
#define __STACK_BASED_FSM_H

#include <stdbool.h>
#include "config.h"

// Callback function pointer typedefs
typedef void(*OnActionCallback)();
typedef void(*OnBackCallback)();
typedef void(*OnEnterStateCallback)(void* pParams);
typedef void(*OnLeaveStateCallback)();
typedef void(*OnTickCallback)();
typedef void(*OnDirectionCallback)(bool south, bool north, bool west, bool east);

typedef struct {
  OnActionCallback onAction;
  OnBackCallback onBack;
  OnEnterStateCallback onEnterState;
  OnLeaveStateCallback onLeaveState;
  OnTickCallback onTick;
  OnDirectionCallback onDirection;
} FsmState;

typedef void(*TransitionFunc)(FsmState* state, void* args);

typedef struct {
  FsmState stack[FSM_STACK_SIZE];
  int stackSize_;
} StackBasedFsm_t;

void fsmInit(StackBasedFsm_t* fsm);
bool fsmPush(StackBasedFsm_t* fsm, TransitionFunc* newStateFunc, void* pArgs);
bool fsmPop(StackBasedFsm_t* fsm);
FsmState* fsmGetCurrentState(StackBasedFsm_t* fsm);
void fsmTick(StackBasedFsm_t* fsm);

#endif
#ifndef __STACK_BASED_FSM_H
#define __STACK_BASED_FSM_H

#include <stdbool.h>
#include "config.h"

typedef struct StackBasedFsm_t;

// Callback function pointer typedefs
typedef void(*OnActionCallback)();
typedef void(*OnBackCallback)();
typedef void(*OnEnterStateCallback)(struct StackBasedFsm_t* pFsm, void* pParams);
typedef void(*OnReenterStateCallback)();
typedef void(*OnLeaveStateCallback)();
typedef void(*OnTickCallback)();
typedef void(*OnDirectionCallback)(bool south, bool north, bool west, bool east);

typedef struct {
  OnActionCallback onAction;
  OnBackCallback onBack;
  OnEnterStateCallback onEnterState;
  OnReenterStateCallback onReenterState;
  OnLeaveStateCallback onLeaveState;
  OnTickCallback onTick;
  OnDirectionCallback onDirection;
} FsmState;

typedef struct {
  FsmState stack[FSM_STACK_SIZE];
  int stackSize_;
} StackBasedFsm_t;

typedef void(*TransitionFunc)(StackBasedFsm_t* pFsm, FsmState* pState, void* pArgs);

void fsmInit(StackBasedFsm_t* pFsm);
bool fsmPush(StackBasedFsm_t* pFsm, TransitionFunc pFunc, void* pArgs);
bool fsmPop(StackBasedFsm_t* pFsm);
FsmState* fsmGetCurrentState(StackBasedFsm_t* pFsm);
void fsmTick(StackBasedFsm_t* pFsm);

#endif
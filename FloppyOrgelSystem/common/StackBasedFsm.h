#ifndef __STACK_BASED_FSM_H
#define __STACK_BASED_FSM_H

#include <stdbool.h>
#include "config.h"

typedef struct FsmState FsmState;
typedef struct StackBasedFsm_t StackBasedFsm_t;

// Callback function pointer typedefs
typedef void(*OnActionCallback)();
typedef void(*OnBackCallback)();
typedef void(*OnEnterStateCallback)(StackBasedFsm_t* pFsm, void* pParams);
typedef void(*OnReenterStateCallback)();
typedef void(*OnLeaveStateCallback)();
typedef void(*OnTickCallback)();
typedef void(*OnDirectionCallback)(bool south, bool north, bool west, bool east);

struct FsmState {
  OnActionCallback onAction;
  OnBackCallback onBack;
  OnEnterStateCallback onEnterState;
  OnReenterStateCallback onReenterState;
  OnLeaveStateCallback onLeaveState;
  OnTickCallback onTick;
  OnDirectionCallback onDirection;
};

struct StackBasedFsm_t {
  FsmState stack[FSM_STACK_SIZE];
  int stackSize_;
};

typedef void(*TransitionFunc)(StackBasedFsm_t* pFsm, FsmState* pState, void* pArgs);

void fsmInit(StackBasedFsm_t* pFsm);
bool fsmPush(StackBasedFsm_t* pFsm, TransitionFunc pFunc, void* pArgs);
bool fsmPop(StackBasedFsm_t* pFsm);
FsmState* fsmGetCurrentState(StackBasedFsm_t* pFsm);
void fsmTick(StackBasedFsm_t* pFsm);

#endif
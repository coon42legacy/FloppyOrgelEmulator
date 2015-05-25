#ifndef __STACK_BASED_FSM_H
#define __STACK_BASED_FSM_H

#include <stdbool.h>
#include "config.h"

typedef struct {
  void* stack[FSM_STACK_SIZE]; // TODO: type should be FsmStateFunc but is not possible to define???
  int stackSize_;
} StackBasedFsm_t;

typedef void FsmState;
typedef FsmState (*FsmStateFunc)(StackBasedFsm_t* fsm);

void fsmInit(StackBasedFsm_t* fsm);
bool fsmPush(StackBasedFsm_t* fsm, FsmStateFunc stateFunc);
bool fsmPop(StackBasedFsm_t* fsm);
FsmState* fsmGetCurrentState(StackBasedFsm_t* fsm);
void fsmTick(StackBasedFsm_t* fsm);

#endif
#ifndef __STACK_BASED_FSM_H
#define __STACK_BASED_FSM_H

#include <stdbool.h>
#include "config.h"

typedef struct {
  void* stack[FSM_STACK_SIZE];
  int stackSize_;
} StackBasedFsm_t;

void fsmInit(StackBasedFsm_t* fsm);
bool fsmPush(StackBasedFsm_t* fsm, void* state);
bool fsmPop(StackBasedFsm_t* fsm);
void* fsmGetCurrentState(StackBasedFsm_t* fsm);
void fsmTick(StackBasedFsm_t* fsm);

#endif